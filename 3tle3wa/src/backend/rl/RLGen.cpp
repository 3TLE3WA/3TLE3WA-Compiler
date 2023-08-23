#include "3tle3wa/backend/rl/RLGen.hh"

#include "3tle3wa/backend/asm/AsmBasicBlock.hh"
#include "3tle3wa/backend/asm/AsmGen.hh"
#include "3tle3wa/backend/asm/AsmGlobalValue.hh"
#include "3tle3wa/backend/asm/AsmInstruction.hh"
#include "3tle3wa/backend/asm/AsmLocalConstant.hh"
#include "3tle3wa/backend/asm/AsmProgress.hh"
#include "3tle3wa/backend/rl/InternalTranslation.hh"
#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLPlanner.hh"
#include "3tle3wa/backend/rl/RLProgress.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/backend/utils.hh"
#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/utils/logs.hh"

RLGen::RLGen() : lc_map_(), gv_map_(), translation_tasks_(), asm_gen_(std::make_unique<AsmGen>()) {}

void RLGen::Register(CompilationUnit &comp_unit) {
    auto &&nvmap = comp_unit.getGlbTable().GetNameValueMap();
    for (auto &&[name, value] : nvmap) {
        if (value->IsGlobalValue()) {
            auto gvp = dynamic_cast<GlobalValue *>(value.get());
            Assert(gvp, "bad dynamic cast");
            registerGlobalValue(gvp, name);
        }

        if (value->IsConstant() and value->GetBaseType()->FloatType()) {
            static size_t lc_idx_alloc = 0;
            auto cvp = dynamic_cast<Constant *>(value.get());
            Assert(cvp, "bad dynamic cast");
            registerLocalConstant(cvp, lc_idx_alloc++);
        }
    }

    registerNormalFunction(comp_unit.GetNormalFuncTable());
}

std::unique_ptr<AsmGen> &RLGen::AG() { return asm_gen_; }

void RLGen::registerGlobalValue(GlobalValue *gvp, const std::string &name) {
    auto &&init_val_ptr = gvp->GetInitValue().get();

    if (init_val_ptr->IsUnInitVar()) {
        auto &&uninit_val_ptr = dynamic_cast<UnInitVar *>(init_val_ptr);
        Assert(uninit_val_ptr, "bad dynamic cast");

        if (uninit_val_ptr->GetBaseType()->IsArray()) {
            auto &&uninit_array_type_ptr = dynamic_cast<ListType *>(uninit_val_ptr->GetBaseType().get());
            Assert(uninit_array_type_ptr, "bad dynamic cast");

            auto agv = std::make_unique<AsmGlobalValue>(name, uninit_array_type_ptr->GetCapacity() * 4, true, 0);

            CRVC_UNUSE auto result = gv_map_.emplace(gvp->GetGlobalValueIdx(), agv.get());
            Assert(result.second, "insert global variable fail!");

            asm_gen_->PushAsmGlobalValue(std::move(agv));
            return;
        }

        auto agv = std::make_unique<AsmGlobalValue>(name, 4, true, 0);

        CRVC_UNUSE auto result = gv_map_.emplace(gvp->GetGlobalValueIdx(), agv.get());
        Assert(result.second, "insert global variable fail!");

        asm_gen_->PushAsmGlobalValue(std::move(agv));
        return;
    }

    if (init_val_ptr->GetBaseType()->IsArray()) {
        auto init_array_ptr = dynamic_cast<ConstArray *>(init_val_ptr);
        Assert(init_array_ptr, "bad dynamic cast");

        auto init_array_type_ptr = dynamic_cast<ListType *>(init_array_ptr->GetBaseType().get());
        Assert(init_array_type_ptr, "bad dynamic cast");

        auto capacity = init_array_type_ptr->GetCapacity();
        auto agv = std::make_unique<AsmGlobalValue>(name, capacity * 4, false, capacity);

        // extract const array
        auto push_init_value = [init_array_ptr, &agv]() {
            auto &&cstarr = init_array_ptr->GetConstArr();

            for (auto &&v : cstarr) {
                auto &&cinfo = XConstValue(v->GetValue());

                // no need to check for now
                agv->Push(cinfo.v32_.u32_);
            }
        };
        push_init_value();

        CRVC_UNUSE auto result = gv_map_.emplace(gvp->GetGlobalValueIdx(), agv.get());
        Assert(result.second, "insert global variable fail!");

        asm_gen_->PushAsmGlobalValue(std::move(agv));
        return;
    }

    auto init_val = dynamic_cast<Constant *>(init_val_ptr);
    auto &&cinfo = XConstValue(init_val->GetValue());
    auto agv = std::make_unique<AsmGlobalValue>(name, 4, false, 1);
    agv->Push(cinfo.v32_.u32_);

    CRVC_UNUSE auto result = gv_map_.emplace(gvp->GetGlobalValueIdx(), agv.get());
    Assert(result.second, "insert global variable fail!");

    asm_gen_->PushAsmGlobalValue(std::move(agv));
}

void RLGen::registerNormalFunction(NormalFuncList &nflst) {
    for (auto &&nf : nflst) {
        auto itunit = std::make_unique<InternalTranslation>(nf, lc_map_, gv_map_);
        translation_tasks_.push_back(std::move(itunit));
    }
}

void RLGen::registerLocalConstant(Constant *cvp, const size_t idx) {
    auto &&cinfo = XConstValue(cvp->GetValue());
    Assert(cinfo.isflt_ and cinfo.width_ == 32, "not float local constant");
    if (lc_map_.find(cinfo.v32_.u32_) == lc_map_.end()) {
        auto lcv = std::make_unique<AsmLocalConstant>(idx, cinfo.v32_.u32_);
        lc_map_.emplace(cinfo.v32_.u32_, idx);
        asm_gen_->PushAsmLocalConstant(std::move(lcv));
    }
}

void RLGen::SerialGenerate() {
    for (auto &&task : translation_tasks_) {
        task->DoTranslation();
        task->DoAssignment();
        task->DoTranslateToAsm();
        asm_gen_->PushAsmProgress(task->ExportAP());
    }
}