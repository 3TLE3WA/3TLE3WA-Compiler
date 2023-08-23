#pragma once

#include <unordered_map>

#include "3tle3wa/backend/rl/RLGen.hh"
#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/function/function.hh"
#include "3tle3wa/ir/symTable.hh"

namespace Inline {

static std::unordered_map<CfgNodePtr, CfgNodePtr> block_map;
static std::unordered_map<BaseValuePtr, BaseValuePtr> value_map;

BaseValuePtr InstCopy(InstPtr &, CfgNodePtr &);
std::pair<BaseValuePtr, CfgNodePtr> Inline(NormalFuncPtr &, NormalFunction *, ParamList &, NameValueMap &, CfgNodePtr &,
                                           bool, CfgNodePtr &);
void InlineOptFunc(NormalFuncPtr &, SymbolTable &);
}  // namespace Inline