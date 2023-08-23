#include "3tle3wa/pass/interprocedural/ssa/ssa.hh"

#include <memory>
#include <unordered_map>

#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/memoryInst.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/pass/interprocedural/dce/dce.hh"

SSA::RenameData::RenameData(CfgNodePtr _node, CfgNodePtr _pred, ValueVector _v)
    : node(_node), pred(_pred), valuelist(_v) {}

SSA::RenameDatePtr SSA::RenameData::CreatePtr(CfgNodePtr _node, CfgNodePtr _pred, ValueVector _valuelist) {
    return std::make_shared<RenameData>(_node, _pred, _valuelist);
}

void SSA::SSAConstruction(NormalFuncPtr func) {
    auto entry = func->GetEntryNode();
    auto allNodes = func->GetSequentialNodes();

    std::vector<std::set<CfgNodePtr>> defBlocks;
    std::list<AllocaInstPtr> allocaInsts;
    std::unordered_map<AllocaInstPtr, int32_t> allocaLookUp;
    std::unordered_map<PhiInstPtr, int32_t> phi2AllocaMap;

    auto InsertPhiFunction = [&allNodes, &defBlocks, &allocaInsts, &allocaLookUp, &phi2AllocaMap]() {
        for (auto &&node : allNodes) {
            for (auto &&inst : node->GetInstList()) {
                if (inst->IsAllocaInst()) {
                    auto &&alloca_inst = std::static_pointer_cast<AllocaInst>(inst);
                    if (alloca_inst->GetAllocaType()->IsScalar()) {
                        defBlocks.push_back(std::set<CfgNodePtr>());
                        allocaInsts.push_back(alloca_inst);
                        allocaLookUp[alloca_inst] = allocaInsts.size() - 1;
                    }
                }
            }
        }
        for (auto &&node : allNodes) {
            for (auto &&inst : node->GetInstList()) {
                if (inst->IsStoreInst()) {
                    auto &&store_inst = std::static_pointer_cast<StoreInst>(inst);
                    auto &&inst_gen_store_addr = store_inst->GetStoreAddr()->GetParent();
                    if (inst_gen_store_addr && inst_gen_store_addr->IsAllocaInst()) {
                        auto &&alloca_inst = std::static_pointer_cast<AllocaInst>(inst_gen_store_addr);
                        if (allocaLookUp.find(alloca_inst) != allocaLookUp.end()) {
                            defBlocks[allocaLookUp[alloca_inst]].insert(node);
                        }
                    }
                }
            }
        }

        auto InDefBlocks = [&defBlocks](int32_t index, CfgNodePtr node) {
            auto &&blockSet = defBlocks[index];
            if (blockSet.find(node) != blockSet.end()) {
                return true;
            }
            return false;
        };
        std::queue<CfgNodePtr> WorkList;
        for (auto &&alloca_inst : allocaInsts) {
            int32_t index = allocaLookUp[alloca_inst];
            BaseTypePtr type = alloca_inst->GetAllocaType();
            std::for_each(allNodes.begin(), allNodes.end(), [](const auto &node) { node->SetDirty(false); });
            std::for_each(defBlocks[index].begin(), defBlocks[index].end(),
                          [&WorkList](const auto &node) { WorkList.push(node); });
            while (!WorkList.empty()) {
                CfgNodePtr front = WorkList.front();
                WorkList.pop();
                for (auto &&df : front->GetDomFrontier()) {
                    if (!df->GetDirty()) {
                        df->SetDirty(true);
                        auto &&phi_inst = PhiInst::CreatePtr(type, df);
                        phi_inst->SetOriginAlloca(alloca_inst);
                        phi2AllocaMap[phi_inst] = index;
                        if (!InDefBlocks(index, df)) {
                            WorkList.push(df);
                        }
                    }
                }
            }
        }
    };

    auto VariableRename = [&entry, &allNodes, &allocaInsts, &allocaLookUp, &phi2AllocaMap]() {
        ValueVector valuelist;
        std::for_each(allocaInsts.begin(), allocaInsts.end(), [&valuelist](const auto &inst) {
            valuelist.push_back(inst->GetAllocaType()->IntType()
                                    ? ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))
                                    : ConstantAllocator::FindConstantPtr(static_cast<float>(0)));
        });
        std::for_each(allNodes.begin(), allNodes.end(), [](const auto &node) { node->SetDirty(false); });

        std::stack<RenameDatePtr> renameDateStack;
        renameDateStack.push(RenameData::CreatePtr(entry, nullptr, valuelist));
        while (!renameDateStack.empty()) {
            RenameDatePtr top = renameDateStack.top();
            renameDateStack.pop();
            ValueVector cur_valuelist = top->valuelist;

            for (auto &&inst : top->node->GetInstList()) {
                if (inst->IsPhiInst()) {
                    auto &&phi_inst = std::static_pointer_cast<PhiInst>(inst);
                    if (phi2AllocaMap.find(phi_inst) != phi2AllocaMap.end()) {
                        PhiInst::InsertPhiData(phi_inst, top->valuelist[phi2AllocaMap[phi_inst]], top->pred);
                    }
                }
            }

            if (top->node->GetDirty()) continue;
            top->node->SetDirty(true);

            auto &&inst_list = top->node->GetInstList();
            for (auto &&iter = inst_list.begin(); iter != inst_list.end();) {
                auto &&inst = (*iter);
                if (inst->IsAllocaInst()) {
                    auto &&alloca_inst = std::static_pointer_cast<AllocaInst>(inst);
                    if (allocaLookUp.find(alloca_inst) != allocaLookUp.end()) {
                        iter = inst_list.erase(iter);
                        continue;
                    }
                } else if (inst->IsLoadInst()) {
                    auto &&load_inst = std::static_pointer_cast<LoadInst>(inst);
                    auto &&inst_gen_load_addr = load_inst->GetOprand()->GetParent();
                    if (inst_gen_load_addr && inst_gen_load_addr->IsAllocaInst()) {
                        auto &&alloca_inst = std::static_pointer_cast<AllocaInst>(inst_gen_load_addr);
                        int32_t allocaIndex = allocaLookUp[alloca_inst];
                        auto &&load_result = load_inst->GetResult();
                        ReplaceSRC(load_inst->GetResult(), cur_valuelist[allocaIndex]);
                        RemoveInst(load_inst);
                        iter = inst_list.erase(iter);
                        continue;
                    }
                } else if (inst->IsStoreInst()) {
                    auto &&store_inst = std::static_pointer_cast<StoreInst>(inst);
                    auto &&inst_gen_store_addr = store_inst->GetStoreAddr()->GetParent();
                    if (inst_gen_store_addr && inst_gen_store_addr->IsAllocaInst()) {
                        auto &&alloca_inst = std::static_pointer_cast<AllocaInst>(inst_gen_store_addr);
                        int32_t allocaIndex = allocaLookUp[alloca_inst];
                        cur_valuelist[allocaIndex] = store_inst->GetStoreValue();
                        RemoveInst(store_inst);
                        iter = inst_list.erase(iter);
                        continue;
                    }
                } else if (inst->IsPhiInst()) {
                    auto &&phi_inst = std::static_pointer_cast<PhiInst>(inst);
                    int allocaIndex = phi2AllocaMap[phi_inst];
                    cur_valuelist[allocaIndex] = phi_inst->GetResult();
                }
                ++iter;
            }

            for (auto &&succ : top->node->GetSuccessors()) {
                renameDateStack.push(RenameData::CreatePtr(succ, top->node, cur_valuelist));
            }
        }
    };

    InsertPhiFunction();
    VariableRename();

    // remove redundant phi-inst
    DCE::EliminateUselessCode(func);
}

void SSA::SSADestruction(NormalFuncPtr func) {
    auto &&allNodes = func->GetSequentialNodes();
    std::unordered_map<AllocaInst *, bool> visit;

    for (auto node : func->GetSequentialNodes()) {
        auto &&inst_list = node->GetInstList();
        for (auto &&iter = inst_list.begin(); iter != inst_list.end();) {
            auto inst = (*iter);
            if (inst->IsPhiInst()) {
                auto phi_inst = std::static_pointer_cast<PhiInst>(inst);
                auto result = phi_inst->GetResult();

                auto alloca_inst = phi_inst->GetOriginAlloca();

                if (visit[alloca_inst.get()] == false) {
                    auto node = alloca_inst->GetParent();
                    node->InsertInstFront(alloca_inst);
                    visit[alloca_inst.get()] = true;
                }

                auto addr = alloca_inst->GetAllocaAddr();

                auto datalist = phi_inst->GetDataList();
                for (auto [value, block] : datalist) {
                    auto &&store_inst = StoreInst::CreatePtr(addr, value, block);
                    addr->InsertUser(store_inst);
                    value->InsertUser(store_inst);

                    auto &&block_inst_list = block->GetInstList();
                    block_inst_list.insert(--block_inst_list.end(), store_inst);
                }

                RemoveInst(phi_inst);

                iter = inst_list.erase(iter);

                auto load_inst = LoadInst::CreatePtr(result, addr, node);
                result->SetParent(load_inst);
                addr->InsertUser(load_inst);
                inst_list.insert(iter, load_inst);
                continue;
            }
            ++iter;
        }
    }
}
