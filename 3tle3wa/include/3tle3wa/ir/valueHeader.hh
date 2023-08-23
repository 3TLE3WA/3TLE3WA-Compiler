#pragma once

#include <memory>

#include "3tle3wa/ir/exprFlod.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instHeader.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/constarray.hh"
#include "3tle3wa/ir/value/globalvalue.hh"
#include "3tle3wa/ir/value/uninitvar.hh"
#include "3tle3wa/ir/value/variable.hh"

namespace Value {

BaseValuePtr UnaryOperate(const OpCode, BaseValuePtr, CfgNodePtr);

BaseValuePtr BinaryOperate(const OpCode, BaseValuePtr, BaseValuePtr, CfgNodePtr);

BaseValuePtr ScalarTypeConvert(ATTR_TYPE, BaseValuePtr, CfgNodePtr);

bool ValueCompare(const BaseValue *, const BaseValue *);

BaseValuePtr FixValue(const ATTR_TYPE, BaseValuePtr);

}  // namespace Value
