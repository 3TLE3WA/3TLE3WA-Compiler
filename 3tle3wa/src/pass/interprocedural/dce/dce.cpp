#include "3tle3wa/pass/interprocedural/dce/dce.hh"

#include <math.h>

#include <algorithm>
#include <cassert>
#include <memory>
#include <queue>
#include <unordered_map>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instruction/controlFlowInst.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/memoryInst.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/ir/value/use.hh"

void DCE::EliminateUselessCode(NormalFuncPtr func) {
    auto seq_nodes = func->GetSequentialNodes();

    std::unordered_map<Instruction *, bool> visit;
    std::queue<InstPtr> WorkList;

    auto Mark = [&visit, &seq_nodes, &WorkList]() {
        for (auto &&node : seq_nodes) {
            for (auto &&inst : node->GetInstList()) {
                if (inst->IsCriticalOperation()) {
                    WorkList.push(inst);
                }
            }
        }
        while (!WorkList.empty()) {
            auto &&front = WorkList.front();
            WorkList.pop();
            if (visit[front.get()] == false) {
                visit[front.get()] = true;
                for (auto &&value : front->GetOprands()) {
                    auto &&inst_def_value = value->GetParent();
                    if (inst_def_value == nullptr) continue;
                    if (visit[inst_def_value.get()] == false) {
                        WorkList.push(inst_def_value);
                    }
                }
            }
        }
        assert(WorkList.empty());
    };

    auto EliminateRedundantStore = [&visit, seq_nodes]() {
        // addr is real use by call-inst except memset
        auto NotMemsetFunc = [](InstPtr inst) -> bool {
            assert(inst->IsCallInst());
            auto &&call_inst = std::static_pointer_cast<CallInst>(inst);
            return (call_inst->GetCalleeFunc()->GetFuncName() != "llvm.memset.p0i8.i64");
        };

        for (auto &&node : seq_nodes) {
            for (auto &&inst : node->GetInstList()) {
                if (inst->IsAllocaInst()) {
                    bool real_use = false;      // load, or non-memset call
                    InstList inst_list;         // related inst-list
                    std::queue<InstPtr> queue;  // iterate to get all related-inst
                    queue.push(std::static_pointer_cast<AllocaInst>(inst));
                    while (!queue.empty()) {
                        auto &&front = queue.front();
                        inst_list.push_front(front);
                        queue.pop();
                        if (front->IsLoadInst() || (front->IsCallInst() && NotMemsetFunc(front))) {  // read-use. stop
                            real_use = true;
                            break;
                        }
                        if (front->GetResult() != nullptr) {  // avoid store or void-call cause seg-fault
                            for (auto &&user : front->GetResult()->GetUserList()) {
                                queue.push(user);
                            }
                        }
                    }                         // iterate end
                    if (real_use == false) {  // alloca-addr is not real-use, tag related-inst to be deleted
                        for (auto &&inst : inst_list) {
                            visit[inst.get()] = false;
                        }
                    }
                }
            }
        }
        // no need to delete relate-inst, Sweep will do it
    };

    auto Sweep = [&visit, &seq_nodes]() {
        for (auto &&node : seq_nodes) {
            auto &&inst_list = node->GetInstList();
            for (auto &&iter = inst_list.begin(); iter != inst_list.end();) {
                auto &&inst = (*iter);
                if (visit[inst.get()] == false) {
                    RemoveInst(inst);
                    iter = inst_list.erase(iter);
                    continue;
                }
                ++iter;
            }
        }
    };

    Mark();
    // EliminateRedundantStore();
    Sweep();
}

void DCE::EliminateUselessControlFlow(NormalFuncPtr func) {
    // when processing a node
    // it cannot be a phi parameter
    std::map<CtrlFlowGraphNode *, bool> PhiParamBlock;
    std::map<CtrlFlowGraphNode *, CfgNodePtr> NodeMap;

    auto Initialition = [&func, &PhiParamBlock, &NodeMap]() {
        PhiParamBlock.clear();
        NodeMap.clear();
        for (auto node : func->GetSequentialNodes()) {
            NodeMap[node.get()] = node;
            for (auto inst : node->GetInstList()) {
                if (inst->IsPhiInst()) {
                    auto phi_inst = std::static_pointer_cast<PhiInst>(inst);
                    for (auto [value, block] : phi_inst->GetDataList()) {
                        PhiParamBlock[block.get()] = true;
                    }
                    // auto alloca_inst = phi_inst->GetOriginAlloca();
                    // PhiParamBlock[alloca_inst->GetParent().get()] = true;
                }
            }
        }
    };

    auto FlodRedundantBranch = [&PhiParamBlock](const BaseValue *cond, const CfgNodePtr &node, const CfgNodePtr &iftrue,
                                                const CfgNodePtr &iffalse) {
        bool changed = false;
        bool phi_iftrue = PhiParamBlock[iftrue.get()];
        bool phi_iffalse = PhiParamBlock[iffalse.get()];
        if ((iftrue == iffalse) && (phi_iftrue == false)) {
            node->RemoveLastInst();
            node->InsertInstBack(JumpInst::CreatePtr(iftrue, node));
            changed = true;
        } else if (cond->IsConstant()) {
            auto conditon = std::get<bool>(static_cast<const Constant *>(cond)->GetValue());
            if (conditon == true && phi_iftrue == false) {
                node->RemoveLastInst();
                node->InsertInstBack(JumpInst::CreatePtr(iftrue, node));
                node->RmvSuccessor(iffalse);
                iffalse->RmvPredecessor(node);
                changed = true;
            } else if (conditon == false && phi_iffalse == false) {
                node->RemoveLastInst();
                node->InsertInstBack(JumpInst::CreatePtr(iffalse, node));
                node->RmvSuccessor(iftrue);
                iftrue->RmvPredecessor(node);
                changed = true;
            }
        }
        return changed;
    };

    auto RemoveEmptyBlock = [](const CfgNodePtr &i, const CfgNodePtr &j) {
        auto &&predecessors = i->GetPredecessors();

        for (auto &&pred : predecessors) {
            pred->AddSuccessor(j);
            j->AddPredecessor(pred);
            pred->GetLastInst()->ReplaceTarget(i, j);

            pred->blk_attr.CombineBlkAttr(i->blk_attr);
        }
        RemoveNode(i);
    };

    auto CombineBlocks = [&func](const CfgNodePtr &i, const CfgNodePtr &j) {
        i->RemoveLastInst();
        auto &&i_inst_list = i->GetInstList();
        auto &&j_inst_list = j->GetInstList();
        for_each(i_inst_list.begin(), i_inst_list.end(), [&j](const auto &inst) { inst->SetParent(j); });
        i_inst_list.insert(i_inst_list.end(), j_inst_list.begin(), j_inst_list.end());
        j_inst_list = std::move(i_inst_list);

        j->blk_attr.CombineBlkAttr(i->blk_attr);

        if (j->blk_attr.ChkOneOfBlkType(BlkAttr::Entry)) {
            func->SetEntryNode(j);
        }

        // ReplacePredSucc(i, j);
        for (auto &&pred : i->GetPredecessors()) {
            pred->AddSuccessor(j);
            j->AddPredecessor(pred);
            pred->GetLastInst()->ReplaceTarget(i, j);
        }
        RemoveNode(i);
    };

    CRVC_UNUSE auto HoistBranch = [](CRVC_UNUSE const CfgNodePtr &i, CRVC_UNUSE const CfgNodePtr &j) {};

    auto AdjustOriAlloca = [&func, &NodeMap]() {
        for (auto &&node : func->GetSequentialNodes()) {
            for (auto &&inst : node->GetInstList()) {
                if (inst->IsPhiInst()) {
                    auto &&phi_inst = std::static_pointer_cast<PhiInst>(inst);
                    auto &&origin_alloca = phi_inst->GetOriginAlloca();

                    // iterate to get a fixed parent
                    auto parent = origin_alloca->GetParent();
                    while (parent != NodeMap[parent.get()]) {
                        parent = NodeMap[parent.get()];
                    }
                    origin_alloca->SetParent(parent);
                }
            }
        }
    };

    auto OnePass = [&](CfgNodeList &seq_nodes) {
        Initialition();

        bool changed = false;
        for (auto &&iter = seq_nodes.begin(); iter != seq_nodes.end();) {
            auto i = (*iter);

            // if i act as a phi parameter, cannot be processed
            if (PhiParamBlock[i.get()]) {
                ++iter;
                continue;
            }
            assert(!PhiParamBlock[i.get()]);
            auto last_inst = i->GetLastInst();
            if (last_inst->IsBranchInst()) {
                // case 1, fold redundant branch
                auto br_inst = std::static_pointer_cast<BranchInst>(last_inst);
                auto cond = br_inst->GetCondition().get();
                auto iftrue = br_inst->GetTrueTarget();
                auto iffalse = br_inst->GetFalseTarget();

                changed = FlodRedundantBranch(cond, i, iftrue, iffalse);
            }
            last_inst = i->GetLastInst();
            if (last_inst->IsJumpInst()) {
                auto jump_inst = std::static_pointer_cast<JumpInst>(last_inst);
                auto j = jump_inst->GetTarget();

                // case 2, remove empty block
                if (i->GetPredecessors().size() && i->GetInstCnt() == 1 && i->GetLastInst()->IsJumpInst()) {
                    RemoveEmptyBlock(i, j);
                    changed = true;
                    iter = seq_nodes.erase(iter);
                    continue;
                }
                // case 3, combine i and j
                if (j->GetPredecessors().size() == 1) {
                    CombineBlocks(i, j);
                    NodeMap[i.get()] = j;
                    changed = true;
                    iter = seq_nodes.erase(iter);
                    continue;
                }
                // case 4, hoist a branch
            }
            ++iter;
        }
        AdjustOriAlloca();
        return changed;
    };

    while (true) {
        // although remove false-branch of branch-inst
        // false-branch still exsit in control-flow-graph, without predecessors but have successors
        // so, use EliminateUnreachableCode to fix control-flow-graph
        DCE::EliminateUnreachableCode(func);
        auto &&seq_nodes = func->GetSequentialNodes();
        if (!OnePass(seq_nodes)) break;
    }

    for (auto &&node : func->GetSequentialNodes()) {
        for (auto &&inst : node->GetInstList()) {
            if (inst->IsPhiInst()) {
                auto &&phi_inst = std::static_pointer_cast<PhiInst>(inst);
                auto &&origin_alloca = phi_inst->GetOriginAlloca();

                // iterate to get a fixed parent
                auto parent = origin_alloca->GetParent();
                while (parent != NodeMap[parent.get()]) {
                    parent = NodeMap[parent.get()];
                }

                origin_alloca->SetParent(parent);
            }
        }
    }
}

void DCE::EliminateUnreachableCode(NormalFuncPtr func) {
    std::queue<CfgNodePtr> queue;
    std::unordered_map<CtrlFlowGraphNode *, bool> visit;

    CfgNodeList seq_nodes;
    queue.push(func->GetEntryNode());
    while (!queue.empty()) {
        auto &&front = queue.front();
        queue.pop();
        if (!visit[front.get()]) {
            visit[front.get()] = true;
            seq_nodes.push_back(front);
            for (auto &&node : front->GetSuccessors()) {
                queue.push(node);
            }
        }
    }
    assert(queue.empty());
    visit.clear();

    CfgNodeList delNodes;
    queue.push(func->GetExitNode());
    while (!queue.empty()) {
        auto &&front = queue.front();
        queue.pop();
        if (!visit[front.get()]) {
            visit[front.get()] = true;
            for (auto &&pred : front->GetPredecessors()) {
                queue.push(pred);
                if (std::find(seq_nodes.begin(), seq_nodes.end(), pred) == seq_nodes.end()) {
                    delNodes.push_back(pred);
                }
            }
        }
    }
    assert(queue.empty());

    // remove dead-node
    std::for_each(delNodes.begin(), delNodes.end(), RemoveNode);
}

void DCE::DCE(NormalFuncPtr func) {
    EliminateUselessCode(func);
    EliminateUselessControlFlow(func);
}
