#pragma once

#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/value/baseValue.hh"

namespace InstComb {

enum BinType {
    LVRV,  // variable op variable
    LVRC,  // variable op constant
    LCRV,  // constant op variable
    LCRC,  // constant op constant
};

BinType GetBinType(const BaseValue *, const BaseValue *);

void InstCombine(NormalFuncPtr);

}  // namespace InstComb
