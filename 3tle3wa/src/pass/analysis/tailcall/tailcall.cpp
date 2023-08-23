#include "3tle3wa/pass/analysis//tailcall/tailcall.hh"

#include <cassert>
#include <iterator>
#include <memory>

#include "3tle3wa/ir/instruction/controlFlowInst.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"

void TailCall::TailCallAnalysis(NormalFuncPtr func) {
    auto exit = func->GetExitNode().get();
    auto last_inst = exit->GetLastInst().get();
    assert(last_inst->IsReturnInst());

    auto ret_inst = static_cast<ReturnInst *>(last_inst);
    auto ret_value = ret_inst->GetRetValue().get();

    if (ret_value != nullptr) {
        if (ret_value && ret_value->GetParent()) {
            if (auto call_inst = dynamic_cast<CallInst *>(ret_value->GetParent().get())) {
                auto block = call_inst->GetParent().get();

                bool next_of_call = false;
                for (auto &&inst : block->GetInstList()) {
                    if (inst.get() == call_inst) {
                        next_of_call = true;
                        continue;
                    }
                    if (next_of_call) {
                        if (inst.get() == ret_inst) {  // next inst of call is ret
                            call_inst->SetTailCall();
                        } else {
                            next_of_call = false;
                        }
                    }
                }
            } else if (auto phi_inst = dynamic_cast<PhiInst *>(ret_value->GetParent().get())) {
                auto &&block = phi_inst->GetParent();

                bool next_of_call = false;
                for (auto &&inst : block->GetInstList()) {
                    if (inst.get() == phi_inst) {
                        next_of_call = true;
                        continue;
                    }
                    if (next_of_call) {
                        if (inst.get() == ret_inst) {  // next inst of phi is ret
                            for (auto &&[value, blk] : phi_inst->GetDataList()) {
                                if (value->GetParent() && value->GetParent()->IsCallInst()) {
                                    assert(blk->GetLastInst()->IsJumpInst());
                                    auto phi_call = static_cast<CallInst *>(value->GetParent().get());
                                    auto &&iter = blk->GetInstList().end();
                                    std::advance(iter, -2);
                                    if ((*iter).get() == phi_call) {
                                        phi_call->SetTailCall();
                                    }
                                }
                            }
                        } else {
                            next_of_call = false;
                        }
                    }
                }
            }
        }
    } else {
        if (exit->GetInstCnt() > 1) {
            auto &&iter = exit->GetInstList().end();
            std::advance(iter, -2);
            auto second_last_inst = (*iter).get();
            if (second_last_inst->IsCallInst()) {
                auto call_inst = static_cast<CallInst *>(second_last_inst);
                call_inst->SetTailCall();
            }
        }
    }
    return;
}