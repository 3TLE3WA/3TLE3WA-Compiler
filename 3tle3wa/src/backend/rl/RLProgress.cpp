#include "3tle3wa/backend/rl/RLProgress.hh"

#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLPlanner.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/ir/IR.hh"

RLProgress::RLProgress(const std::string &name)
    : label_(nullptr),
      call_info_{.call_libfunc = false, .call_self = false, .call_func = false},
      planner_(nullptr),
      rlbbs_(),
      lbmap_() {
    size_t label_len_ = 0;

    FILE *fp = open_memstream(&label_, &label_len_);
    fprintf(fp, "%s", name.c_str());
    fflush(fp);
    fclose(fp);
}

RLProgress::~RLProgress() {
    if (label_) {
        free(label_);
        label_ = nullptr;
    }
}

void RLProgress::MeetCall() { call_info_.call_func = true; }
void RLProgress::MeetLibCall() { call_info_.call_libfunc = true; }
void RLProgress::MeetCallOther() { call_info_.call_self = false; }
void RLProgress::MeetCallSelf() { call_info_.call_self = true; }

void RLProgress::RegisterPlanner(std::unique_ptr<RLPlanner> planner) {
    planner_ = std::move(planner);
    planner_->RegisterOwner(this);
}

void RLProgress::RegisterBasicBlock(std::unique_ptr<RLBasicBlock> rlbb, bool tail_call) {
    if (tail_call) {
        rlbb->SetHasTailCall(true);
    }
    lbmap_[rlbb->GetLabelIdx()] = rlbb.get();
    rlbbs_.push_back(std::move(rlbb));
}

void RLProgress::RegisterParams(ParamList &plst) { planner_->RegisterParams(plst); }

RLBasicBlock *RLProgress::FindBlkById(size_t lbidx) { return lbmap_.at(lbidx); }