#pragma once

#include <unordered_map>

#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/function.hh"
#include "3tle3wa/ir/function/structure/loop.hh"

namespace StructureAnalysis {

// use cond-begin as key
static std::unordered_map<CtrlFlowGraphNode *, Loop *> Node2Loop;
static std::unordered_map<CtrlFlowGraphNode *, Branch_ *> Node2Branch;

Loop *FindInNode2Loop(CtrlFlowGraphNode *);
void RmvLoopInNode2Loop(Loop *);

void LoopAnalysis(NormalFuncPtr &);
void BranchAnalysis(NormalFuncPtr &);
}  // namespace StructureAnalysis