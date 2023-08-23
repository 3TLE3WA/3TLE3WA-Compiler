#pragma once

#include <list>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/function/function.hh"

namespace SSA {

using ValueVector = std::vector<BaseValuePtr>;

struct RenameData;
using RenameDatePtr = std::shared_ptr<RenameData>;
struct RenameData {
    CfgNodePtr node;
    CfgNodePtr pred;
    ValueVector valuelist;

    RenameData(CfgNodePtr, CfgNodePtr, ValueVector);

    static RenameDatePtr CreatePtr(CfgNodePtr, CfgNodePtr, ValueVector);
};

void SSAConstruction(NormalFuncPtr);

void SSADestruction(NormalFuncPtr);
}  // namespace SSA
