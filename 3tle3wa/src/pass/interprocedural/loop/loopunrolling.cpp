#include "3tle3wa/pass/interprocedural/loop/loopunrolling.hh"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <utility>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/structure/loop.hh"
#include "3tle3wa/ir/instruction/binaryOpInst.hh"
#include "3tle3wa/ir/instruction/controlFlowInst.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"

size_t cur_unroll_time = 0;

void LoopUnrolling::LoopUnrolling(NormalFuncPtr func) {
    auto &&sub_structures = func->loops->sub_structures;
    for (auto &&iter = sub_structures.begin(); iter != sub_structures.end();) {
        cur_unroll_time = 0;  //  for each loop in function
        auto &&sub_loop = static_cast<Loop *>(*iter);
        if (ExpandLoop(func, sub_loop)) {
            StructureAnalysis::RmvLoopInNode2Loop(sub_loop);
            iter = sub_structures.erase(iter);
        } else {
            ++iter;
        }
    }
    return;
}

bool LoopUnrolling::ExpandLoop(NormalFuncPtr func, Loop *loop) {
    auto &&sub_loops = loop->sub_structures;
    for (auto &&iter = sub_loops.begin(); iter != sub_loops.end();) {
        auto &&sub_loop = static_cast<Loop *>(*iter);
        if (ExpandLoop(func, sub_loop)) {
            StructureAnalysis::RmvLoopInNode2Loop(sub_loop);
            iter = sub_loops.erase(iter);
            cur_unroll_time += 1;
        } else {
            ++iter;
        }
        if (cur_unroll_time > MAX_UNROLL_TIME) {
            return false;
        }
    }
    if (loop->before_blk) {
        // cout << loop->IsSimpleLoop() << ' ' << loop_blks.size() << ' ' << LoopTime(loop) << endl;
        if (loop->IsSimpleLoop() == false) {
            return false;
        }
        auto loop_cond = loop->GetCondBodyBlks();
        auto loop_blks = loop->GetLoopBodyBlks();
        if (loop_blks.size() > 1) {
            return false;
        }
        if (loop_blks.front()->GetInstCnt() > 10) {
            return false;
        }
        if (loop->sub_structures.size() != 0) {
            return false;
        }
        auto loop_time = LoopTime(loop);
        if (loop_time == 0 || loop_time > 100) {
            return false;
        }
        FullyExpand(func, loop_time, loop);
        // FullyExpand_Multi_Blks(func, loop_time, loop);
        return true;
    }
    assert(loop->parent == nullptr);
    return false;
}

void LoopUnrolling::FullyExpand(NormalFuncPtr func, int loop_time, Loop *loop) {
    auto &&before_blk = loop->before_blk;
    auto loop_blks = loop->GetEntireStructure();
    auto cond_begin = loop->cond_begin;
    auto loop_body = loop_blks.back();
    auto loop_exit = loop->loop_exit;

    // remove loop branch inst
    auto &&entry_terminator = before_blk->GetInstList().back();
    if (entry_terminator->IsJumpInst() == false) {
        return;
    }
    auto &&body_terminator = loop_body->GetInstList().back();
    if (body_terminator->IsJumpInst() == false) {
        return;
    }
    before_blk->RemoveInst(entry_terminator);
    loop_body->RemoveInst(body_terminator);

    // START
    // Record the mapping of the result of the phi instruction to its sources (from outside the loop
    // or within the loop) for updating instructions using the result during the loop iteration
    phi_results.clear();
    phi_source_defined_in_loop.clear();
    phi_source_defined_out_loop.clear();
    phi_source_updated.clear();
    old_to_new.clear();

    for (auto &&inst : cond_begin->GetInstList()) {
        if (inst->IsPhiInst()) {
            auto &&result = inst->GetResult();
            auto &&phi_inst = std::static_pointer_cast<PhiInst>(inst);
            auto &&data_list = phi_inst->GetDataList();
            auto &&data_source1 = data_list.front();
            auto &&data_source2 = *std::next(data_list.begin(), 1);
            auto &&source_value1 = data_source1.first;
            auto &&source_blk1 = data_source1.second;
            auto &&source_value2 = data_source2.first;
            auto &&source_blk2 = data_source2.second;

            phi_results.insert(result);
            if (std::find(loop_blks.begin(), loop_blks.end(), source_blk1) != loop_blks.end()) {
                phi_source_defined_in_loop[result] = source_value1;
                phi_source_defined_out_loop[result] = source_value2;
                loop_variants.insert(source_value1);
            } else if (std::find(loop_blks.begin(), loop_blks.end(), source_blk2) != loop_blks.end()) {
                phi_source_defined_in_loop[result] = source_value2;
                phi_source_defined_out_loop[result] = source_value1;
                loop_variants.insert(source_value2);
            } else {
                continue;
            }
        }
    }
    // ENDS

    // copy insts
    bool init_flag = true;
    for (int i = 0; i < loop_time; ++i) {
        for (auto &&inst : loop_body->GetInstList()) {
            if (inst->IsReturnInst() || inst->IsPhiInst()) {
                return;
            }
            if (inst->IsStoreInst() == false) {
                auto new_value = InstCopy(inst, before_blk, init_flag);
                if (new_value == nullptr) {
                    continue;
                }
                old_to_new[inst->GetResult()] = new_value;
            } else {
                auto &&binary_inst_ = std::static_pointer_cast<BinaryInstruction>(inst);
                auto &&lhs = binary_inst_->GetLHS();
                auto &&rhs = binary_inst_->GetRHS();
                lhs = OperandUpdate(lhs, init_flag);
                rhs = OperandUpdate(rhs, init_flag);
                StoreInst::DoStoreValue(lhs, rhs, before_blk);
            }
        }
        init_flag = false;
        for (auto pair : old_to_new) {
            phi_source_updated[pair.first] = old_to_new[pair.second];
        }
    }

    // remove all loop blocks
    for (auto &&blk : loop_blks) {
        RemoveNode(blk);
    }
    // loop is expanded, expanded-inst is inserted into before-blk
    auto &&i_inst_list = before_blk->GetInstList();
    auto &&j_inst_list = loop_exit->GetInstList();
    // because sub-loop is expanded, before-blk and loop-exit can combine
    std::for_each(i_inst_list.begin(), i_inst_list.end(),
                  [&loop_exit](const auto &inst) { inst->SetParent(loop_exit); });
    i_inst_list.insert(i_inst_list.end(), j_inst_list.begin(), j_inst_list.end());
    j_inst_list = std::move(i_inst_list);
    // combine two block-attr
    loop_exit->blk_attr.blk_type |= before_blk->blk_attr.blk_type;

    assert(before_blk->blk_attr.before_blk && loop_exit->blk_attr.structure_out);
    assert(!before_blk->blk_attr.cond_begin && !before_blk->blk_attr.cond_end);
    assert(!loop_exit->blk_attr.cond_begin && !loop_exit->blk_attr.cond_end && !loop_exit->blk_attr.body_begin);

    if (!before_blk->blk_attr.structure_out && !loop_exit->blk_attr.before_blk) {
        loop_exit->blk_attr.before_blk = false;
        loop_exit->blk_attr.structure_out = false;
    } else if (!before_blk->blk_attr.structure_out && loop_exit->blk_attr.before_blk) {
        loop_exit->blk_attr.before_blk = true;
        loop_exit->blk_attr.structure_out = false;
    } else if (before_blk->blk_attr.structure_out && !loop_exit->blk_attr.before_blk) {
        loop_exit->blk_attr.before_blk = true;
        loop_exit->blk_attr.structure_out = false;
    } else if (before_blk->blk_attr.structure_out && loop_exit->blk_attr.before_blk) {
        loop_exit->blk_attr.before_blk = true;
        loop_exit->blk_attr.structure_out = true;
    } else {
        assert(false);
    }

    // fix blk-attr
    if (loop_exit->blk_attr.ChkAllOfBlkType(BlkAttr::GoReturn, BlkAttr::Exit)) {
        loop_exit->blk_attr.ClrBlkTypes(BlkAttr::GoReturn);
    }
    if (loop_exit->blk_attr.ChkOneOfBlkType(BlkAttr::Entry)) {
        func->SetEntryNode(loop_exit);
    }

    // adjust predecessors and successors
    for (auto &&pred : before_blk->GetPredecessors()) {
        pred->AddSuccessor(loop_exit);
        loop_exit->AddPredecessor(pred);
        pred->GetLastInst()->ReplaceTarget(before_blk, loop_exit);
    }

    // remove before-blk
    RemoveNode(before_blk);
    // loop-exit is original jump to parent-loop-cond-begin
    // no need to insert new jump-inst
    for (auto &&invariant : phi_results) {
        auto defined_in_loop = phi_source_defined_in_loop[invariant];
        auto replacer = old_to_new[defined_in_loop];
        ReplaceSRC(invariant, replacer);
    }

    if (loop->parent != func->loops) {
        auto &&parent = static_cast<Loop *>(loop->parent);
        if (parent->before_blk != nullptr && parent->body_begin == before_blk) {
            parent->body_begin = loop_exit;
            loop_exit->blk_attr.body_begin = true;
        }
    }
    // adjust phi-origin-alloca-parent
    for (auto &&node : func->GetSequentialNodes()) {
        for (auto &&inst : node->GetInstList()) {
            if (inst->IsPhiInst()) {
                auto &&phi_inst = std::static_pointer_cast<PhiInst>(inst);
                auto &&ori_alloca = phi_inst->GetOriginAlloca();
                if (ori_alloca->GetParent() == before_blk) {
                    ori_alloca->SetParent(loop_exit);
                }
            }
        }
    }
    return;
}

int LoopUnrolling::LoopTime(Loop *loop) {
    int32_t time = 0;
    auto loop_blks = loop->GetLoopBodyBlks();
    auto cond_begin = loop->cond_begin;
    auto loop_end = loop_blks.back();
    if (cond_begin->GetPredecessors().size() > 2) {
        return 0;
    }
    if (loop_end->GetSuccessors().size() > 1) {
        return 0;
    }

    auto cond_end = loop->cond_end;
    auto last_inst = cond_end->GetInstList().back();
    if (last_inst->IsBranchInst() && last_inst->GetOprands().size() != 0) {
        auto &&last_inst_oprands = last_inst->GetOprands();
        auto &&end_value = last_inst_oprands.front();
        auto &&cmp_inst = end_value->GetParent();
        if (cmp_inst == nullptr) {
            // cmp_inst is nullptr
            return 0;
        }
        auto opcode = cmp_inst->GetOpCode();
        if (opcode < OP_LTH) {
            // Opcode is not cmp
            return 0;
        }

        int32_t limit;
        int32_t init;
        int32_t change_val = 0;
        int const_check = ConstCheck(cmp_inst);
        if (const_check == 0) {
            // both cmp operands are not constant
            return 0;
        }

        auto operands_of_cmp_vector = InstOperandsInVector(cmp_inst);

        // key:obtain key of the very compare instruction
        // limit:obtain loop boundary value,restricted to type int
        auto key = operands_of_cmp_vector.at(2 - const_check);
        auto &&cmp_value = operands_of_cmp_vector.at(const_check - 1);
        if (cmp_value->IsConstant() == false || cmp_value->GetBaseType()->IntType() == false) {
            // cmp_value is not int
            return 0;
        }
        auto &&constant_value = std::static_pointer_cast<Constant>(cmp_value);
        auto const_val = constant_value->GetValue();
        if (!std::holds_alternative<int32_t>(const_val) && !std::holds_alternative<int64_t>(const_val)) {
            // when get compare limit, type is not int32_t or int64_t
            return 0;
        }
        limit = std::get<int32_t>(const_val);

        // Change the form of the comparison instruction to
        // "VARIANT OPCODE VALUE"
        // VARIANT:variant key in LoopTime function
        // VALUE:variant limit in LoopTime function
        if (const_check == 1) {
            opcode = FlipComparisonOp(opcode);
        }

        auto key_parent = key->GetParent();
        if (key_parent == nullptr) {
            return 0;
        }
        if (key_parent->IsPhiInst() == false) {
            // compare key is not from phi inst
            return 0;
        }

        auto &&phi_inst = std::static_pointer_cast<PhiInst>(key_parent);

        // start to get init value of key
        auto &&data_list = phi_inst->GetDataList();
        auto &&data_source1 = data_list.front();
        auto &&data_source2 = *std::next(data_list.begin(), 1);
        auto &&source_value1 = data_source1.first;
        auto &&source_blk1 = data_source1.second;
        auto &&source_value2 = data_source2.first;
        auto &&source_blk2 = data_source2.second;

        InstPtr phi_part_defined_in_loop;
        BaseValuePtr value_defined_in_loop;
        if (std::find(loop_blks.begin(), loop_blks.end(), source_blk1) != loop_blks.end()) {
            value_defined_in_loop = source_value1;
            phi_part_defined_in_loop = source_value1->GetParent();
            auto const_init_val = std::static_pointer_cast<Constant>(source_value2);
            if (const_init_val == nullptr) {
                return 0;
            }
            if (!std::holds_alternative<int32_t>(const_init_val->GetValue()) &&
                !std::holds_alternative<int64_t>(const_init_val->GetValue())) {
                // init of key is not int
                return 0;
            }
            init = std::get<int32_t>(const_init_val->GetValue());
        } else if (std::find(loop_blks.begin(), loop_blks.end(), source_blk2) != loop_blks.end()) {
            value_defined_in_loop = source_value2;
            phi_part_defined_in_loop = source_value2->GetParent();
            auto const_init_val = std::static_pointer_cast<Constant>(source_value1);
            if (const_init_val == nullptr) {
                return 0;
            }
            if (!std::holds_alternative<int32_t>(const_init_val->GetValue()) &&
                !std::holds_alternative<int64_t>(const_init_val->GetValue())) {
                // init of key is not int
                return 0;
            }
            init = std::get<int32_t>(const_init_val->GetValue());
        } else {
            // both phi oprands are not from the loop
            return 0;
        }
        // end of finding init value of key

        // obtain the change in value of the key during loop iteration
        while (value_defined_in_loop != key) {
            if (phi_part_defined_in_loop == nullptr) {
                return 0;
            }
            if (phi_part_defined_in_loop->GetOpCode() == OP_ADD) {
                auto operands = InstOperandsInVector(phi_part_defined_in_loop);
                const_check = ConstCheck(phi_part_defined_in_loop);
                auto const_val_in_phi = operands.at(const_check - 1);
                if (const_val_in_phi->IsConstant() == false) {
                    return 0;
                }
                auto constant_in_phi = std::static_pointer_cast<Constant>(const_val_in_phi)->GetValue();
                if (!std::holds_alternative<int32_t>(constant_in_phi) &&
                    !std::holds_alternative<int64_t>(constant_in_phi)) {
                    // change value of key is not int
                    return 0;
                }
                change_val += std::get<int32_t>(constant_in_phi);
                value_defined_in_loop = operands.at(2 - const_check);
            } else if (phi_part_defined_in_loop->GetOpCode() == OP_SUB) {
                auto operands = InstOperandsInVector(phi_part_defined_in_loop);
                const_check = ConstCheck(phi_part_defined_in_loop);
                auto const_val_in_phi = operands.at(const_check - 1);
                if (const_val_in_phi->IsConstant() == false) {
                    return 0;
                }
                auto constant_in_phi = std::static_pointer_cast<Constant>(const_val_in_phi)->GetValue();
                if (!std::holds_alternative<int32_t>(constant_in_phi) &&
                    !std::holds_alternative<int64_t>(constant_in_phi)) {
                    // change value is not int
                    return 0;
                }
                change_val -= std::get<int32_t>(constant_in_phi);
                value_defined_in_loop = operands.at(2 - const_check);
            } else {
                return 0;
            }
        }
        // obtain the change in value of the key during loop iteration ENDs

        int32_t gap = init - limit;
        switch (opcode) {
            case OP_GEQ:
                if (gap < 0) {
                    return -1;
                } else {
                    if (change_val >= 0) {
                        return 0;
                    }
                    while (gap >= 0) {
                        gap += change_val;
                        time++;
                    }
                    return time;
                }
                break;
            case OP_LEQ:
                if (gap > 0) {
                    return -1;
                } else {
                    if (change_val <= 0) {
                        return 0;
                    }
                    while (gap <= 0) {
                        gap += change_val;
                        time++;
                    }
                    return time;
                }
                break;
            case OP_GTH:
                if (gap <= 0) {
                    return -1;
                } else {
                    if (change_val > 0) {
                        return 0;
                    }
                    while (gap > 0) {
                        gap += change_val;
                        time++;
                    }
                    return time;
                }
                break;
            case OP_LTH:
                if (gap >= 0) {
                    return -1;
                } else {
                    if (change_val < 0) {
                        return 0;
                    }
                    while (gap < 0) {
                        gap += change_val;
                        time++;
                    }
                    return time;
                }
                break;
            default:
                return 0;
                break;
        }
    } else {
        return 0;
    }
    return 0;
}

int LoopUnrolling::ConstCheck(InstPtr inst) {
    // Determine if the operands of the specific comparison instruction are constants.
    // Returns:
    // - 0 if neither of the operands is a constant
    // - 1 if the left operand is a constant
    // - 2 if the right operand is a constant
    // It's unlikely for both operands to be constants.
    if (inst->IsTwoOprandInst() == false) {
        throw("not twoOperand instruction");
    }
    auto &&inst_oprands = inst->GetOprands();
    auto lhs = *inst_oprands.begin();
    auto rhs = *std::next(inst_oprands.begin(), 1);
    if (lhs->IsConstant() && rhs->IsConstant() == false) {
        return 1;
    } else if (rhs->IsConstant() && lhs->IsConstant() == false) {
        return 2;
    }
    return 0;
}

OpCode LoopUnrolling::FlipComparisonOp(OpCode opcode) {
    switch (opcode) {
        case OpCode::OP_LTH:
            return OpCode::OP_GTH;
        case OpCode::OP_LEQ:
            return OpCode::OP_GEQ;
        case OpCode::OP_GTH:
            return OpCode::OP_LTH;
        case OpCode::OP_GEQ:
            return OpCode::OP_LEQ;
        default:
            return opcode;
    }
}

Operands LoopUnrolling::InstOperandsInVector(InstPtr inst) {
    Operands operands_in_vector;
    for (auto &&operand : inst->GetOprands()) {
        operands_in_vector.push_back(operand);
    }
    return operands_in_vector;
}

BaseValuePtr LoopUnrolling::InstCopy(InstPtr &inst_, CfgNodePtr &parent, bool init_flag) {
    assert(!(inst_->IsReturnInst() || inst_->IsPhiInst()));
    BaseValuePtr result = nullptr;
    OpCode opcode = inst_->GetOpCode();
    if (inst_->IsOneOprandInst()) {
        auto &&unary_inst_ = std::static_pointer_cast<UnaryInstruction>(inst_);
        auto &&oprand = unary_inst_->GetOprand();
        oprand = OperandUpdate(oprand, init_flag);
        if (opcode == Load) {
            result = LoadInst::DoLoadValue(oprand, parent);
        } else if (opcode == BitCast) {
            result = BitCastInst::DoBitCast(oprand, parent);
        } else if (opcode == SiToFp) {
            result = SitoFpInst::DoSitoFp(oprand, parent);
        } else if (opcode == FpToSi) {
            auto type = unary_inst_->GetResult()->GetBaseType()->GetAttrType();
            result = FptoSiInst::DoFptoSi(type, oprand, parent);
        } else if (opcode == Zext) {
            result = ZextInst::DoZeroExt(oprand, parent);
        } else if (opcode == FNeg) {
            result = FNegInst::DoFloatNeg(oprand, parent);
        } else {
            assert(false);
        }
    } else if (inst_->IsTwoOprandInst()) {
        auto &&binary_inst_ = std::static_pointer_cast<BinaryInstruction>(inst_);
        auto &&lhs = binary_inst_->GetLHS();
        auto &&rhs = binary_inst_->GetRHS();
        lhs = OperandUpdate(lhs, init_flag);
        rhs = OperandUpdate(rhs, init_flag);
        if (binary_inst_->IsIBinaryInst()) {
            result = IBinaryInst::DoIBinOperate(opcode, lhs, rhs, parent);
        } else if (binary_inst_->IsFBinaryInst()) {
            result = FBinaryInst::DoFBinOperate(opcode, lhs, rhs, parent);
        } else if (binary_inst_->IsICmpInst()) {
            result = ICmpInst::DoICompare(opcode, lhs, rhs, parent);
        } else if (binary_inst_->IsFCmpInst()) {
            result = FCmpInst::DoFCompare(opcode, lhs, rhs, parent);
        } else {
            assert(false);
        }
    } else if (inst_->IsGepInst()) {
        auto &&gep_inst_ = std::static_pointer_cast<GetElementPtrInst>(inst_);
        OffsetList off_list;
        for (auto &&off : gep_inst_->GetOffList()) {
            auto result = OperandUpdate(off, init_flag);
            off_list.push_back(result);
        }
        auto new_addr = OperandUpdate(gep_inst_->GetBaseAddr(), init_flag);
        result = GetElementPtrInst::DoGetPointer(gep_inst_->GetStoreType(), new_addr, off_list, parent);
    } else if (inst_->IsCallInst()) {
        auto &&call_inst_ = std::static_pointer_cast<CallInst>(inst_);
        ParamList param_list;
        for (auto &&param : call_inst_->GetParamList()) {
            auto result = OperandUpdate(param, init_flag);
            param_list.push_back(result);
        }
        result = CallInst::DoCallFunction(call_inst_->GetRetType(), call_inst_->GetCalleeFunc(), param_list, parent);
    }
    assert((result != nullptr) || (inst_->IsCallInst()));
    return result;
}

BaseValuePtr LoopUnrolling::OperandUpdate(BaseValuePtr operand, bool init_flag) {
    BaseValuePtr result = nullptr;
    if (phi_results.find(operand) == phi_results.end()) {
        if (old_to_new[operand] == nullptr) {
            return operand;
        } else {
            return old_to_new[operand];
        }
    }
    if (init_flag) {
        if (phi_source_defined_out_loop[operand] != nullptr) {
            result = phi_source_defined_out_loop[operand];
        }
    }
    auto old_value = phi_source_defined_in_loop[operand];
    if (old_value != nullptr) {
        if (old_to_new[old_value] != nullptr) {
            result = old_to_new[old_value];
        }
    }
    old_to_new[operand] = result;
    return result;
}

CfgNodePtr LoopUnrolling::CfgNodeUpdate(const CfgNodePtr cfgnode) {
    if (block_mapping[cfgnode] == nullptr) {
        return cfgnode;
    }
    CfgNodePtr result = nullptr;
    result = block_mapping[cfgnode];
    return result;
}

// TODO:jmp from before_blk to new_entry, and remove the original loop body blocks and cond block
void LoopUnrolling::FullyExpand_Multi_Blks(NormalFuncPtr func, int loop_time, Loop *loop) {
    if (loop_time == -1) {
        return;
    }

    auto &&before_blk = loop->before_blk;
    auto &&loop_blks = loop->GetLoopBodyBlks();
    auto &&loop_begin = loop->body_begin;
    auto &&cond_begin = loop->cond_begin;
    auto &&loop_end = loop_blks.back();
    auto &&loop_exit = loop->loop_exit;

    // remove loop branch inst
    auto &&entry_terminator = before_blk->GetInstList().back();
    if (entry_terminator->IsJumpInst() == false) {
        return;
    }
    auto &&body_terminator = loop_end->GetInstList().back();
    if (body_terminator->IsJumpInst() == false) {
        return;
    }
    before_blk->RemoveInst(entry_terminator);

    // START
    // Record the mapping of the result of the phi instruction to its sources (from outside the loop
    // or within the loop) for updating instructions using the result during the loop iteration
    phi_results.clear();
    phi_source_defined_in_loop.clear();
    phi_source_defined_out_loop.clear();
    phi_source_updated.clear();
    old_to_new.clear();
    block_mapping.clear();
    phi_to_update.clear();

    for (auto &&inst : cond_begin->GetInstList()) {
        if (inst->IsPhiInst()) {
            auto &&result = inst->GetResult();
            auto &&phi_inst = std::static_pointer_cast<PhiInst>(inst);
            auto &&data_list = phi_inst->GetDataList();
            auto &&data_source1 = data_list.front();
            auto &&data_source2 = *std::next(data_list.begin(), 1);
            auto &&source_value1 = data_source1.first;
            auto &&source_blk1 = data_source1.second;
            auto &&source_value2 = data_source2.first;
            auto &&source_blk2 = data_source2.second;

            phi_results.insert(result);
            if (std::find(loop_blks.begin(), loop_blks.end(), source_blk1) != loop_blks.end()) {
                phi_source_defined_in_loop[result] = source_value1;
                phi_source_defined_out_loop[result] = source_value2;
                loop_variants.insert(source_value1);
            } else if (std::find(loop_blks.begin(), loop_blks.end(), source_blk2) != loop_blks.end()) {
                phi_source_defined_in_loop[result] = source_value2;
                phi_source_defined_out_loop[result] = source_value1;
                loop_variants.insert(source_value2);
            } else {
                continue;
            }
        }
    }
    // ENDS

    // copy insts
    bool init_flag = true;
    CfgNodePtr new_entry = nullptr;
    for (int i = 0; i < loop_time; ++i) {
        for (auto &&body_blk : loop_blks) {
            if (body_blk == cond_begin) {
                continue;
            }
            auto new_blk = CtrlFlowGraphNode::CreatePtr(body_blk->GetParent(), body_blk->blk_attr.blk_type);

            // get the beginning block of the new iteration and replace the loop_exit block of the previous iteration
            // with this block
            if (body_blk == loop_begin) {
                if (init_flag == false) {
                    block_mapping[cond_begin] = new_blk;
                    AddJmpInst(cond_begin);
                    AddBranchInst(cond_begin, init_flag);
                    // AddPhiInst();
                } else {
                    new_entry = new_blk;  // update new body entry
                }
            }
            for (auto pair : old_to_new) {
                phi_source_updated[pair.first] = pair.second;
            }

            for (auto &&inst : body_blk->GetInstList()) {
                if (inst->IsReturnInst()) {
                    return;
                }
                if (inst->IsJumpInst() || inst->IsBranchInst()) {
                    continue;
                }
                if (inst->IsStoreInst() == false && inst->IsPhiInst() == false) {
                    auto new_value = InstCopy(inst, new_blk, init_flag);
                    if (new_value == nullptr) {
                        continue;
                    }
                    old_to_new[inst->GetResult()] = new_value;
                } else if (inst->IsStoreInst() && inst->IsPhiInst() == false) {
                    auto &&binary_inst_ = std::static_pointer_cast<BinaryInstruction>(inst);
                    auto &&lhs = binary_inst_->GetLHS();
                    auto &&rhs = binary_inst_->GetRHS();
                    lhs = OperandUpdate(lhs, init_flag);
                    rhs = OperandUpdate(rhs, init_flag);
                    StoreInst::DoStoreValue(lhs, rhs, new_blk);
                } else {
                    auto old_phi_inst = std::static_pointer_cast<PhiInst>(inst);
                    auto new_phi_inst = PhiInst::CreatePtr(old_phi_inst->GetResult()->GetBaseType(), new_blk);
                    new_phi_inst->SetOriginAlloca(old_phi_inst->GetOriginAlloca());
                    for (auto &&val_blk : old_phi_inst->GetDataList()) {
                        auto &&old_val = val_blk.first;
                        auto &&old_block = val_blk.second;
                        auto new_val = OperandUpdate(old_val, init_flag);
                        auto new_block = CfgNodeUpdate(old_block);
                        new_phi_inst->InsertPhiData(new_phi_inst, new_val, new_block);
                    }
                    old_to_new[inst->GetResult()] = new_phi_inst->GetResult();
                }
            }
            block_mapping[body_blk] = new_blk;
        }
        init_flag = false;
        for (auto pair : old_to_new) {
            phi_source_updated[pair.first] = pair.second;
        }
    }

    // after the last iteration is completed, update the jump instruction for the blocks of the final iteration
    block_mapping[cond_begin] = loop_exit;
    AddJmpInst(cond_begin);
    AddBranchInst(cond_begin, init_flag);
    // AddPhiInst();

    // remove all loop blocks
    for (auto &&blk : loop_blks) {
        RemoveNode(blk);
    }
    RemoveNode(cond_begin);

    before_blk->RmvSuccessor(cond_begin);
    loop_exit->RmvPredecessor(cond_begin);
    JumpInstPtr to_new_entry = JumpInst::CreatePtr(new_entry, before_blk);
    before_blk->InsertInstBack(to_new_entry);
    for (auto &&invariant : phi_results) {
        auto defined_in_loop = phi_source_defined_in_loop[invariant];
        auto replacer = old_to_new[defined_in_loop];
        ReplaceSRC(invariant, replacer);
    }
}

void LoopUnrolling::AddJmpInst(CfgNodePtr &cond_begin) {
    for (auto &pair : block_mapping) {
        auto &new_one = pair.second;
        auto &old_one = pair.first;
        if (old_one == cond_begin) {
            continue;
        }
        auto &old_jmp = old_one->GetInstList().back();
        if (old_jmp->IsJumpInst() == false) {
            continue;
        }
        auto old_jmp_inst = std::static_pointer_cast<JumpInst>(old_jmp);
        auto old_target = old_jmp_inst->GetTarget();
        auto new_jmp_inst = JumpInst::CreatePtr(block_mapping[old_target], new_one);
        new_one->InsertInstBack(new_jmp_inst);
    }
}

void LoopUnrolling::AddBranchInst(CfgNodePtr &cond_begin, bool init_flag) {
    for (auto &pair : block_mapping) {
        auto &old_block = pair.first;
        auto &new_block = pair.second;
        if (old_block == cond_begin) {
            continue;
        }
        auto &last_inst = old_block->GetInstList().back();
        if (last_inst->IsBranchInst() == false) {
            continue;
        }
        auto old_br = std::static_pointer_cast<BranchInst>(last_inst);
        auto old_cond = old_br->GetCondition();
        auto if_false = old_br->GetFalseTarget();
        auto if_true = old_br->GetTrueTarget();
        auto new_cond = OperandUpdate(old_cond, init_flag);
        auto new_if_false = CfgNodeUpdate(if_false);
        auto new_if_true = CfgNodeUpdate(if_true);
        auto new_br = BranchInst::CreatePtr(new_cond, new_if_true, new_if_false, new_block);
        new_block->InsertInstBack(new_br);
    }
}

void LoopUnrolling::DynamicUnrolling(NormalFuncPtr &func) {
    auto &&seq_nodes = func->GetSequentialNodes();
    if (seq_nodes.size() != 4) return;

    auto &&entry = func->GetEntryNode();
    auto &&exit = func->GetExitNode();

    if (entry->GetSuccessors().size() > 1 || exit->GetPredecessors().size() > 1) return;
    // means this func is a simple loop
    auto &&loop_cond = (*entry->GetSuccessors().begin());
    assert(loop_cond->GetPredecessors().size() == 2 && loop_cond->GetSuccessors().size() == 2);
    auto &&loop_body = (*loop_cond->GetSuccessors().begin() != exit) ? *loop_cond->GetSuccessors().begin()
                                                                     : *(--loop_cond->GetSuccessors().end());
    if (loop_body->GetInstCnt() > 4) return;

    auto &&br = std::static_pointer_cast<BranchInst>(loop_cond->GetLastInst());
    auto &&cmp = std::static_pointer_cast<BinaryInstruction>(br->GetCondition()->GetParent());
    auto &&lhs = cmp->GetLHS();
    auto &&rhs = cmp->GetRHS();

    if (lhs->GetParent() && lhs->GetParent()->IsPhiInst()) {
        BaseValuePtr loop_time = rhs;
        if (rhs->IsConstant()) return;

        auto &&phi = std::static_pointer_cast<PhiInst>(lhs->GetParent());  // conut
        if (phi->GetDataList().size() != 2) return;

        auto &&data_list = phi->GetDataList();
        auto &&init = (data_list.front().second == entry) ? data_list.front().first : data_list.back().first;
        if (init != ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))) return;

        auto &&counter = (data_list.front().second == entry) ? data_list.back().first : data_list.front().first;
        auto &&count_definer = counter->GetParent();
        if (!(count_definer->GetOpCode() == OP_ADD || count_definer->GetOpCode() == OP_SUB)) return;

        auto &&bin = std::static_pointer_cast<BinaryInstruction>(count_definer);
        auto step = (bin->GetLHS() == phi->GetResult()) ? bin->GetRHS() : bin->GetLHS();
        if (step != ConstantAllocator::FindConstantPtr(static_cast<int32_t>(1))) return;

        for (auto &&inst : loop_body->GetInstList()) {
            if (inst == count_definer) continue;
            if (inst->IsTwoOprandInst()) {
                auto &&bin = std::static_pointer_cast<BinaryInstruction>(inst);
                auto &&lhs = bin->GetLHS();
                auto &&rhs = bin->GetRHS();
                if (rhs->IsVariable()) continue;

                auto &&lhs_definer = lhs->GetParent();
                if (lhs_definer->IsPhiInst() == false) return;
                auto &&phi = std::static_pointer_cast<PhiInst>(lhs_definer);
                auto &&data_list = phi->GetDataList();
                auto &&init = (data_list.front().second == entry) ? data_list.front().first : data_list.back().first;
                if (init != ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))) return;

                auto &&user = (*bin->GetResult()->GetUserList().begin());
                if (user->IsTwoOprandInst() == false) return;

                entry->RemoveLastInst();

                if (rhs->GetBaseType()->IntType()) {
                    auto &&bin = std::static_pointer_cast<BinaryInstruction>(user);
                    auto &&rhs2 = bin->GetRHS();

                    auto &&result0 = IBinaryInst::DoIBinOperate(OP_REM, loop_time, rhs2, entry);
                    auto &&result1 = IBinaryInst::DoIBinOperate(OP_MUL, result0, rhs, entry);

                    auto &&result2 = IBinaryInst::DoIBinOperate(OP_ADD, result1, rhs2, entry);
                    auto &&result3 = IBinaryInst::DoIBinOperate(OP_REM, result2, rhs2, entry);

                    ReplaceSRC(phi->GetResult(), result3);

                    entry->InsertInstBack(JumpInst::CreatePtr(exit, entry));

                    entry->RmvSuccessor(loop_cond);
                    exit->RmvPredecessor(loop_cond);
                } else {
                    return;
                }
            }
        }

        assert(false);
    }
    return;
}