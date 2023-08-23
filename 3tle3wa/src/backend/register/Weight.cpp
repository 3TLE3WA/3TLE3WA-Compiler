#include <cmath>

#include "3tle3wa/backend/rl/LiveInterval.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/backend/rl/RLPlanner.hh"

void RLPlanner::CalculateWeights() {
    for (auto &&vr : vr_storage_) {
        vr->CaculateWeight();
    }
}

void VirtualRegister::CaculateWeight() {
    double base = 1.0;
    weight_ = (base * use_times_) / (ival_mgr_.SegNum() + std::sqrt(ival_mgr_.LiveSize() + 1));
}

double VirtualRegister::Weight() {
    return weight_;
}