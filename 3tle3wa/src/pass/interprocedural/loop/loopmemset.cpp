#include "3tle3wa/pass/interprocedural/loop/loopmemset.hh"

#include <cassert>
#include <cstdint>
#include <memory>
#include <queue>
#include <utility>

#include "3tle3wa/backend/rl/RLGen.hh"
#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/structure/loop.hh"
#include "3tle3wa/ir/instruction/controlFlowInst.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/memoryInst.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/globalvalue.hh"
#include "3tle3wa/ir/value/type/baseType.hh"
#include "3tle3wa/ir/value/type/listType.hh"
#include "3tle3wa/pass/analysis/structure/structure.hh"

void LoopMemset::LoopMemset(NormalFuncPtr func, CompilationUnit &comp_unit) {
    BaseValuePtr array = nullptr;
    BaseValuePtr init_num = nullptr;
    auto structure = func->loops->sub_structures;
    int begin_level = 0;

    std::string memset = "llvm.memset.p0i8.i64";
    callee = comp_unit.GetFunction(memset);  // memeset

    assert(!LoopTreeTraversal(func, func->loops, array, init_num, begin_level));
}

// post-order travel loop tree
bool LoopMemset::LoopTreeTraversal(NormalFuncPtr func, Loop *loop, BaseValuePtr &array, BaseValuePtr &init_num,
                                   int &level) {
    auto &&sub_loops = loop->sub_structures;
    if (sub_loops.size() > 0) {
        for (auto &&iter = sub_loops.begin(); iter != sub_loops.end();) {
            auto &&sub_loop = static_cast<Loop *>(*iter);
            if (LoopTreeTraversal(func, sub_loop, array, init_num, level)) {
                for (auto &&node : sub_loop->GetEntireStructure()) {
                    RemoveNode(node);
                }
                std::queue<Loop *> queue;
                std::list<Loop *> list;
                queue.push(sub_loop);
                while (!queue.empty()) {
                    auto &&front = queue.front();
                    queue.pop();
                    for (auto &&sub : front->sub_structures) {
                        queue.push(static_cast<Loop *>(sub));
                    }
                }
                for (auto &&del_loop : list) {
                    StructureAnalysis::RmvLoopInNode2Loop(del_loop);
                }
                iter = sub_loops.erase(iter);
            } else {
                ++iter;
            }
        }
    } else if (sub_loops.size() == 0) {
        if (loop->before_blk == nullptr) {
            return false;
        }
        // if isArrayAssignment is true, level++
        auto array_and_init = GetArrayInit(loop);
        if (array_and_init.first == nullptr) {
            return false;
        } else {
            level++;
            array = array_and_init.first;
            init_num = array_and_init.second;
        }
    }
    if (loop->before_blk == nullptr) {
        return false;
    }

    if (array == nullptr) {
        return false;
    } else {
        // judge if loopbody equals entire subloop structure
        if (sub_loops.size() != 0) {
            if (sub_loops.size() > 1) {
                level = 0;
                array = nullptr;
                return false;
            }
            auto &&sub_loop = static_cast<Loop *>(sub_loops.front());
            std::set<CfgNodePtr> loop_body_set;
            std::set<CfgNodePtr> entire_sub_loop_set;
            for (auto &&blk : loop->GetLoopBodyBlks()) {
                loop_body_set.insert(blk);
            }
            entire_sub_loop_set.insert(sub_loop->before_blk);
            entire_sub_loop_set.insert(sub_loop->loop_exit);
            for (auto &&blk : sub_loop->GetEntireStructure()) {
                entire_sub_loop_set.insert(blk);
            }
            if (loop_body_set != entire_sub_loop_set) {
                level = 0;
                array = nullptr;
                return false;
            }
            if (sub_loop->before_blk->GetInstList().size() > 3 || sub_loop->loop_exit->GetInstList().size() > 3) {
                level = 0;
                array = nullptr;
                return false;
            }
        }

        // check if loop step is 1
        if (CheckLoopStep(loop) == false) {
            level = 0;
            array = nullptr;
            return false;
        }

        // judge if level is array dimension and if loop time equals upper of this dimension
        // true  : memset
        // false : return
        int array_dimension;
        auto &&list = std::static_pointer_cast<ListType>(array->GetBaseType());
        array_dimension = list->GetArrDims().size();
        auto loop_time = LoopUnrolling::LoopTime(loop);
        int dimension_upper = list->GetArrDims().at(array_dimension - level);
        if (dimension_upper != loop_time) {
            level = 0;
            array = nullptr;
            return false;
        }

        if (level == array_dimension) {
            loop->before_blk->RemoveLastInst();

            BaseTypePtr &&base_type = nullptr;
            if (array->IsGlobalValue()) {
                auto &&glb_value = std::static_pointer_cast<GlobalValue>(array);
                base_type = glb_value->GetInitValue()->GetBaseType();
            } else if (array->IsVariable()) {
                assert(array->GetParent()->IsAllocaInst());
                auto &&alloca_inst = std::static_pointer_cast<AllocaInst>(array->GetParent());
                base_type = alloca_inst->GetAllocaType();
            } else {
                assert(false);
            }
            assert(base_type->IsArray());
            auto &&list_type = std::static_pointer_cast<ListType>(base_type);

            BaseValuePtr start_addr = GetElementPtrInst::DoGetPointer(
                base_type, array, OffsetList(2, ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))),
                loop->before_blk);
            BaseValuePtr i8_addr = BitCastInst::DoBitCast(start_addr, loop->before_blk);

            func->InsertCallWho(callee.get());
            callee->InsertWhoCall(func.get());

            BaseValuePtr init_value = nullptr;
            if (init_num == ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))) {
                init_value = ConstantAllocator::FindConstantPtr(static_cast<char>(0));
            } else if (init_num == ConstantAllocator::FindConstantPtr(static_cast<int32_t>(-1))) {
                init_value = ConstantAllocator::FindConstantPtr(static_cast<char>(-1));
            } else if (init_num == ConstantAllocator::FindConstantPtr(static_cast<float>(0))) {
                init_value = ConstantAllocator::FindConstantPtr(static_cast<char>(0));
            } else {
                assert(false);
            }

            ParamList param_list = ParamList();
            param_list.push_back(i8_addr);
            param_list.push_back(init_value);
            param_list.push_back(ConstantAllocator::FindConstantPtr(
                static_cast<int64_t>(static_cast<int64_t>(list_type->GetCapacity() * 4))));
            param_list.push_back(ConstantAllocator::FindConstantPtr(static_cast<bool>(0)));

            auto &&call_ret_value =
                CallInst::DoCallFunction(callee->GetReturnType(), callee, param_list, loop->before_blk);

            loop->before_blk->InsertInstBack(JumpInst::CreatePtr(loop->loop_exit, loop->before_blk));

            level = 0;
            array = nullptr;
            return true;
        } else {
            level++;
            return false;
        }
        return false;
    }
}

std::pair<BaseValuePtr, BaseValuePtr> LoopMemset::GetArrayInit(Loop *loop) {
    auto &&result = std::make_pair(nullptr, nullptr);
    int store_num = 0;
    auto &&sub_loops = loop->sub_structures;
    auto &&loop_body = loop->body_begin;
    auto inst_list = loop_body->GetInstList();
    inst_list.pop_back();
    GepInstPtr gep_inst = nullptr;
    StoreInstPtr store_inst = nullptr;
    if (sub_loops.size() != 0) {  // not the deepest loop
        return result;
    }
    if (loop->IsSimpleLoop() == false) {  // not simple loop
        return result;
    }
    if (loop->GetLoopBodyBlks().size() != 1) {  // loop body size more than 1
        return result;
    }
    for (auto &&inst : loop_body->GetInstList()) {
        if (inst->IsStoreInst()) {
            store_inst = std::static_pointer_cast<StoreInst>(inst);
            store_num++;
            inst_list.remove(inst);
        }
    }
    if (store_num != 1) {  // store_num != 1
        return result;
    }
    auto &&store_addr = store_inst->GetStoreAddr();
    auto &&store_value = store_inst->GetStoreValue();
    if (store_value->IsConstant() == false) {  // store value is not constant
        return result;
    } else if (store_value != ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0)) &&
               store_value != ConstantAllocator::FindConstantPtr(static_cast<int32_t>(-1)) &&
               store_value != ConstantAllocator::FindConstantPtr(static_cast<float>(0))) {
        return result;
    }
    auto &&addr_parent = store_addr->GetParent();
    if (addr_parent->IsGepInst() == false) {  // not array
        return result;
    }
    gep_inst = std::static_pointer_cast<GetElementPtrInst>(addr_parent);
    inst_list.remove(addr_parent);
    auto &&offset_list = gep_inst->GetOffList();
    auto &&base_addr = gep_inst->GetBaseAddr();
    if (base_addr->GetBaseType()->IsArray() == false) {
        auto base_addr_parent = base_addr->GetParent();
        if (base_addr_parent->IsGepInst() == false) {
            // not array
            return result;
        }
        auto base_addr_parent_gep = std::static_pointer_cast<GetElementPtrInst>(base_addr_parent);
        base_addr = base_addr_parent_gep->GetBaseAddr();
    }
    for (auto &&offset : offset_list) {
        if (offset->IsConstant()) {
            continue;
        }
        auto &&offset_init_inst = offset->GetParent();
        for (auto &&operand : offset_init_inst->GetOprands()) {
            if (operand->IsConstant()) {
                continue;
            }
            auto operand_parent = operand->GetParent();
            bool in_body_blk = FindInst(operand_parent, loop_body);
            bool in_cond_blk = FindInst(operand_parent, loop->cond_begin);
            bool in_before_blk = FindInst(operand_parent, loop->before_blk);
            if ((in_body_blk || in_cond_blk || in_before_blk) == false) {  // defined out loop
                return result;
            }
            if (in_body_blk) {
                inst_list.remove(operand_parent);
            }
        }
        if (FindInst(offset_init_inst, loop_body)) {
            inst_list.remove(offset_init_inst);
        }
    }

    if (inst_list.size() != 0) {
        if (inst_list.size() > 1) {
            return result;
        } else {
            auto last_inst = inst_list.front();
            for (auto operand : last_inst->GetOprands()) {
                if (operand->IsConstant()) {
                    continue;
                }
                auto operand_parent = operand->GetParent();
                bool in_body_blk = FindInst(operand_parent, loop_body);
                bool in_cond_blk = FindInst(operand_parent, loop->cond_begin);
                bool in_before_blk = FindInst(operand_parent, loop->before_blk);
                if ((in_body_blk || in_cond_blk || in_before_blk) == false) {
                    return result;
                }
            }
        }
    }
    return std::make_pair(base_addr, store_value);
}

bool LoopMemset::FindInst(InstPtr inst, CfgNodePtr cfgNode) {
    auto &&inst_list = cfgNode->GetInstList();
    return (std::find(inst_list.begin(), inst_list.end(), inst) != inst_list.end());
}

bool LoopMemset::CheckLoopStep(Loop *loop) {
    int32_t change_val = 0;
    int32_t init = 0;
    auto branch = loop->cond_begin->GetInstList().back();
    if (branch->IsBranchInst() == false) {
        return false;
    }
    auto branch_inst = std::static_pointer_cast<BranchInst>(branch);
    auto bin_inst = std::static_pointer_cast<BinaryInstruction>(branch_inst->GetCondition()->GetParent());
    if (bin_inst->IsICmpInst() == false) {
        return false;
    }
    int const_check = LoopUnrolling::ConstCheck(bin_inst);
    if (const_check == 0) {
        return false;
    }
    auto operands_of_cmp_vector = LoopUnrolling::InstOperandsInVector(bin_inst);
    auto key = operands_of_cmp_vector.at(2 - const_check);
    auto cmp_value = operands_of_cmp_vector.at(const_check - 1);
    if (cmp_value->IsConstant() == false || cmp_value->GetBaseType()->IntType() == false) {
        return false;
    }
    auto key_parent = key->GetParent();
    if (key_parent == nullptr || key_parent->IsPhiInst() == false) {
        return false;
    }

    auto phi_inst = std::static_pointer_cast<PhiInst>(key_parent);
    auto loop_blks = loop->GetLoopBodyBlks();

    // start to get init value of key
    auto data_list = phi_inst->GetDataList();
    auto data_source1 = data_list.front();
    auto data_source2 = *std::next(data_list.begin(), 1);
    auto source_value1 = data_source1.first;
    auto source_blk1 = data_source1.second;
    auto source_value2 = data_source2.first;
    auto source_blk2 = data_source2.second;

    InstPtr phi_part_defined_in_loop;
    BaseValuePtr value_defined_in_loop;
    if (std::find(loop_blks.begin(), loop_blks.end(), source_blk1) != loop_blks.end()) {
        value_defined_in_loop = source_value1;
        phi_part_defined_in_loop = source_value1->GetParent();
        if (source_value2->IsConstant() == false) {
            return false;
        }
        auto &&const_init_val = std::static_pointer_cast<Constant>(source_value2);
        if (!std::holds_alternative<int32_t>(const_init_val->GetValue()) &&
            !std::holds_alternative<int64_t>(const_init_val->GetValue())) {
            return false;
        }
        init = std::get<int32_t>(const_init_val->GetValue());
    } else if (std::find(loop_blks.begin(), loop_blks.end(), source_blk2) != loop_blks.end()) {
        value_defined_in_loop = source_value2;
        phi_part_defined_in_loop = source_value2->GetParent();
        if (source_value1->IsConstant() == false) {
            return false;
        }
        auto const_init_val = std::static_pointer_cast<Constant>(source_value1);
        if (!std::holds_alternative<int32_t>(const_init_val->GetValue()) &&
            !std::holds_alternative<int64_t>(const_init_val->GetValue())) {
            return false;
        }
        init = std::get<int32_t>(const_init_val->GetValue());
    } else {
        return false;
    }

    if (init != 0) {  // not init from 0
        return false;
    }

    while (value_defined_in_loop != key) {
        if (phi_part_defined_in_loop == nullptr) {
            return false;
        }
        if (phi_part_defined_in_loop->GetOpCode() == OP_ADD) {
            auto operands = LoopUnrolling::InstOperandsInVector(phi_part_defined_in_loop);
            const_check = LoopUnrolling::ConstCheck(phi_part_defined_in_loop);
            auto const_val_in_phi = operands.at(const_check - 1);
            if (const_val_in_phi->IsConstant() == false) {
                return false;
            }
            auto constant_in_phi = std::static_pointer_cast<Constant>(const_val_in_phi)->GetValue();
            if (!std::holds_alternative<int32_t>(constant_in_phi) &&
                !std::holds_alternative<int64_t>(constant_in_phi)) {
                // change value of key is not int
                return false;
            }
            change_val += std::get<int32_t>(constant_in_phi);
            value_defined_in_loop = operands.at(2 - const_check);
        } else if (phi_part_defined_in_loop->GetOpCode() == OP_SUB) {
            auto operands = LoopUnrolling::InstOperandsInVector(phi_part_defined_in_loop);
            const_check = LoopUnrolling::ConstCheck(phi_part_defined_in_loop);
            auto const_val_in_phi = operands.at(const_check - 1);
            if (const_val_in_phi->IsConstant() == false) {
                return false;
            }
            auto constant_in_phi = std::static_pointer_cast<Constant>(const_val_in_phi)->GetValue();
            if (!std::holds_alternative<int32_t>(constant_in_phi) &&
                !std::holds_alternative<int64_t>(constant_in_phi)) {
                // change value is not int
                return false;
            }
            change_val -= std::get<int32_t>(constant_in_phi);
            value_defined_in_loop = operands.at(2 - const_check);
        } else {
            return false;
        }
    }
    if (change_val != 1) {
        return false;
    }
    return true;
}
