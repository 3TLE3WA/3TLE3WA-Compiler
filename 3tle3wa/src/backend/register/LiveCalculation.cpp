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

void InternalTranslation::DoAssignment() { rlps_->DoAssignment(); }

void RLProgress::DoAssignment() {
    // after this scheduler
    // other scheduler should operate on view not on raw
    DoFIFOSchedule();

    rewrite();

    do {
        cleanAll();

        computeLivenessInfomation();

        computeLiveInterval();

        planner_->CalculateWeights();

        auto spill = registerAllocation();

        if (not spill) {
            break;
        }

        planner_->GenerateStackInfo();

        rewrite();

    } while (true);

    planner_->GenerateStackInfo();
    
    rewrite();
}

void RLProgress::cleanAll() { planner_->ResetRAInfo(); }

void RLPlanner::ResetRAInfo() {
    for (auto &&vr : vr_storage_) {
        if (not vr->IsParam()) {
            vr->ResetRAInfo();
        }
    }
}

void RLProgress::computeLivenessInfomation() {
    size_t pridx = 0;
    for (auto &&bb : rlbbs_) {
        bb->ClearLiveSet();

        if (pridx == 0) {
            bb->SetLiveDefParams(planner_->Params());
            pridx += 1;
        }

        bb->InitCalculation();
    }

    bool changed = false;
    do {
        changed = false;
        for (auto &&rit = rlbbs_.rbegin(); rit != rlbbs_.rend(); ++rit) {
            changed = (*rit)->CalculateLiveInOut(lbmap_) or changed;
        }
    } while (changed);

    (*rlbbs_.begin())->SetLiveinParams(planner_->Params());
}

void RLBasicBlock::ClearLiveSet() {
    live_def_.clear();
    live_use_.clear();
    live_out_.clear();
    live_in_.clear();
}

void RLBasicBlock::SetLiveDefParams(std::vector<VirtualRegister *> &params) {
    for (auto &&param : params) {
        live_def_.insert(param->GetVRIdx());
    }
}

void RLBasicBlock::SetLiveinParams(std::vector<VirtualRegister *> &params) {
    for (auto &&param : params) {
        live_in_.insert(param->GetVRIdx());
    }
}

void RLBasicBlock::InitCalculation() {
    for (auto &&uop : uops_view_) {
        auto operands = uop->GetOperands();
        if (not operands.empty()) {
            for (auto &&operand : operands) {
                auto vridx = operand->GetVRIdx();
                if (live_def_.find(vridx) == live_def_.end()) {
                    live_use_.insert(vridx);
                }
            }
        }

        auto result = uop->GetResult();
        if (result != nullptr) {
            live_def_.insert(result->GetVRIdx());
        }
    }
}

bool RLBasicBlock::CalculateLiveInOut(std::unordered_map<size_t, RLBasicBlock *> &lbmap) {
    size_t prev_size = live_out_.size();

    for (auto &&succ : successors_) {
        auto succbb = lbmap.at(succ);
        live_out_.insert(succbb->live_in_.begin(), succbb->live_in_.end());
    }

    live_in_ = live_use_;
    std::set_difference(live_out_.begin(), live_out_.end(), live_def_.begin(), live_def_.end(),
                        std::inserter(live_in_, live_in_.end()));

    return prev_size != live_out_.size();
}

void RLProgress::computeLiveInterval() {
    size_t idx = 0;
    for (auto &&bb : rlbbs_) {
        bb->IndexAllUops(idx);
        bb->CalculateInterval();
    }
}

void RLBasicBlock::IndexAllUops(size_t &begin) {
    uop_idx_bgn_ = begin + 1;

    for (auto &&uop : uops_view_) {
        begin += 1;
        uop->SetUopIdx(begin);
    }

    uop_idx_end_ = begin;
}

void RLBasicBlock::CalculateInterval() {
    std::unordered_set<VirtualRegister *> collector;

    for (auto &&v : live_in_) {
        auto &&vr = planner_->GetVReg(v);
        vr->Def(uop_idx_bgn_, loop_info_.loop_time);
        collector.insert(vr);
    }

    for (auto &&uop : uops_view_) {
        size_t idx = uop->GetUopIdx();

        auto res = uop->GetResult();
        if (res != nullptr) {
            res->Def(idx, loop_info_.loop_time);
            collector.insert(res);
        }

        auto opds = uop->GetOperands();
        if (not opds.empty()) {
            for (auto opd : opds) {
                opd->Use(idx);
                collector.insert(opd);
            }
        }
    }

    for (auto &&v : live_out_) {
        auto &&vr = planner_->GetVReg(v);
        vr->Use(uop_idx_end_ + 1);
        collector.insert(vr);
    }

    for (auto &&vr : collector) {
        vr->GenSegment(loop_info_.loop_time);
    }
}