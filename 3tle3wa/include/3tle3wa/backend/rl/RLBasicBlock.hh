#pragma once

#include <list>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "3tle3wa/backend/Interface.hh"

class UopGeneral;
class VirtualRegister;
class CtrlFlowGraphNode;
class RLPlanner;
class RLProgress;
class AsmBasicBlock;
class InterferenceGraph;
class UopCall;

using CfgNodePtr = std::shared_ptr<CtrlFlowGraphNode>;

class RLBasicBlock final : public Serializable {
    size_t lbidx_;

    std::set<size_t> successors_;
    std::set<size_t> dominators_;
    std::set<size_t> predecessors_;

    std::set<size_t> live_out_;
    std::set<size_t> live_in_;
    std::set<size_t> live_use_;
    std::set<size_t> live_def_;

    std::list<std::unique_ptr<UopGeneral>> uops_;

    std::list<std::unique_ptr<UopGeneral>> tmp_;

    std::list<UopGeneral *> uops_view_;

    RLPlanner *planner_;
    RLProgress *belong_to_;

    bool has_tail_call_;

    size_t uop_idx_bgn_;
    size_t uop_idx_end_;

    struct LoopInfo {
        size_t loop_time;
        size_t loop_depth;
    } loop_info_;

    void formatString(FILE *fp) final;

   public:
    RLBasicBlock(CfgNodePtr &cfg, RLPlanner *planner, RLProgress *belong_to);

    void PushUop(UopGeneral *uop);
    void TempPop();
    void RecoverUops();

    size_t GetLabelIdx() const;

    void SetHasTailCall(bool on);
    bool HasTailCall();

    bool IsLiveOut(size_t vridx);

    void FIFOSchedule();
    void FastUseSchedule();

    // for liveness calculation
    void InitCalculation();
    void ClearLiveSet();
    void SetLiveDefParams(std::vector<VirtualRegister *> &params);
    void SetLiveinParams(std::vector<VirtualRegister *> &params);
    bool CalculateLiveInOut(std::unordered_map<size_t, RLBasicBlock *> &lbmap);

    // for live interval calculation
    void IndexAllUops(size_t &begin);
    void CalculateInterval();

    void Rewrite();
    std::list<UopGeneral *>::iterator InsertToView(std::list<UopGeneral *>::iterator it, UopGeneral *uop);

    void ToAsm(AsmBasicBlock *abb, RLPlanner *plan, const std::unordered_set<size_t> &last_call);
    void GetCallMapInfo(std::unordered_map<size_t, UopCall *> &call_map, std::unordered_set<size_t> &last_call);
    void SetMergable(InterferenceGraph &ig);

};