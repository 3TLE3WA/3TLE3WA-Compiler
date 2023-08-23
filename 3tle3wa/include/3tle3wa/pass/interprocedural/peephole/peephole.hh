#pragma once

#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/function/function.hh"
#include "3tle3wa/ir/symTable.hh"

namespace PeepHole {

void PeepHoleOpt(NormalFuncPtr &);
void PeepHole4Gep(NormalFuncPtr &, SymbolTable &);
};  // namespace PeepHole
