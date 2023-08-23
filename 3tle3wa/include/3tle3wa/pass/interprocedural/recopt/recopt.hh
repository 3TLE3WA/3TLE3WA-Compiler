#pragma once

#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/function/function.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/symTable.hh"

namespace RecursionOpt {

static std::unordered_map<BaseValue *, BaseValuePtr> value_map;

void DoRecursionOpt(NormalFuncPtr &, SymbolTable &);

void InitValueMap(SymbolTable &);

void DoTailRec2Loop(NormalFuncPtr &, SymbolTable &);
void TailRec2Loop(NormalFuncPtr &);
BaseValuePtr InstCopy(Instruction *, CfgNodePtr &);

void DoSimpleRecOpt(NormalFuncPtr &, SymbolTable &);
}  // namespace RecursionOpt