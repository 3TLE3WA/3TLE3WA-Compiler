#include <unordered_map>
#include <unordered_set>

#include "3tle3wa/backend/asm/AsmBasicBlock.hh"
#include "3tle3wa/backend/asm/AsmGlobalValue.hh"
#include "3tle3wa/backend/asm/AsmProgress.hh"
#include "3tle3wa/backend/asm/AsmSchedItem.hh"
#include "3tle3wa/backend/rl/InternalTranslation.hh"
#include "3tle3wa/backend/rl/LiveInterval.hh"
#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLGen.hh"
#include "3tle3wa/backend/rl/RLPlanner.hh"
#include "3tle3wa/backend/rl/RLProgress.hh"
#include "3tle3wa/backend/rl/RLSchedItem.hh"
#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/backend/scheduler/FIFO.hh"
#include "3tle3wa/backend/scheduler/FastUse.hh"
#include "3tle3wa/backend/scheduler/SchedMachine.hh"
#include "3tle3wa/backend/utils.hh"
#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/instruction/opCode.hh"

void RLProgress::DoFIFOSchedule() {
    for (auto &&rlbb : rlbbs_) {
        rlbb->FIFOSchedule();
    }
}

void RLBasicBlock::FIFOSchedule() {
    SchedMachine mach(std::make_unique<SchedFIFO>());

    for (auto &&uop : uops_) {
        auto item = std::make_unique<RLSchedItem>();
        item->uop = uop.get();

        mach.Push(std::move(item));
    }

    mach.Sched();

    auto fifo = dynamic_cast<SchedFIFO *>(mach.Policy());

    for (auto &&item : fifo->View()) {
        auto rlitem = dynamic_cast<RLSchedItem *>(item);
        uops_view_.push_back(rlitem->uop);
    }
}

void RLProgress::DoFastUseSchedule() {
    for (auto &&rlbb : rlbbs_) {
        rlbb->FastUseSchedule();
    }
}

void RLBasicBlock::FastUseSchedule() {
    SchedMachine mach(std::make_unique<SchedFastUse>());
    for (auto &&uop : uops_view_) {
        auto item = std::make_unique<RLSchedItem>();
        item->uop = uop;
        item->type = uop->GetSchedType();
        
        item->memop = (item->type == SCHED_TYPE::LOAD) or (item->type == SCHED_TYPE::STORE);
        
        if (auto res = uop->GetResult(); res != nullptr) {
            item->writes.push_back(res->GetVRIdx());
        }

        if (auto opds = uop->GetOperands(); not opds.empty()) {
            for (auto &&vr : opds) {
                item->reads.push_back(vr->GetVRIdx());
            }
        }

        mach.Push(std::move(item));
    }

    uops_view_.clear();

    mach.Sched();

    auto fast_use = dynamic_cast<SchedFastUse *>(mach.Policy());

    for (auto &&item : fast_use->View()) {
        auto rlitem = dynamic_cast<RLSchedItem *>(item);
        uops_view_.push_back(rlitem->uop);
    }
}