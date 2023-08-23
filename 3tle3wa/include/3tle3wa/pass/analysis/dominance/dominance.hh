#pragma once

#include <list>
#include <queue>

#include "3tle3wa/ir/IR.hh"

namespace Dominance {
void ComputeDominanceInfo(CfgNodePtr, CfgNodeList);
void ComputeDominanceFrontier(CfgNodeList);

void DominanceAnalysis(NormalFuncPtr);
}  // namespace Dominance
