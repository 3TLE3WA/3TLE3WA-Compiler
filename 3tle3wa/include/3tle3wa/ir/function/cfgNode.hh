#pragma once

#include <set>

#include "3tle3wa/ir/function/basicblock.hh"
#include "3tle3wa/ir/instruction/controlFlowInst.hh"

class CtrlFlowGraphNode;
using CfgNodePtr = std::shared_ptr<CtrlFlowGraphNode>;

typedef std::list<CfgNodePtr> CfgNodeList;

typedef std::set<CfgNodePtr> Predecessor;
typedef std::set<CfgNodePtr> Successor;
typedef std::set<CfgNodePtr> DominatorSet;

class BaseFunction;

class CtrlFlowGraphNode final : public BasicBlock {
   private:
    bool dirty;

    Predecessor predecessors;
    Successor successors;

    DominatorSet dominator_set;

    CfgNodePtr immediate_dominator;
    DominatorSet dominate_children;

    DominatorSet dominance_frontier;

   public:
    CtrlFlowGraphNode(BaseFunction *, BlkAttr::BlkType blk_type = BlkAttr::Normal);
    ~CtrlFlowGraphNode() = default;

    bool GetDirty();
    void SetDirty(bool);

    void AddPredecessor(CfgNodePtr);
    void AddSuccessor(CfgNodePtr);
    void RmvPredecessor(CfgNodePtr);
    void RmvSuccessor(CfgNodePtr);
    Predecessor &GetPredecessors();
    Successor &GetSuccessors();

    void InsertDominator(CfgNodePtr);
    DominatorSet &GetDominatorSet();
    void SetDominatorSet(DominatorSet &);

    CfgNodePtr GetImmediateDominator();
    void SetImmediateDominator(CfgNodePtr);

    DominatorSet &GetDominateChildren();
    void InsertDominateChild(CfgNodePtr);

    void InsertDomFrontier(CfgNodePtr);
    DominatorSet &GetDomFrontier();

    static CfgNodePtr CreatePtr(BaseFunction *, BlkAttr::BlkType blk_type = BlkAttr::Normal);

    std::string tollvmIR();
};

void RemoveNode(CfgNodePtr node);
