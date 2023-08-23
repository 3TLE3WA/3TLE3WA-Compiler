#include "3tle3wa/backend/rl/RLVirtualRegister.hh"

#include "3tle3wa/backend/rl/LiveInterval.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"

VirtualRegister::VirtualRegister(VREG_TYPE type, uint64_t vridx)
    : type_(type),
      virt_reg_idx_(vridx),
      real_reg_idx_(0),
      param_reg_idx_(0),
      stk_info_(nullptr),
      addr_info_(nullptr),
      on_stk_(false),
      reg_allocated_(false),
      is_param_(false),
      is_retval_(false),
      can_spill_(true),
      is_saving_(false),
      save_off_(0),
      ival_mgr_(),
      def_idx_(0),
      use_idx_(),
      use_times_(0),
      weight_(0.0) {}

void VirtualRegister::SetOnStack(bool on) { on_stk_ = on; }

void VirtualRegister::SetCanSpill(bool on) { can_spill_ = on; }

void VirtualRegister::SetIsRetval(bool on) { is_retval_ = on; }

void VirtualRegister::SetRealRegIdx(size_t rridx) {
    real_reg_idx_ = rridx;
    reg_allocated_ = true;
}

void VirtualRegister::SetParamRegIdx(size_t pos) {
    reg_allocated_ = true;
    is_param_ = true;
    param_reg_idx_ = pos;
}

void VirtualRegister::SetSavingOff(size_t off) {
    if (off == 0) {
        is_saving_ = false;
        return;
    }

    is_saving_ = true;
    save_off_ = off;
}

void VirtualRegister::SetStackInfo(StackInfo *info) { stk_info_ = info; }

void VirtualRegister::SetAddressInfo(StackInfo *info) { addr_info_ = info; }

size_t VirtualRegister::GetSaveInfo() { return save_off_; }

size_t VirtualRegister::GetVRIdx() { return virt_reg_idx_; }

size_t VirtualRegister::GetRealRegIdx() { return real_reg_idx_; }

StackInfo *VirtualRegister::GetStackInfo() { return stk_info_; }

StackInfo *VirtualRegister::GetAddressInfo() { return addr_info_; }

const std::set<Segment> &VirtualRegister::GetLiveSegs() { return ival_mgr_.Segs(); }

bool VirtualRegister::IsSaving() { return is_saving_; }

bool VirtualRegister::IsAllocated() { return reg_allocated_; }

bool VirtualRegister::IsParam() { return is_param_; }

bool VirtualRegister::IsRetval() { return is_retval_; }

bool VirtualRegister::IsOnStk() { return on_stk_; }

bool VirtualRegister::CanSpill() { return can_spill_; }

bool VirtualRegister::UseIGPR() {
    switch (type_) {
        case VREG_TYPE::FLT:
            return false;
        case VREG_TYPE::INT:
        case VREG_TYPE::PTR:
            return true;
    }
    panic("illegel");
    return false;
}

bool VirtualRegister::UseFGPR() {
    switch (type_) {
        case VREG_TYPE::FLT:
            return true;
        case VREG_TYPE::INT:
        case VREG_TYPE::PTR:
            return false;
    }
    panic("illegel");
    return false;
}

VREG_TYPE VirtualRegister::GetVRType() { return type_; }

size_t VirtualRegister::GetSize() {
    switch (type_) {
        case VREG_TYPE::FLT:
        case VREG_TYPE::INT:
            return 4;
        case VREG_TYPE::PTR:
            return 8;
    }
    panic("illegel");
    return 0;
}

void VirtualRegister::ResetRAInfo() {
    real_reg_idx_ = 0;
    reg_allocated_ = false;
    ival_mgr_.ClearAll();
}

void VirtualRegister::Def(size_t idx, size_t loopt) {
    if (def_idx_ != 0) {
        GenSegment(loopt);
    }
    def_idx_ = idx;
}

void VirtualRegister::Use(size_t idx) { use_idx_.push_back(idx); }

void VirtualRegister::GenSegment(size_t loopt) {
    if (use_idx_.empty() /* and is_param_ */) {
        use_idx_.push_back(def_idx_);
    }
    // Assert(not use_idx_.empty(), "use should not empty");
    ival_mgr_.MakeSeg(def_idx_, use_idx_.back(), this);
    use_times_ += use_idx_.size() * loopt;

    def_idx_ = 0;
    use_idx_.clear();
}

bool VirtualRegister::LiveAt(size_t idx) { return ival_mgr_.LiveAt(idx); }