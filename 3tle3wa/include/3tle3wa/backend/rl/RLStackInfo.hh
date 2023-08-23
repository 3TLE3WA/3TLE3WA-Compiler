#pragma once

#include <cstddef>
#include <cstdint>

#include "3tle3wa/backend/Interface.hh"

class VirtualRegister;

class StackInfo final {
    size_t stk_idx_;
    size_t stk_len_;
    size_t offset_;

    size_t param_offset_;
    bool is_param_;

    VirtualRegister *belong_to_;

   public:
    StackInfo(size_t sidx, size_t slen);

    size_t GetStackIdx();
    size_t GetStackLength();
    size_t GetStackOff();
    size_t GetParamStackOff();

    void SetOff(size_t off);
    void SetParamOff(size_t off);

    bool IsParam();
};