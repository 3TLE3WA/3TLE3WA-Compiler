#include "3tle3wa/backend/rl/RLBasicBlock.hh"

#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/ir/IR.hh"

RLBasicBlock::RLBasicBlock(CfgNodePtr &cfg, RLPlanner *planner, RLProgress *belong_to)
    : lbidx_(cfg->GetBlockIdx()),
      successors_(),
      dominators_(),
      predecessors_(),
      live_out_(),
      live_in_(),
      live_use_(),
      live_def_(),
      uops_(),
      uops_view_(),
      planner_(planner),
      belong_to_(belong_to),
      has_tail_call_(false),
      uop_idx_bgn_(0),
      uop_idx_end_(0),
      loop_info_{.loop_time = 1, .loop_depth = 1} {
    auto &&succ_lst = cfg->GetSuccessors();
    for (auto &&succ : succ_lst) {
        successors_.insert(succ->GetBlockIdx());
    }

    auto &&domi_lst = cfg->GetDominatorSet();
    for (auto &&domi : domi_lst) {
        dominators_.insert(domi->GetBlockIdx());
    }

    auto &&pred_lst = cfg->GetPredecessors();
    for (auto &&pred : pred_lst) {
        predecessors_.insert(pred->GetBlockIdx());
    }
}

void RLBasicBlock::PushUop(UopGeneral *uop) { uops_.push_back(std::unique_ptr<UopGeneral>(uop)); }

void RLBasicBlock::TempPop() {
    tmp_.push_back(std::move(uops_.back()));
    uops_.pop_back();
}

void RLBasicBlock::RecoverUops() {
    while (not tmp_.empty()) {
        uops_.push_back(std::move(tmp_.back()));
        tmp_.pop_back();
    }
}

size_t RLBasicBlock::GetLabelIdx() const { return lbidx_; }

void RLBasicBlock::SetHasTailCall(bool on) { has_tail_call_ = on; }

bool RLBasicBlock::HasTailCall() { return has_tail_call_; }

bool RLBasicBlock::IsLiveOut(size_t vridx) { return live_out_.find(vridx) != live_out_.end(); }
