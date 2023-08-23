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
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/backend/scheduler/FIFO.hh"
#include "3tle3wa/backend/scheduler/SchedMachine.hh"
#include "3tle3wa/backend/utils.hh"
#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/instruction/opCode.hh"

void RLProgress::rewrite() {
    for (auto &&rlbb : rlbbs_) {
        rlbb->Rewrite();
    }
}

void RLBasicBlock::Rewrite() {
    for (auto it = uops_view_.begin(); it != uops_view_.end(); ++it) {
        (*it)->Rewrite(it, this, planner_);
    }
}

std::list<UopGeneral *>::iterator RLBasicBlock::InsertToView(std::list<UopGeneral *>::iterator it, UopGeneral *uop) {
    return uops_view_.insert(it, uop);
}

static inline VirtualRegister *load(std::list<UopGeneral *>::iterator it, RLBasicBlock *rlbb, RLPlanner *plan,
                                    VirtualRegister *from) {
    if (from == nullptr or not from->IsOnStk()) {
        return from;
    }

    auto si = from->GetStackInfo();
    auto off = si->GetStackOff();
    auto to = plan->NewVReg(from->GetVRType());
    to->SetCanSpill(false);
    to->SetStackInfo(si);

    if (from->UseFGPR()) {
        auto uop = new UopFLoad;
        uop->SetDst(to);
        uop->SetOff(off);
        uop->SetBase(nullptr);

        rlbb->PushUop(uop);
        rlbb->InsertToView(it, uop);
    } else {
        auto uop = new UopLoad;
        uop->SetDst(to);
        uop->SetOff(off);
        uop->SetBase(nullptr);

        rlbb->PushUop(uop);
        rlbb->InsertToView(it, uop);
    }

    return to;
}

static inline VirtualRegister *store(std::list<UopGeneral *>::iterator it, RLBasicBlock *rlbb, RLPlanner *plan,
                                     VirtualRegister *to) {
    if (to == nullptr or not to->IsOnStk()) {
        return to;
    }

    auto si = to->GetStackInfo();
    auto off = si->GetStackOff();
    auto from = plan->NewVReg(to->GetVRType());
    from->SetCanSpill(false);
    from->SetStackInfo(si);

    it++;

    if (from->UseFGPR()) {
        auto uop = new UopFStore;
        uop->SetSrc(from);
        uop->SetOff(off);
        uop->SetBase(nullptr);

        rlbb->PushUop(uop);
        rlbb->InsertToView(it, uop);
    } else {
        auto uop = new UopStore;
        uop->SetSrc(from);
        uop->SetOff(off);
        uop->SetBase(nullptr);

        rlbb->PushUop(uop);
        rlbb->InsertToView(it, uop);
    }

    return from;
}

#define LOAD(x) (x) = load(it, rlbb, plan, (x))
#define STORE(x) (x) = store(it, rlbb, plan, (x))
#define REWRITE() \
    Rewrite(CRVC_UNUSE std::list<UopGeneral *>::iterator it, CRVC_UNUSE RLBasicBlock *rlbb, CRVC_UNUSE RLPlanner *plan)

void UopRet::REWRITE() { LOAD(retval_); }

void UopCall::REWRITE() {
    // for (auto &&v : params_) {
    //     LOAD(v);
    // }

    STORE(retval_);
}
void UopLui::REWRITE() { STORE(dst_); }

void UopLi::REWRITE() { STORE(dst_); }

void UopLla::REWRITE() {
    if (not src_.empty()) {
        STORE(dst_);
        return;
    }

    auto addr = dst_->GetAddressInfo();
    STORE(dst_);
    dst_->SetAddressInfo(addr);
}

void UopMv::REWRITE() {
    LOAD(src_);
    STORE(dst_);
}

void UopCvtS2W::REWRITE() {
    LOAD(src_);
    STORE(dst_);
}

void UopCvtW2S::REWRITE() {
    LOAD(src_);
    STORE(dst_);
}

void UopBranch::REWRITE() { LOAD(cond_); }

void UopJump::REWRITE() {}

void UopLoad::REWRITE() {
    if (base_ == nullptr) {
        off_lo12_ = dst_->GetStackInfo()->GetStackOff();
        return;
    }
    LOAD(base_);
    STORE(dst_);
}

void UopStore::REWRITE() {
    if (base_ == nullptr) {
        off_lo12_ = src_->GetStackInfo()->GetStackOff();
        return;
    }
    LOAD(base_);
    LOAD(src_);
}

void UopFLoad::REWRITE() {
    if (base_ == nullptr) {
        off_lo12_ = dst_->GetStackInfo()->GetStackOff();
        return;
    }
    LOAD(base_);
    STORE(dst_);
}

void UopFLoadLB::REWRITE() {
    LOAD(helper_);
    STORE(dst_);
}

void UopFStore::REWRITE() {
    if (base_ == nullptr) {
        off_lo12_ = src_->GetStackInfo()->GetStackOff();
        return;
    }
    LOAD(base_);
    LOAD(src_);
}

void UopFCmp::REWRITE() {
    LOAD(lhs_);
    LOAD(rhs_);
    STORE(dst_);
}

void UopIBin::REWRITE() {
    LOAD(lhs_);
    LOAD(rhs_);
    STORE(dst_);
}

void UopIBin64::REWRITE() {
    LOAD(lhs_);
    LOAD(rhs_);
    STORE(dst_);
}

void UopIBinImm::REWRITE() {
    LOAD(lhs_);
    STORE(dst_);
}

void UopIBinImm64::REWRITE() {
    LOAD(lhs_);
    STORE(dst_);
}

void UopFBin::REWRITE() {
    LOAD(lhs_);
    LOAD(rhs_);
    STORE(dst_);
}

void UopFTri::REWRITE() {
    LOAD(lhs_);
    LOAD(rhs_);
    LOAD(ahs_);
    STORE(dst_);
}

void UopICmpBranch::REWRITE() {
    LOAD(lhs_);
    LOAD(rhs_);
}

void UopNop::REWRITE() {}