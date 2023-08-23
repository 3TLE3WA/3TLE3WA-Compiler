#include "3tle3wa/pass/intraprocedural/edp/edp.hh"

#include <cassert>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "3tle3wa/ir/function/function.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/pass/interprocedural/dce/dce.hh"

void EDP::EliminateDeadParameter(NormalFuncPtr func) {
    std::vector<bool> active_param;

    bool exist_inactive_param = false;
    auto &&fparam_list = func->GetParamList();

    for (size_t idx = 0; idx < func->GetParamSize(); ++idx) {
        auto &&param = fparam_list[idx];
        bool active = false;  // avoid recursion make dead-parameter inactive
        for (auto &&user : param->GetUserList()) {
            if (user->IsCallInst()) {
                auto &&call_inst = static_cast<CallInst *>(user.get());
                auto &&callee = call_inst->GetCalleeFunc();
                if (callee != func) {  // non-recursion
                    active = true;
                } else {  // recursion
                    auto &&rparam_list = call_inst->GetParamList();
                    if (param != rparam_list[idx]) {
                        active = true;
                    }
                }
            } else {
                active = true;
            }
        }
        active_param.push_back(active);

        if (active == false) {
            exist_inactive_param = true;
        }
    }

    if (exist_inactive_param == false) return;

    for (auto &&caller : func->GetWhoCall()) {
        assert(!caller->IsLibFunction());
        auto &&normal_caller = static_cast<NormalFunction *>(caller);

        for (auto &&node : normal_caller->GetSequentialNodes()) {
            for (auto &&inst : node->GetInstList()) {
                if (inst->IsCallInst()) {
                    auto &&call_inst = std::static_pointer_cast<CallInst>(inst);
                    auto &&callee = call_inst->GetCalleeFunc();
                    if (callee != func) continue;
                    assert(callee == func);

                    size_t idx = 0;
                    auto &&rparam_list = call_inst->GetParamList();
                    for (auto &&iter = rparam_list.begin(); iter != rparam_list.end(); ++idx) {
                        if (active_param[idx] == false && (*iter)->IsVariable()) {
                            (*iter)->RemoveUser(call_inst);
                            iter = rparam_list.erase(iter);
                        } else {
                            ++iter;
                        }
                    }
                }
            }
        }
    }

    size_t idx = 0;
    for (auto &&iter = fparam_list.begin(); iter != fparam_list.end(); ++idx) {
        if (active_param[idx] == false) {
            iter = fparam_list.erase(iter);
            continue;
        }
        ++iter;
    }
    return;
}
