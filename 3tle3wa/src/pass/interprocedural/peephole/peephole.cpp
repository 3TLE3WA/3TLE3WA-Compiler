#include "3tle3wa/pass/interprocedural/peephole/peephole.hh"

#include <cassert>
#include <cstdint>
#include <iterator>
#include <memory>
#include <unordered_map>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instruction/binaryOpInst.hh"
#include "3tle3wa/ir/instruction/compareInst.hh"
#include "3tle3wa/ir/instruction/controlFlowInst.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/memoryInst.hh"
#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/ir/instruction/unaryOpInst.hh"
#include "3tle3wa/ir/symTable.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/constarray.hh"
#include "3tle3wa/ir/value/globalvalue.hh"
#include "3tle3wa/ir/value/type/scalarType.hh"
#include "3tle3wa/ir/value/variable.hh"
#include "3tle3wa/pass/interprocedural/dce/dce.hh"

void PeepHole::PeepHoleOpt(NormalFuncPtr &func) {
    for (auto &&node : func->GetSequentialNodes()) {
        auto &&inst_list = node->GetInstList();
        for (auto &&iter = inst_list.begin(); iter != inst_list.end();) {
            auto &&inst = (*iter);
            if (inst->IsPhiInst()) {  // fabs
                auto &&phi = std::static_pointer_cast<PhiInst>(inst);
                auto &&phi_result = phi->GetResult();

                auto &&phi_list = phi->GetDataList();
                if (phi_list.size() == 2) {
                    auto &&[value1, block1] = phi_list.front();
                    auto &&[value2, block2] = phi_list.back();
                    // check simple if-else
                    if (block1->GetPredecessors().size() == 1 && block2->GetPredecessors().size() == 1 &&
                        (*block1->GetPredecessors().begin() == *block2->GetPredecessors().begin())) {
                        auto &&cond_blk = (*block1->GetPredecessors().begin());
                        // cout << cond_blk->tollvmIR() << endl;

                        if (auto &&br_inst = std::dynamic_pointer_cast<BranchInst>(cond_blk->GetLastInst())) {
                            auto &&cond = br_inst->GetCondition();
                            auto &&iftrue = br_inst->GetTrueTarget();
                            auto &&iffalse = br_inst->GetFalseTarget();
                            // check float compare
                            if (auto &&fcmp_inst = std::dynamic_pointer_cast<FCmpInst>(cond->GetParent())) {
                                auto opcode = fcmp_inst->GetOpCode();
                                auto &&lhs = fcmp_inst->GetLHS();
                                auto &&rhs = fcmp_inst->GetRHS();
                                auto &&zero = ConstantAllocator::FindConstantPtr(static_cast<float>(0));

                                std::unordered_map<CtrlFlowGraphNode *, BaseValuePtr> map;
                                map[block1.get()] = value1;
                                map[block2.get()] = value2;
                                // check fabs condition
                                if ((opcode == OP_LTH || opcode == OP_LEQ) && rhs == zero) {
                                    if (lhs == map[iffalse.get()]) {
                                        if (auto &&fneg_inst =
                                                std::dynamic_pointer_cast<FNegInst>(map[iftrue.get()]->GetParent());
                                            fneg_inst != nullptr && fneg_inst->GetOprand() == lhs) {
                                            // create fabs-inst
                                            auto &&fabs_inst = FAbsInst::CreatePtr(phi_result, lhs, cond_blk);
                                            lhs->InsertUser(fabs_inst);
                                            phi_result->SetParent(fabs_inst);
                                            // insert fabs-inst into cond-blk
                                            auto &&cond_blk_inst_list = cond_blk->GetInstList();
                                            auto &&cond_blk_iter = cond_blk_inst_list.end();
                                            std::advance(cond_blk_iter, -1);
                                            cond_blk_inst_list.insert(cond_blk_iter, fabs_inst);
                                            // simpfly
                                            if (iftrue->GetInstCnt() == 2 && iffalse->GetInstCnt() == 1) {
                                                RemoveInst(br_inst);
                                                cond_blk->RemoveLastInst();
                                                RemoveNode(iftrue);
                                                RemoveNode(iffalse);
                                                cond_blk->InsertInstBack(JumpInst::CreatePtr(node, cond_blk));
                                            }
                                            // remove ori-phi-inst
                                            iter = inst_list.erase(iter);
                                            continue;
                                        }
                                    }
                                } else if ((opcode == OP_GTH || opcode == OP_GEQ) && rhs == zero) {
                                    if (lhs == map[iftrue.get()]) {
                                        if (auto &&fneg_inst =
                                                std::dynamic_pointer_cast<FNegInst>(map[iffalse.get()]->GetParent());
                                            fneg_inst != nullptr && fneg_inst->GetOprand() == lhs) {
                                            // create fabs-inst
                                            auto &&fabs_inst = FAbsInst::CreatePtr(phi_result, lhs, cond_blk);
                                            lhs->InsertUser(fabs_inst);
                                            phi_result->SetParent(fabs_inst);
                                            // insert fabs-inst into cond-blk
                                            auto &&cond_blk_inst_list = cond_blk->GetInstList();
                                            auto &&cond_blk_iter = cond_blk_inst_list.end();
                                            std::advance(cond_blk_iter, -1);
                                            cond_blk_inst_list.insert(cond_blk_iter, fabs_inst);
                                            // simpfly
                                            if (iftrue->GetInstCnt() == 1 && iffalse->GetInstCnt() == 2) {
                                                RemoveInst(br_inst);
                                                cond_blk->RemoveLastInst();
                                                RemoveNode(iftrue);
                                                RemoveNode(iffalse);
                                                cond_blk->InsertInstBack(JumpInst::CreatePtr(node, cond_blk));
                                            }
                                            // remove ori-phi-inst
                                            iter = inst_list.erase(iter);
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else if (inst->IsLoadInst()) {
                auto &&load_inst = std::static_pointer_cast<LoadInst>(inst);
                if (load_inst->GetOprand()->IsVariable()) {  // load-addr, variable or global-value
                    auto &&load_addr = std::static_pointer_cast<Variable>(load_inst->GetOprand());
                    if (load_addr->GetParent() && load_addr->GetParent()->IsGepInst()) {
                        auto &&gep_inst = std::static_pointer_cast<GetElementPtrInst>(load_addr->GetParent());

                        auto &&base_addr = gep_inst->GetBaseAddr();
                        auto &&offset = gep_inst->GetOffList().back();
                        if (base_addr->IsGlobalValue() && offset->IsConstant()) {
                            auto &&glb_value = std::static_pointer_cast<GlobalValue>(base_addr);

                            auto &&init_value = glb_value->GetInitValue();
                            if (glb_value->GetBaseType()->IsImMutable() && init_value->IsConstArray()) {
                                const auto &&const_arr = std::static_pointer_cast<ConstArray>(init_value);
                                auto &&init_arr = const_arr->GetConstArr();
                                auto &&load_value =
                                    init_arr[std::get<int32_t>(std::static_pointer_cast<Constant>(offset)->GetValue())];
                                ReplaceSRC(load_inst->GetResult(), load_value);
                            }
                        }
                    }
                }
            }
            ++iter;
        }
    }
}

void PeepHole::PeepHole4Gep(NormalFuncPtr &func, SymbolTable &glb_table) {
    auto PreLoadGlbAddr = [&func, &glb_table]() -> void {
        // insert glb-address getelementptr first
        auto &&entry = func->GetEntryNode();
        std::list<GepInstPtr> gep_list;

        std::unordered_map<BaseValue *, BaseValuePtr> addr_map;
        for (auto &&[_, value] : glb_table.GetNameValueMap()) {
            auto &&base_type = value->GetBaseType();
            if (base_type->IsScalar()) continue;

            auto &&glb_value = std::static_pointer_cast<GlobalValue>(value);
            const auto &used_in = glb_value->GetUsedIn();
            const auto &define_in = glb_value->GetDefineIn();

            if (used_in.find(func.get()) != used_in.end() || define_in.find(func.get()) != define_in.end()) {
                auto store_type = glb_value->GetInitValue()->GetBaseType();
                OffsetList off_list = OffsetList(2, ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0)));
                auto &&result = Variable::CreatePtr(base_type->IntType() ? type_int_ptr_L : type_float_ptr_L, nullptr);
                auto &&gep = GetElementPtrInst::CreatePtr(result, store_type, glb_value, off_list, entry);
                result->SetParent(gep);

                addr_map[glb_value.get()] = result;

                gep_list.push_back(gep);
            }
        }
        // replace first
        for (auto &&node : func->GetSequentialNodes()) {
            for (auto &&inst : node->GetInstList()) {
                if (inst->IsGepInst()) {
                    auto gep = std::static_pointer_cast<GetElementPtrInst>(inst);
                    if (gep->GetBaseAddr()->IsGlobalValue()) {  // avoid replace
                        auto ori_base_addr = gep->GetBaseAddr();
                        auto mapped_addr = addr_map[ori_base_addr.get()];
                        if (mapped_addr != nullptr) {
                            ori_base_addr->RemoveUser(gep);
                            gep->SetBaseAddr(mapped_addr);
                            mapped_addr->InsertUser(gep);
                        }
                    }
                }
            }
        }
        // fill in second
        auto &&inst_list = entry->GetInstList();
        for (auto &&iter = inst_list.begin();; ++iter) {
            if ((*iter)->IsPhiInst() == false) {
                for (auto &&gep : gep_list) {
                    inst_list.insert(iter, gep);
                }
                break;
            }
        }
    };

    auto GepInstCombine = [&func]() -> void {
        for (auto &&node : func->GetSequentialNodes()) {
            for (auto &&inst : node->GetInstList()) {
                if (inst->IsGepInst()) {
                    auto gep = std::static_pointer_cast<GetElementPtrInst>(inst);
                    std::list<GepInstPtr> gep_list = std::list<GepInstPtr>(1, gep);
                    while (gep->GetBaseAddr()->GetParent() && gep->GetBaseAddr()->GetParent()->IsGepInst()) {
                        gep = std::static_pointer_cast<GetElementPtrInst>(gep->GetBaseAddr()->GetParent());
                        gep_list.push_front(gep);
                    }
                    if (gep_list.front()->GetBaseAddr()->IsGlobalValue() ||               // global address
                        gep_list.front()->GetBaseAddr()->GetBaseType()->IsParameter() ||  // parameter address
                        gep_list.front()->GetBaseAddr()->GetParent()->IsAllocaInst())     // alloca address
                    {
                        auto &&front = gep_list.front();
                        auto &&off_list = front->GetOffList();
                        if (off_list.size() == 2 &&
                            off_list.back() == ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))) {
                            gep_list.pop_front();
                        }
                    }
                    if (gep_list.size() > 1) {  // at least 2 gep-inst
                        for (auto &&iter = gep_list.begin(); iter != gep_list.end();) {
                            auto &&gep = (*iter);
                            auto &&off_list = gep->GetOffList();
                            if (off_list.back() == ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))) {
                                ReplaceSRC(gep->GetResult(), gep->GetBaseAddr());
                                RemoveInst(gep);
                                iter = gep_list.erase(iter);
                            } else {
                                ++iter;
                            }
                        }
                    }
                    if (gep_list.size() > 1) {  // combine logic-sequential constant offset gep-inst
                        bool all_constant = true;
                        auto &&off_set = std::list<BaseValuePtr>();
                        for (auto &&gep : gep_list) {
                            auto &&offset = gep->GetOffList().back();
                            off_set.push_back(offset);
                            all_constant = false;
                        }
                        if (all_constant) {
                            int32_t sum = 0;
                            for (auto &&constant : off_set) {
                                sum += std::get<int32_t>(std::static_pointer_cast<Constant>(constant)->GetValue());
                            }
                            auto &&offset = ConstantAllocator::FindConstantPtr(sum);

                            auto &&front = gep_list.front();
                            auto &&back = gep_list.back();
                            // new store-type
                            back->SetStoreType(front->GetStoreType());
                            // new base-addr
                            auto &&ori_base_addr = back->GetBaseAddr();
                            auto &&new_base_addr = front->GetBaseAddr();
                            ori_base_addr->RemoveUser(back);
                            new_base_addr->InsertUser(back);
                            back->SetBaseAddr(new_base_addr);
                            // new offset-list
                            auto &&off_list = OffsetList(1, offset);
                            if (front->GetOffList().size() == 2)
                                off_list.push_front(ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0)));
                            back->SetOffList(off_list);
                        } else {
                            bool can_flod = true;
                            size_t var_cnt = 0;
                            VariablePtr variable = nullptr;
                            for (auto &&off : off_set) {
                                if (off->IsConstant()) {
                                    if (off != ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))) {
                                        can_flod = false;
                                    }
                                } else if (off->IsVariable()) {
                                    var_cnt += 1;
                                    variable = std::static_pointer_cast<Variable>(off);
                                }
                            }
                            if (can_flod && var_cnt == 1) {
                                auto &&front = gep_list.front();
                                auto &&back = gep_list.back();
                                // new store-type
                                back->SetStoreType(front->GetStoreType());
                                // new base-addr
                                auto &&ori_base_addr = back->GetBaseAddr();
                                auto &&new_base_addr = front->GetBaseAddr();
                                ori_base_addr->RemoveUser(back);
                                new_base_addr->InsertUser(back);
                                back->SetBaseAddr(new_base_addr);
                                // new offset-list
                                auto &&off_list = OffsetList(1, variable);
                                if (front->GetOffList().size() == 2)
                                    off_list.push_front(ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0)));
                                back->SetOffList(off_list);
                            }
                        }
                    }
                    if (gep_list.size() == 1) {
                        auto &&gep = gep_list.front();
                        auto &&off_list = gep->GetOffList();
                        auto &&base_addr = gep->GetBaseAddr();
                        if (off_list.size() == 1 &&  // gep a exist pointer
                            (!base_addr->GetBaseType()->IsGlobal() &&
                             base_addr->GetBaseType()->IsPointer()) &&  // avoid global-scalar pointer
                            off_list.back() == ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))) {
                            ReplaceSRC(gep->GetResult(), base_addr);
                            RemoveInst(gep);
                            gep_list.clear();
                        }
                    }
                }
            }
        }
    };

    PreLoadGlbAddr();
    GepInstCombine();
    DCE::EliminateUselessCode(func);
}
