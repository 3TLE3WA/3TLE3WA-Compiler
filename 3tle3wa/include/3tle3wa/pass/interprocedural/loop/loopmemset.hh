#pragma once

#include <set>
#include <utility>

#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/structure/loop.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/otherInst.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/use.hh"
#include "3tle3wa/pass/analysis/structure/structure.hh"
#include "3tle3wa/pass/interprocedural/loop/loopunrolling.hh"

namespace LoopMemset {

static BaseFuncPtr callee = nullptr;

void LoopMemset(NormalFuncPtr, CompilationUnit &);

bool LoopTreeTraversal(NormalFuncPtr func, Loop *, BaseValuePtr &, BaseValuePtr &, int &);
std::pair<BaseValuePtr, BaseValuePtr> GetArrayInit(Loop *);
bool FindInst(InstPtr, CfgNodePtr);
bool CheckLoopStep(Loop *);
}  // namespace LoopMemset