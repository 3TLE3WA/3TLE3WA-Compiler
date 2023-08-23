#pragma once

#include <cstdint>
#include <list>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "3tle3wa/backend/Interface.hh"
#include "3tle3wa/backend/rl/Enums.hh"
#include "3tle3wa/backend/rl/LiveInterval.hh"

class VirtualRegister;
class StackInfo;
class AsmBasicBlock;
class RLProgress;
class BaseValue;
class UopCall;

using BaseValuePtr = std::shared_ptr<BaseValue>;
using ParamList = std::vector<BaseValuePtr>;

class RLPlanner final : public Serializable {
    std::unordered_map<uint64_t, StackInfo *> stk_map_;
    std::unordered_map<uint64_t, VirtualRegister *> vr_map_;

    std::list<std::unique_ptr<StackInfo>> stk_storage_;
    std::list<std::unique_ptr<VirtualRegister>> vr_storage_;

    std::vector<VirtualRegister *> params_;

    size_t stkidx_;
    size_t regidx_;

    size_t total_stack_size_;

    RLProgress *belong_to_;

    std::set<size_t> used_real_regs_;

    std::unordered_map<size_t, std::tuple<size_t, bool, VirtualRegister *>> save_stack_;

    void formatString(FILE *fp) final;

   public:
    RLPlanner(size_t alloc_from);

    void RegisterOwner(RLProgress *rlp);
    void RegisterParams(ParamList &plst);

    VirtualRegister *AllocVReg(VREG_TYPE type, uint64_t vridx);
    VirtualRegister *Alloca(uint64_t vridx, size_t len);
    StackInfo *Alloca(size_t len);
    VirtualRegister *NewVReg(VREG_TYPE type);
    VirtualRegister *GetVReg(uint64_t vridx);

    void Link(uint64_t income, uint64_t old);

    std::vector<VirtualRegister *> &Params();

    bool GraphAllocation();
    void CalculateWeights();
    void UseRealRegister(size_t rridx);
    void GenerateStackInfo();

    void ResetRAInfo();
    void GenerateCallInfo(const std::unordered_map<size_t, UopCall *> &call_map);
    void Init(AsmBasicBlock *abb);
    void Recover(size_t idx);
    size_t BeforeCall(AsmBasicBlock *abb, const std::unordered_set<VirtualRegister *> &livings);
    void RecoverCall(AsmBasicBlock *abb, RLPlanner *plan);
    void RecoverBeforeRet(AsmBasicBlock *abb);

    
};
