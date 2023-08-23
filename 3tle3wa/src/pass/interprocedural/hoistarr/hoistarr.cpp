#include <cassert>
#include <cstddef>
#include <cstdint>
#include <list>
#include <memory>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <variant>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/memoryInst.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/constarray.hh"
#include "3tle3wa/ir/value/globalvalue.hh"
#include "3tle3wa/ir/value/type/baseType.hh"
#include "3tle3wa/ir/value/type/listType.hh"
#include "3tle3wa/ir/value/use.hh"
#include "3tle3wa/pass/interprocedural/hositarr/hositarr.hh"

void HoistLocalArray::HoistLocalArray(NormalFuncPtr &func, SymbolTable &glb_table) {
    std::map<CtrlFlowGraphNode *, InstList> del_lists;
    std::map<BaseValuePtr, BaseValuePtr> replace;
    // find const local array
    for (auto &&node : func->GetSequentialNodes()) {
        // assume a array which can hoist is
        // mannualy initialized after allcate
        // and no more assignment, each slot assign only one time
        for (auto &&inst : node->GetInstList()) {
            // only consider alloca and initilize at same block
            if (inst->IsAllocaInst()) {
                bool can_hoist = true;  // assume can hoist for each alloca
                InstList inst_list;

                auto &&alloca = std::static_pointer_cast<AllocaInst>(inst);
                auto &&address = alloca->GetResult();
                auto list_type = std::static_pointer_cast<ListType>(alloca->GetAllocaType());

                auto &&zero = list_type->IntType() ? ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))
                                                   : ConstantAllocator::FindConstantPtr(static_cast<float>(0));

                ConstArr const_array;
                for (size_t idx = 0, size = list_type->GetArrSize(); idx < size; ++idx) {
                    const_array.push_back(zero);
                }
                while (const_array.size() % ARR_SIZE_ALIGN != 0) {
                    const_array.push_back(zero);
                }

                // each offset store one time at most, mannully store to constant offset
                // get all related gep
                std::list<GepInstPtr> gep_in_node;
                std::list<GepInstPtr> gep_out_node;
                std::queue<GepInstPtr> queue;
                for (auto &&user : address->GetUserList()) {
                    if (user->IsGepInst()) {
                        auto &&gep = std::static_pointer_cast<GetElementPtrInst>(user);
                        queue.push(gep);
                    } else {
                        assert(false);
                    }
                }
                while (!queue.empty()) {
                    auto &&front = queue.front();
                    queue.pop();
                    if (front->GetParent() == node) {
                        gep_in_node.push_back(front);
                    } else {
                        gep_out_node.push_back(front);
                    }
                    for (auto &&user : front->GetResult()->GetUserList()) {
                        if (user->IsGepInst()) {
                            queue.push(std::static_pointer_cast<GetElementPtrInst>(user));
                        }
                    }
                }
                // analysis gep out node
                for (auto &&gep : gep_out_node) {  // gep no in `node`
                    for (auto &&user : gep->GetResult()->GetUserList()) {
                        if (user->IsStoreInst()) {
                            can_hoist = false;
                        } else if (user->IsCallInst()) {
                            auto &&callee = std::static_pointer_cast<CallInst>(user)->GetCalleeFunc();
                            if (callee->GetSideEffect()) {
                                can_hoist = false;
                            }
                        }
                    }
                }
                if (can_hoist == false) {
                    continue;  // check to next alloca
                }
                // analyisi gep in node
                for (auto &&gep : gep_in_node) {
                    auto &&off = gep->GetOffList().back();
                    if (off->IsVariable()) {
                        for (auto &&user : gep->GetResult()->GetUserList()) {
                            if (user->IsStoreInst()) {
                                can_hoist = false;
                            } else if (user->IsCallInst()) {
                                auto &&callee = std::static_pointer_cast<CallInst>(user)->GetCalleeFunc();
                                if (callee->GetSideEffect()) {
                                    can_hoist = false;
                                }
                            }
                        }
                        if (can_hoist == false) break;
                    } else if (off->IsConstant()) {
                        size_t store_cnt = 0;
                        StoreInstPtr store_inst = nullptr;
                        int32_t offset = std::get<int32_t>(std::static_pointer_cast<Constant>(off)->GetValue());

                        for (auto &&user : gep->GetResult()->GetUserList()) {
                            if (user->IsStoreInst()) {
                                store_inst = std::static_pointer_cast<StoreInst>(user);
                                store_cnt += 1;
                            } else if (user->IsBitCast()) {
                                inst_list.push_back(user);  // bitcast
                                assert(user->GetResult()->GetUserList().size() == 1);
                                inst_list.push_back(*user->GetResult()->GetUserList().begin());  // memset
                            } else if (user->IsCallInst()) {
                                auto &&callee = std::static_pointer_cast<CallInst>(user)->GetCalleeFunc();
                                if (callee->GetSideEffect()) {
                                    can_hoist = false;
                                }
                            }
                        }
                        if (!can_hoist) break;
                        if (store_cnt > 1) {
                            can_hoist = false;
                        } else if (store_inst != nullptr) {
                            if (store_inst->GetParent() != node) {
                                can_hoist = false;
                                break;
                            }
                            auto &&store_value = store_inst->GetStoreValue();
                            if (store_value->IsVariable()) {
                                can_hoist = false;
                            } else if (store_value->IsConstant()) {
                                inst_list.push_back(store_inst);
                                const_array[offset] = std::static_pointer_cast<Constant>(store_value);
                            } else {
                                assert(false);
                            }
                        }
                        if (!can_hoist) break;
                    }
                }
                if (can_hoist) {
                    auto &&corr_del_list = del_lists[node.get()];
                    corr_del_list.push_back(alloca);
                    corr_del_list.insert(corr_del_list.end(), inst_list.begin(), inst_list.end());

                    auto arr_dim = list_type->GetArrDims();
                    auto &&type = ListType::CreatePtr((list_type->IntType() ? INT32 : FLOAT), IMMUTABLE, POINTER, ARRAY,
                                                      GLOBAL, arr_dim);
                    auto &&const_arr = ConstArray::CreatePtr(list_type, const_array);
                    GlobalValuePtr glb_value = GlobalValue::CreatePtr(type, const_arr);
                    std::string name =
                        ".array.hoist." + func->GetFuncName() + "." + std::to_string(alloca->GetInstIdx());
                    glb_table.InsertSymbol(name, glb_value);
                    glb_value->InsertUser(func.get());

                    replace[alloca->GetResult()] = glb_value;  // later replace alloca-result with global-value
                }
            }
        }
    }
    if (replace.size() > 0) {
        for (auto &&node : func->GetSequentialNodes()) {
            for (auto &&inst : node->GetInstList()) {
                if (inst->IsGepInst()) {
                    auto &&gep = std::static_pointer_cast<GetElementPtrInst>(inst);
                    auto &&ori_base_addr = gep->GetBaseAddr();
                    if (replace.count(ori_base_addr)) {
                        auto &&new_base_addr = replace[ori_base_addr];

                        ori_base_addr->RemoveUser(gep);
                        gep->SetBaseAddr(new_base_addr);
                        new_base_addr->InsertUser(gep);
                    }
                }
            }
        }
    }
    for (auto &&[node, del_list] : del_lists) {
        auto &&inst_list = node->GetInstList();
        for (auto &&inst : del_list) {
            assert(std::find(inst_list.begin(), inst_list.end(), inst) != inst_list.end());
            RemoveInst(inst);
            inst_list.remove(inst);
        }
    }
    return;
}