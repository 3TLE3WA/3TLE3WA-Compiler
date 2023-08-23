#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"

SCHED_TYPE UopRet::GetSchedType() { return SCHED_TYPE::FENCE; }

SCHED_TYPE UopCall::GetSchedType() { return SCHED_TYPE::FUNCCALL; }

SCHED_TYPE UopLui::GetSchedType() { return SCHED_TYPE::IMISC; }

SCHED_TYPE UopLi::GetSchedType() { return SCHED_TYPE::IMISC; }

SCHED_TYPE UopMv::GetSchedType() {
    if (dst_->UseFGPR()) {
        return SCHED_TYPE::FMISC;
    }
    return SCHED_TYPE::IMISC;
}

SCHED_TYPE UopCvtS2W::GetSchedType() { return SCHED_TYPE::FMISC; }

SCHED_TYPE UopCvtW2S::GetSchedType() { return SCHED_TYPE::FMISC; }

SCHED_TYPE UopBranch::GetSchedType() { return SCHED_TYPE::JMPBR; }

SCHED_TYPE UopJump::GetSchedType() { return SCHED_TYPE::JMPBR; }

SCHED_TYPE UopLla::GetSchedType() { return SCHED_TYPE::IMISC; }

SCHED_TYPE UopLoad::GetSchedType() { return SCHED_TYPE::LOAD; }

SCHED_TYPE UopStore::GetSchedType() { return SCHED_TYPE::STORE; }

SCHED_TYPE UopFLoad::GetSchedType() { return SCHED_TYPE::LOAD; }

SCHED_TYPE UopFLoadLB::GetSchedType() { return SCHED_TYPE::LOAD; }

SCHED_TYPE UopFStore::GetSchedType() { return SCHED_TYPE::STORE; }

SCHED_TYPE UopFCmp::GetSchedType() { return SCHED_TYPE::JMPBR; }

SCHED_TYPE UopIBin::GetSchedType() { return SCHED_TYPE::IMISC; }

SCHED_TYPE UopIBin64::GetSchedType() { return SCHED_TYPE::IMISC; }

SCHED_TYPE UopIBinImm::GetSchedType() { return SCHED_TYPE::IMISC; }

SCHED_TYPE UopIBinImm64::GetSchedType() { return SCHED_TYPE::IMISC; }

SCHED_TYPE UopFBin::GetSchedType() { return SCHED_TYPE::FMISC; }

SCHED_TYPE UopFTri::GetSchedType() { return SCHED_TYPE::FMISC; }

SCHED_TYPE UopICmpBranch::GetSchedType() { return SCHED_TYPE::JMPBR; }

SCHED_TYPE UopNop::GetSchedType() { return SCHED_TYPE::FENCE; }
