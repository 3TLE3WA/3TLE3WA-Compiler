#include "3tle3wa/backend/rl/InternalTranslation.hh"

#include "3tle3wa/backend/asm/AsmBasicBlock.hh"
#include "3tle3wa/backend/asm/AsmGlobalValue.hh"
#include "3tle3wa/backend/asm/AsmInstruction.hh"
#include "3tle3wa/backend/asm/AsmLocalConstant.hh"
#include "3tle3wa/backend/asm/AsmProgress.hh"
#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLPlanner.hh"
#include "3tle3wa/backend/rl/RLProgress.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/utils/logs.hh"

InternalTranslation::InternalTranslation(const NormalFuncPtr &fptr, const std::unordered_map<uint32_t, size_t> &lc_map,
                                         const std::unordered_map<size_t, AsmGlobalValue *> &gv_map)
    : fptr_(fptr),
      lc_map_(lc_map),
      gv_map_(gv_map),
      rlps_(std::make_unique<RLProgress>(fptr->GetFuncName())),
      apg_(std::make_unique<AsmProgress>(fptr->GetFuncName())) {}

void InternalTranslation::DoTranslation() {
    InternalTranslationContext ctx = {.icmp_map{},
                                      .fcmp_map{},
                                      .gep_map{},
                                      .lui_map{},
                                      .fimm_map{},
                                      .phi_vec{},
                                      .cur_cfg = nullptr,
                                      .nxt_cfg = nullptr,
                                      .cur_blk = nullptr,
                                      .planner = nullptr,
                                      .meet_call = false,
                                      .meet_tail = false};

    auto planner = std::make_unique<RLPlanner>(fptr_->GetVarIdx());

    ctx.planner = planner.get();

    rlps_->RegisterPlanner(std::move(planner));
    rlps_->RegisterParams(fptr_->GetParamList());

    auto &&topo = fptr_->GetSequentialNodes();
    auto &&nxt_cfgit = topo.begin();
    auto &&cur_cfgit = nxt_cfgit++;

    while (cur_cfgit != topo.end()) {
        auto rlbb = std::make_unique<RLBasicBlock>(*cur_cfgit, ctx.planner, rlps_.get());

        ctx.cur_cfg = cur_cfgit->get();

        if (nxt_cfgit == topo.end()) {
            ctx.nxt_cfg = nullptr;
        } else {
            ctx.nxt_cfg = nxt_cfgit->get();
        }

        ctx.cur_blk = rlbb.get();
        ctx.meet_tail = false;
        ctx.lui_map.clear();
        ctx.fimm_map.clear();

        for (auto &&inst : (*cur_cfgit)->GetInstList()) {
            inst->TranslateTo(*this, ctx);
        }

        rlps_->RegisterBasicBlock(std::move(rlbb), ctx.meet_tail);

        cur_cfgit = nxt_cfgit++;
    }

    if (ctx.meet_call) {
        rlps_->MeetCall();
    }

    ctx.cur_cfg = nullptr;
    ctx.nxt_cfg = nullptr;

    dephi(ctx);
}

std::unique_ptr<AsmProgress> InternalTranslation::ExportAP() { return std::move(apg_); }