#pragma once

#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/symTable.hh"

namespace HoistLocalArray {

void HoistLocalArray(NormalFuncPtr &, SymbolTable &);
}  // namespace HoistLocalArray