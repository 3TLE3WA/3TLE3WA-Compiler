#include "3tle3wa/pass/intraprocedural/gvl/gvl.hh"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <queue>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/function.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/memoryInst.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/globalvalue.hh"
#include "3tle3wa/ir/value/type/baseType.hh"
#include "3tle3wa/ir/value/type/scalarType.hh"
#include "3tle3wa/ir/value/use.hh"
#include "3tle3wa/ir/value/variable.hh"

void GVL::GlbValueLocalization(CompilationUnit &comp_unit) {
    auto &&glb_table = comp_unit.getGlbTable().GetNameValueMap();

    std::map<CtrlFlowGraphNode *, InstList> del_lists;

    for (auto &&iter = glb_table.begin(); iter != glb_table.end();) {
        auto &&[_, value] = (*iter);

        auto &&glb_value = std::static_pointer_cast<GlobalValue>(value);
        auto &&glbv_type = glb_value->GetBaseType();

        auto &&define_in = glb_value->GetDefineIn();
        auto &&used_in = glb_value->GetUsedIn();

        // glb-value never be loaded, and not do as a function parameter
        if (used_in.size() == 0 && glb_value->IsAsParam() == false) {
            if (define_in.size() != 0) {  // remove redundant store
                std::queue<InstPtr> queue;
                for (auto &&inst : glb_value->GetUserList()) {
                    queue.push(inst);
                }
                while (!queue.empty()) {
                    auto &&front = queue.front();
                    queue.pop();
                    del_lists[front->GetParent().get()].push_back(front);
                    if (front->GetResult() != nullptr) {
                        for (auto &&user : front->GetResult()->GetUserList()) {
                            queue.push(user);
                        }
                    }
                }
            }
            iter = glb_table.erase(iter);
            continue;
        }
        // only scalar glb-value need localization
        if (!(value->IsGlobalValue() && value->GetBaseType()->IsScalar())) {
            ++iter;
            continue;
        }
        if (define_in.size() == 0 && used_in.size() != 0) {  // only initilize no more assign
            auto init_value = glb_value->GetInitValue();
            assert(init_value->IsConstant() || init_value->IsUnInitVar());

            if (init_value->IsConstant() || init_value->IsUnInitVar()) {
                if (init_value->IsUnInitVar()) {
                    init_value = glbv_type->IntType() ? ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))
                                                      : ConstantAllocator::FindConstantPtr(static_cast<float>(0));
                }
                for (auto &&gep : glb_value->GetUserList()) {
                    assert(gep->IsGepInst());
                    del_lists[gep->GetParent().get()].push_back(gep);
                    if (gep->IsGepInst()) {
                        for (auto &&load : gep->GetResult()->GetUserList()) {
                            assert(load->IsLoadInst());
                            del_lists[load->GetParent().get()].push_back(load);
                            ReplaceSRC(load->GetResult(), init_value);
                        }
                    }
                }
                iter = glb_table.erase(iter);
                continue;
            }
        } else if (define_in.size() == 1 && used_in.size() == 1) {
            auto &&define_func = (*define_in.begin());
            auto &&used_func = (*used_in.begin());
            assert(!define_func->IsLibFunction() && !used_func->IsLibFunction());

            // only defined and used in main can be localization
            if (define_func == used_func && define_func->GetFuncName() == "main") {
                auto &&normal_func = static_cast<NormalFunction *>(define_func);

                Variable::SetVarIdx(normal_func->GetVarIdx());
                BasicBlock::SetBlkIdx(normal_func->GetBlkIdx());

                bool int_type = glbv_type->IntType();
                auto &&entry = normal_func->GetEntryNode();

                auto &&type_stored = int_type ? type_int_L : type_float_L;
                auto &&type_alloca = int_type ? type_int_ptr_L : type_float_ptr_L;

                VariablePtr address = Variable::CreatePtr(type_alloca, nullptr);
                auto &&alloca_inst = AllocaInst::CreatePtr(type_stored, address, entry);
                address->SetParent(alloca_inst);

                auto init_value = glb_value->GetInitValue();
                if (init_value->IsUnInitVar()) {
                    init_value = int_type ? ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))
                                          : ConstantAllocator::FindConstantPtr(static_cast<float>(0));
                }
                auto &&store_inst = StoreInst::CreatePtr(address, init_value, entry);

                entry->InsertInstFront(store_inst);
                entry->InsertInstFront(alloca_inst);

                for (auto &&gep : glb_value->GetUserList()) {
                    assert(gep->IsGepInst());
                    del_lists[gep->GetParent().get()].push_back(gep);
                    ReplaceSRC(gep->GetResult(), address);
                }
                normal_func->SetVarIdx(Variable::GetVarIdx());
                normal_func->SetBlkIdx(BasicBlock::GetBlkIdx());

                iter = glb_table.erase(iter);
                continue;
            }
        }
        ++iter;
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
