#pragma once

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "3tle3wa/backend/Interface.hh"
#include "3tle3wa/backend/rl/Enums.hh"

class RLPlanner;
class RLBasicBlock;
class VirtualRegister;
class Variable;
class AsmProgress;
class BaseValue;
class InterferenceGraph;
class UopCall;

using BaseValuePtr = std::shared_ptr<BaseValue>;
using ParamList = std::vector<BaseValuePtr>;

class RLProgress final : public Serializable {
    char *label_;

    struct call_information {
        bool call_libfunc;
        bool call_self;
        bool call_func;
    } call_info_;

    std::unique_ptr<RLPlanner> planner_;

    std::list<std::unique_ptr<RLBasicBlock>> rlbbs_;

    std::unordered_map<size_t, RLBasicBlock *> lbmap_;

    void formatString(FILE *fp) final;

    void computeLivenessInfomation();
    void computeLiveInterval();
    void cleanAll();
    bool registerAllocation();
    void rewrite();

   public:
    RLProgress(const std::string &name);
    ~RLProgress();

    void RegisterPlanner(std::unique_ptr<RLPlanner> planner);
    void RegisterParams(ParamList &plst);
    void RegisterBasicBlock(std::unique_ptr<RLBasicBlock> rlbb, bool tail_call);

    void MeetCall();
    void MeetLibCall();
    void MeetCallOther();
    void MeetCallSelf();

    void DoFIFOSchedule();
    void DoFastUseSchedule();
    void DoAssignment();
    void DoToAsm(AsmProgress *apg);

    void SetMergable(InterferenceGraph &ig);

    RLBasicBlock *FindBlkById(size_t lbidx);
};