#include "3tle3wa/ir/function/structure/loop.hh"

#include <cassert>
#include <cstddef>
#include <memory>
#include <queue>
#include <sstream>
#include <stack>
#include <unordered_map>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/structure/structure.hh"
#include "3tle3wa/ir/instruction/controlFlowInst.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/pass/analysis/structure/structure.hh"

CfgNodeList Loop::GetCondBodyBlks() const {
    CfgNodeList list;
    std::unordered_map<CtrlFlowGraphNode *, bool> visit;

    auto &&ChkAllPredVisit = [&visit](CtrlFlowGraphNode *node) {
        if (node->blk_attr.cond_begin) return true;
        if (node->blk_attr.cond_end) return false;
        assert(node->GetPredecessors().size() > 0);
        for (const auto &pred : node->GetPredecessors()) {
            // if (pred->blk_attr.body_end) continue;                            // body-end jump to cond-begin
            if (pred->blk_attr.ChkOneOfBlkType(BlkAttr::Continue)) continue;  // continue to cond-begin
            if (!visit[pred.get()]) return false;
        }
        return true;
    };

    std::stack<CfgNodePtr> stack;
    stack.push(cond_begin);
    while (!stack.empty()) {
        auto top = stack.top();
        stack.pop();

        if (!visit[top.get()] && ChkAllPredVisit(top.get())) {
            visit[top.get()] = true;
            list.push_back(top);

            auto &&last_inst = top->GetLastInst().get();
            if (last_inst->IsBranchInst()) {
                BranchInst *br_inst = static_cast<BranchInst *>(last_inst);
                auto &&lhs = br_inst->GetTrueTarget();
                auto &&rhs = br_inst->GetFalseTarget();

                if (lhs->blk_attr.cond_begin || rhs->blk_attr.cond_begin) {
                    if (lhs->blk_attr.cond_begin) {
                        auto &&sub_loop = StructureAnalysis::FindInNode2Loop(lhs.get());
                        assert(sub_loop != nullptr);
                        auto &&sub_loop_blks = sub_loop->GetEntireStructure();
                        for (auto &&blk : sub_loop_blks) {
                            visit[blk.get()] = true;
                            list.push_back(blk);
                        }
                    }
                    if (rhs->blk_attr.cond_begin) {
                        auto &&sub_loop = StructureAnalysis::FindInNode2Loop(rhs.get());
                        auto &&sub_loop_blks = sub_loop->GetEntireStructure();
                        for (auto &&blk : sub_loop_blks) {
                            visit[blk.get()] = true;
                            list.push_back(blk);
                        }
                        stack.push(sub_loop->loop_exit);
                    }
                    if (lhs->blk_attr.cond_begin) {
                        auto &&sub_loop = StructureAnalysis::FindInNode2Loop(lhs.get());
                        assert(sub_loop != nullptr);
                        stack.push(sub_loop->loop_exit);
                    }
                }
                if (!rhs->blk_attr.cond_begin && !rhs->blk_attr.body_begin && !rhs->blk_attr.structure_out) {
                    stack.push(rhs);
                }
                if (!lhs->blk_attr.cond_begin && !lhs->blk_attr.body_begin && !lhs->blk_attr.structure_out) {
                    stack.push(lhs);
                }
            } else if (last_inst->IsJumpInst()) {
                JumpInst *jump_inst = static_cast<JumpInst *>(last_inst);
                auto &&target = jump_inst->GetTarget();

                if (target->blk_attr.cond_begin) {
                    auto &&sub_loop = StructureAnalysis::FindInNode2Loop(target.get());
                    assert(sub_loop != nullptr);
                    auto &&sub_loop_blks = sub_loop->GetEntireStructure();
                    for (auto &&blk : sub_loop_blks) {
                        visit[blk.get()] = true;
                        list.push_back(blk);
                    }
                    stack.push(sub_loop->loop_exit);
                } else if (!target->blk_attr.body_begin && !target->blk_attr.structure_out) {
                    stack.push(target);
                }
            }
        }
    }
    if (!visit[cond_end.get()]) list.push_back(cond_end);
    return list;
}

CfgNodeList Loop::GetLoopBodyBlks() const {
    CfgNodeList list;
    std::unordered_map<CtrlFlowGraphNode *, bool> visit;

    auto &&ChkAllPredVisit = [&visit](CtrlFlowGraphNode *node) {
        if (node->blk_attr.body_begin) return true;
        assert(node->GetPredecessors().size() > 0);
        for (const auto &pred : node->GetPredecessors()) {
            if (!visit[pred.get()]) return false;
        }
        return true;
    };

    std::stack<CfgNodePtr> stack;
    stack.push(body_begin);
    while (!stack.empty()) {
        auto top = stack.top();
        stack.pop();

        if (!visit[top.get()] && ChkAllPredVisit(top.get())) {
            visit[top.get()] = true;
            list.push_back(top);

            auto &&last_inst = top->GetLastInst().get();
            if (last_inst->IsBranchInst()) {
                BranchInst *br_inst = static_cast<BranchInst *>(last_inst);
                auto &&lhs = br_inst->GetTrueTarget();
                auto &&rhs = br_inst->GetFalseTarget();

                if (lhs->blk_attr.cond_begin || rhs->blk_attr.cond_begin) {
                    if (lhs->blk_attr.cond_begin) {
                        auto &&sub_loop = StructureAnalysis::FindInNode2Loop(lhs.get());
                        assert(sub_loop != nullptr);
                        auto &&sub_loop_blks = sub_loop->GetEntireStructure();
                        for (auto &&blk : sub_loop_blks) {
                            visit[blk.get()] = true;
                            list.push_back(blk);
                        }
                    }
                    if (rhs->blk_attr.cond_begin) {
                        auto &&sub_loop = StructureAnalysis::FindInNode2Loop(rhs.get());
                        auto &&sub_loop_blks = sub_loop->GetEntireStructure();
                        for (auto &&blk : sub_loop_blks) {
                            visit[blk.get()] = true;
                            list.push_back(blk);
                        }
                        stack.push(sub_loop->loop_exit);
                    }
                    if (lhs->blk_attr.cond_begin) {
                        auto &&sub_loop = StructureAnalysis::FindInNode2Loop(lhs.get());
                        assert(sub_loop != nullptr);
                        stack.push(sub_loop->loop_exit);
                    }
                }

                if (!rhs->blk_attr.cond_begin && !rhs->blk_attr.structure_out) {
                    stack.push(rhs);
                }
                if (!lhs->blk_attr.cond_begin && !lhs->blk_attr.structure_out) {
                    stack.push(lhs);
                }
            } else if (last_inst->IsJumpInst()) {
                JumpInst *jump_inst = static_cast<JumpInst *>(last_inst);
                auto &&target = jump_inst->GetTarget();

                // if target is cond-begin, check whether corresponding-loop is itself
                if (target->blk_attr.cond_begin && StructureAnalysis::FindInNode2Loop(target.get()) != this) {
                    auto &&sub_loop = StructureAnalysis::FindInNode2Loop(target.get());
                    assert(sub_loop != nullptr);
                    auto &&sub_loop_blks = sub_loop->GetEntireStructure();
                    for (auto &&blk : sub_loop_blks) {
                        visit[blk.get()] = true;
                        list.push_back(blk);
                    }
                    stack.push(sub_loop->loop_exit);
                } else if (!(top->blk_attr.ChkOneOfBlkType(BlkAttr::GoReturn, BlkAttr::InlineGR) &&
                             this == target->blk_attr.loop) &&
                           !target->blk_attr.cond_begin && !target->blk_attr.structure_out) {
                    stack.push(target);
                }
            }
        }
    }
    return list;
}

CfgNodeList Loop::GetEntireStructure() const {
    auto &&cond = GetCondBodyBlks();
    auto &&body = GetLoopBodyBlks();

    auto &&entire = cond;
    entire.insert(entire.end(), body.begin(), body.end());

    return entire;
}

bool Loop::IsSimpleLoop() const {
    assert(before_blk != nullptr);
    if (cond_begin->GetPredecessors().size() != 2) return false;  // before-blk + real-loop-end, exclude `continue`
    if (loop_exit->GetPredecessors().size() != 1) return false;   // cond-end, exclude `break`
    if (GetCondBodyBlks().size() > 1) return false;               // too much conditions
    for (auto &&node : GetEntireStructure()) {                    // exclude `return` in entire-loop
        if (node->blk_attr.ChkOneOfBlkType(BlkAttr::GoReturn)) {
            return false;
        }
    }
    return true;
}

void Loop::PrintCurStructure() const {
    auto &&PrintTab = [](depth_t depth) {
        std::stringstream ss;
        for (size_t idx = 0; idx < depth; ++idx) {
            ss << "\t";
        }
        return ss.str();
    };

    if (before_blk) {
        cout << PrintTab(depth) << "\b\bLoop_" << depth << " " << (IsSimpleLoop() ? "simple" : "complex") << endl;
        cout << PrintTab(depth) << "Before-Loop: Block_" << before_blk->GetBlockIdx() << endl;
        cout << PrintTab(depth) << "Cond-Begin : Block_" << cond_begin->GetBlockIdx() << endl;
        cout << PrintTab(depth) << "Cond-End   : Block_" << cond_end->GetBlockIdx() << endl;
        cout << PrintTab(depth) << "Body-Begin : Block_" << body_begin->GetBlockIdx() << endl;
        cout << PrintTab(depth) << "Loop-Exit  : Block_" << loop_exit->GetBlockIdx() << endl;

        cout << PrintTab(depth) << "  Conditions  :";
        for (auto cond : GetCondBodyBlks()) {
            cout << " Block_" << cond->GetBlockIdx();
        }
        cout << endl;

        cout << PrintTab(depth) << "  Loop-Bodys  :";
        for (auto body : GetLoopBodyBlks()) {
            cout << " Block_" << body->GetBlockIdx();
        }
        cout << endl;
    }
}

void Loop::PrintStructure() const {
    PrintCurStructure();
    for (auto &&sub_structure : sub_structures) {
        sub_structure->PrintStructure();
    }
}
