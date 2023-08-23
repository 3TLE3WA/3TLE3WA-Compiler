#include "3tle3wa/pass/intraprocedural/inline/inline.hh"

#include <memory>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/function.hh"
#include "3tle3wa/ir/instruction/controlFlowInst.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/ir/value/variable.hh"
#include "3tle3wa/utils/logs.hh"

BaseValuePtr Inline::InstCopy(InstPtr &inst_, CfgNodePtr &parent) {
    assert(!(inst_->IsReturnInst() || inst_->IsPhiInst()));
    BaseValuePtr result = nullptr;
    OpCode opcode = inst_->GetOpCode();
    if (inst_->IsOneOprandInst()) {
        auto &&unary_inst_ = std::static_pointer_cast<UnaryInstruction>(inst_);
        auto &&oprand = value_map[unary_inst_->GetOprand()];
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
        auto &&lhs = value_map[binary_inst_->GetLHS()];
        auto &&rhs = value_map[binary_inst_->GetRHS()];
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
            off_list.push_back(value_map[off]);
        }
        result = GetElementPtrInst::DoGetPointer(gep_inst_->GetStoreType(), value_map[gep_inst_->GetBaseAddr()],
                                                 off_list, parent);
    } else if (inst_->IsCallInst()) {
        auto &&call_inst_ = std::static_pointer_cast<CallInst>(inst_);
        ParamList param_list;
        for (auto &&param : call_inst_->GetParamList()) {
            param_list.push_back(value_map[param]);
        }
        result = CallInst::DoCallFunction(call_inst_->GetRetType(), call_inst_->GetCalleeFunc(), param_list, parent);
    }
    assert((result != nullptr) || (inst_->IsCallInst()));
    return result;
}

std::pair<BaseValuePtr, CfgNodePtr> Inline::Inline(NormalFuncPtr &caller, NormalFunction *callee, ParamList &param_list,
                                                   NameValueMap &glb_table, CfgNodePtr &cur_block, bool in_loop,
                                                   CfgNodePtr &out_loop_block) {
    assert(value_map.empty() && block_map.empty());
    Log("Do Inline");
    BaseValuePtr ret_value = nullptr;
    CfgNodePtr ret_block = nullptr;

    // init value-map with global-value and func-parameter
    for (auto &&[_, value] : glb_table) value_map[value] = value;
    for (size_t size = param_list.size(), idx = 0; idx < size; ++idx)
        value_map[callee->GetParamList()[idx]] = param_list[idx];
    for (auto [_, constant] : ConstantAllocator::GetConstantAllocator()) {
        value_map[constant] = constant;
    }

    // terminate-inst store here wait for jump-target
    std::list<std::pair<JumpInstPtr, JumpInstPtr>> jump_list;
    std::list<std::pair<BranchInstPtr, BranchInstPtr>> branch_list;

    CfgNodeList inline_blks;
    for (auto node : callee->GetSequentialNodes()) {
        block_map[node] = cur_block;
        cur_block->blk_attr = node->blk_attr;
        cur_block->blk_attr.ClrBlkTypes(BlkAttr::Entry, BlkAttr::GoReturn, BlkAttr::Exit);
        // replace orgin-goreturn to inline-goreturn
        if (node->blk_attr.ChkOneOfBlkType(BlkAttr::GoReturn)) cur_block->blk_attr.AppBlkTypes(BlkAttr::InlineGR);

        inline_blks.push_back(cur_block);

        for (auto inst_ : node->GetInstList()) {
            if (inst_->IsReturnInst()) {  // inst->ret-inst, set ret_value and block
                auto &&ret_inst = std::static_pointer_cast<ReturnInst>(inst_);
                ret_value = value_map[ret_inst->GetRetValue()];
                ret_block = cur_block;
            } else if (inst_->IsJumpInst()) {
                auto &&jump_inst = JumpInst::CreatePtr(nullptr, cur_block);
                cur_block->InsertInstBack(jump_inst);
                jump_list.push_back({jump_inst, std::static_pointer_cast<JumpInst>(inst_)});
            } else if (inst_->IsBranchInst()) {
                auto &&branch_inst_ = std::static_pointer_cast<BranchInst>(inst_);
                auto &&cond = value_map[branch_inst_->GetCondition()];
                auto &&branch_inst = BranchInst::CreatePtr(cond, nullptr, nullptr, cur_block);
                cur_block->InsertInstBack(branch_inst);
                branch_list.push_back({branch_inst, branch_inst_});
            } else if (inst_->IsStoreInst()) {
                auto &&store_inst = std::static_pointer_cast<StoreInst>(inst_);
                auto &&store_value = value_map[store_inst->GetStoreValue()];
                StoreInst::DoStoreValue(value_map[store_inst->GetStoreAddr()], store_value, cur_block);
            } else if (inst_->IsAllocaInst()) {
                auto alloca_inst_ = std::static_pointer_cast<AllocaInst>(inst_);
                VariablePtr result = Variable::CreatePtr(alloca_inst_->GetAllocaAddr()->GetBaseType(), nullptr);
                auto &&inst = AllocaInst::CreatePtr(alloca_inst_->GetAllocaType(), result, nullptr);
                result->SetParent(inst);
                if (in_loop) {
                    out_loop_block->GetInstList().insert(--(out_loop_block->GetInstList().end()), inst);
                    inst->SetParent(out_loop_block);
                } else {
                    cur_block->InsertInstBack(inst);
                    inst->SetParent(cur_block);
                }
                value_map[inst_->GetResult()] = result;
            } else if (inst_->IsPhiInst()) {
                auto phi_inst_ = std::static_pointer_cast<PhiInst>(inst_);
                auto &&inst = PhiInst::CreatePtr(phi_inst_->GetResult()->GetBaseType(), cur_block);
                value_map[inst_->GetResult()] = inst->GetResult();
                for (auto &&[value, block] : phi_inst_->GetDataList()) {
                    auto &&new_value = value_map[value];
                    auto &&new_block = block_map[block];
                    PhiInst::InsertPhiData(inst, new_value, new_block);
                }
            } else {  // insert inst->copy to cur_block
                value_map[inst_->GetResult()] = InstCopy(inst_, cur_block);
            }
        }
        cur_block = caller->CreateCfgNode();
    }
    // fill jump-target
    for (auto &&[jump1, jump2] : jump_list) {
        jump1->SetTarget(block_map[jump2->GetTarget()]);
    }
    for (auto &&[branch1, branch2] : branch_list) {
        branch1->SetTrueTarget(block_map[branch2->GetTrueTarget()]);
        branch1->SetFalseTarget(block_map[branch2->GetFalseTarget()]);
    }

    // after inline, clear
    value_map.clear();
    block_map.clear();

    return {ret_value, ret_block};
}

void Inline::InlineOptFunc(NormalFuncPtr &func, SymbolTable &glb_table) {
    for (auto &&callee : func->GetCallWho()) {
        if (callee->IsLibFunction() == false) {
            auto callee_func = static_cast<NormalFunction *>(callee);
            CfgNodeList call_nodes;
            for (auto &&node : func->GetSequentialNodes()) {
                for (auto &&inst : node->GetInstList()) {
                    if (inst->IsCallInst()) {
                        auto &&call_inst = std::static_pointer_cast<CallInst>(inst);
                        if (call_inst->GetCalleeFunc().get() == callee_func && callee_func->GetRecursive() == false) {
                            call_nodes.push_back(node);
                        }
                    }
                }
            }
            for (auto &&node : call_nodes) {
                // split call node
                CallInstPtr call = nullptr;
                CfgNodePtr upper_node = func->CreateCfgNode();
                upper_node->blk_attr = node->blk_attr;
                if (upper_node->blk_attr.ChkOneOfBlkType(BlkAttr::Entry)) func->SetEntryNode(upper_node);
                if (upper_node->blk_attr.ChkOneOfBlkType(BlkAttr::Continue, BlkAttr::Break, BlkAttr::GoReturn,
                                                         BlkAttr::InlineGR, BlkAttr::Exit)) {
                    upper_node->blk_attr.ClrBlkTypes(BlkAttr::Continue, BlkAttr::Break, BlkAttr::GoReturn,
                                                     BlkAttr::InlineGR, BlkAttr::Exit);
                }

                auto &&inst_list = node->GetInstList();
                auto &&iter = inst_list.begin();
                for (; iter != inst_list.end(); ++iter) {
                    auto &&inst = (*iter);
                    if (inst->IsCallInst()) {
                        auto &&call_inst = std::static_pointer_cast<CallInst>(inst);
                        if (call_inst->GetCalleeFunc().get() != callee_func) {
                            upper_node->InsertInstBack(inst);
                        } else {
                            call = call_inst;
                            break;
                        }
                    } else {
                        upper_node->InsertInstBack(inst);
                    }
                }
                CfgNodePtr inline_block = func->CreateCfgNode(BlkAttr::Normal);
                upper_node->InsertInstBack(JumpInst::CreatePtr(inline_block, upper_node));
                for (auto &&inst : upper_node->GetInstList()) {
                    inst->SetParent(upper_node);
                }

                CfgNodePtr out_loop_block = nullptr;
                auto &&[ret_value, ret_block] =
                    Inline(func, callee_func, call->GetParamList(), glb_table.GetNameValueMap(), inline_block, false,
                           out_loop_block);
                ReplaceSRC(call->GetResult(), ret_value);

                inline_block = ret_block;

                inline_block->blk_attr = node->blk_attr;
                if (inline_block->blk_attr.ChkOneOfBlkType(BlkAttr::Exit)) func->SetExitNode(inline_block);
                if (inline_block->blk_attr.ChkOneOfBlkType(BlkAttr::Entry)) {
                    inline_block->blk_attr.ClrBlkTypes(BlkAttr::Entry);
                }

                for (++iter; iter != inst_list.end(); ++iter) {
                    auto &&inst = (*iter);
                    inline_block->InsertInstBack(inst);
                }

                for (auto &&pred : node->GetPredecessors()) {
                    upper_node->AddPredecessor(pred);
                    pred->AddSuccessor(upper_node);
                    pred->GetLastInst()->ReplaceTarget(node, upper_node);
                }
                for (auto &&succ : node->GetSuccessors()) {
                    inline_block->AddSuccessor(succ);
                    succ->AddPredecessor(inline_block);
                }
                RemoveNode(node);
            }
        }
    }
    return;
}
