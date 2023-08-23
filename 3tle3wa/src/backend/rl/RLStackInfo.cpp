#include "3tle3wa/backend/rl/RLStackInfo.hh"

StackInfo::StackInfo(size_t sidx, size_t slen)
    : stk_idx_(sidx), stk_len_(slen), offset_(0), param_offset_(0), is_param_(false) {}

size_t StackInfo::GetStackIdx() { return stk_idx_; }

size_t StackInfo::GetStackLength() { return stk_len_; }

size_t StackInfo::GetStackOff() {
    return offset_;
}

size_t StackInfo::GetParamStackOff() { return param_offset_; }

void StackInfo::SetOff(size_t off) { offset_ = off; }

void StackInfo::SetParamOff(size_t off) {
    is_param_ = true;
    param_offset_ = off;
}

bool StackInfo::IsParam() { return is_param_; }
