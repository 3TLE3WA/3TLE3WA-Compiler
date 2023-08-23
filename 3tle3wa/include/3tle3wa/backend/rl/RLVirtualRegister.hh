#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

#include "3tle3wa/backend/Interface.hh"
#include "3tle3wa/backend/rl/Enums.hh"
#include "3tle3wa/backend/rl/LiveInterval.hh"

class StackInfo;
class AsmBasicBlock;
class RLPlanner;

class VirtualRegister final : public Serializable {
    VREG_TYPE type_;

    size_t virt_reg_idx_;

    size_t real_reg_idx_;
    size_t param_reg_idx_;

    StackInfo *stk_info_;
    StackInfo *addr_info_;

    bool on_stk_;
    bool reg_allocated_;
    bool is_param_;
    bool is_retval_;
    bool can_spill_;

    bool is_saving_;
    size_t save_off_;

    LiveInterval ival_mgr_;
    size_t def_idx_;
    std::vector<size_t> use_idx_;

    size_t use_times_;
    double weight_;

    void formatString(FILE *fp) final;

   public:
    VirtualRegister(VREG_TYPE type, uint64_t vridx);

    void SetOnStack(bool on);
    void SetCanSpill(bool on);
    void SetIsRetval(bool on);

    void SetRealRegIdx(size_t idx);
    void SetParamRegIdx(size_t idx);
    void SetSavingOff(size_t off);
    void SetStackInfo(StackInfo *info);
    void SetAddressInfo(StackInfo *info);

    size_t GetSaveInfo();
    size_t GetVRIdx();
    size_t GetRealRegIdx();
    StackInfo *GetStackInfo();
    StackInfo *GetAddressInfo();
    const std::set<Segment> &GetLiveSegs();

    bool IsSaving();
    bool IsAllocated();
    bool IsParam();
    bool IsRetval();
    bool IsOnStk();

    bool CanSpill();
    bool UseIGPR();
    bool UseFGPR();

    VREG_TYPE GetVRType();
    size_t GetSize();
    void ResetRAInfo();

    void Def(size_t idx, size_t loopt);
    void Use(size_t idx);
    void GenSegment(size_t loopt);
    
    bool LiveAt(size_t idx);

    void CaculateWeight();
    double Weight();
};
