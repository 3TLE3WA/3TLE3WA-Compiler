#pragma once

#include <list>
#include <queue>
#include <set>

#include "3tle3wa/ir/IR.hh"

namespace DCE {
void EliminateUselessCode(NormalFuncPtr);
void EliminateUselessControlFlow(NormalFuncPtr);
void EliminateUnreachableCode(NormalFuncPtr);

void DCE(NormalFuncPtr);
}  // namespace DCE