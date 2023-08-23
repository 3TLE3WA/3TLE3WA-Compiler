#pragma once

#include <algorithm>
#include <cstddef>
#include <list>
#include <unordered_map>
#include <utility>

#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/structure/loop.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/value/use.hh"

namespace LoopInvariant {

typedef std::list<InstPtr> Invariants;

static std::unordered_map<CtrlFlowGraphNode *, bool> visit;
static std::unordered_map<Instruction *, bool> is_variant;

void LoopInvariant(NormalFuncPtr);
void InvariantMotion(Loop *);
Invariants FindInvariant(Loop *);
}  // namespace LoopInvariant