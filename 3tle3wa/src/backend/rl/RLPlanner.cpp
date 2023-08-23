#include "3tle3wa/backend/rl/RLPlanner.hh"

#include "3tle3wa/backend/asm/AsmAbi.hh"
#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLProgress.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/ir/IR.hh"

RLPlanner::RLPlanner(size_t alloc_from)
    : stk_map_(),
      vr_map_(),
      stk_storage_(),
      vr_storage_(),
      params_(),
      stkidx_(0),
      regidx_(alloc_from),
      total_stack_size_(0),
      belong_to_(nullptr),
      used_real_regs_(),
      save_stack_() {}

void RLPlanner::RegisterOwner(RLProgress *rlp) { belong_to_ = rlp; }

StackInfo *RLPlanner::Alloca(size_t len) {
    stkidx_ += 1;

    auto stkinfo = std::make_unique<StackInfo>(stkidx_, len);
    auto bptr = stkinfo.get();

    CRVC_UNUSE auto result = stk_map_.emplace(stkidx_, bptr);
    Assert(result.second, "stkinfo add failed");

    stk_storage_.push_back(std::move(stkinfo));
    return bptr;
}

VirtualRegister *RLPlanner::Alloca(uint64_t vridx, size_t len) {
    auto vr = AllocVReg(VREG_TYPE::PTR, vridx);
    auto stk = Alloca(len);

    vr->SetAddressInfo(stk);
    return vr;
}

VirtualRegister *RLPlanner::AllocVReg(VREG_TYPE type, uint64_t vridx) {
    if (auto fnd = vr_map_.find(vridx); fnd != vr_map_.end()) {
        return fnd->second;
    }

    auto vreg = std::make_unique<VirtualRegister>(type, vridx);
    auto bptr = vreg.get();

    CRVC_UNUSE auto result = vr_map_.emplace(vridx, bptr);
    Assert(result.second, "virtual register add failed");

    vr_storage_.push_back(std::move(vreg));

    return bptr;
}

VirtualRegister *RLPlanner::NewVReg(VREG_TYPE type) {
    regidx_ += 1;

    auto vreg = std::make_unique<VirtualRegister>(type, regidx_);
    auto bptr = vreg.get();

    CRVC_UNUSE auto result = vr_map_.emplace(regidx_, bptr);
    Assert(result.second, "virtual register add failed");

    vr_storage_.push_back(std::move(vreg));

    return bptr;
}

VirtualRegister *RLPlanner::GetVReg(uint64_t vridx) {
    // avoid format
    return vr_map_.at(vridx);
}

void RLPlanner::Link(uint64_t income, uint64_t old) {
    // avoid format
    vr_map_[income] = vr_map_.at(old);
}

std::vector<VirtualRegister *> &RLPlanner::Params() { return params_; }
