#pragma once

#include <memory>
#include <variant>

#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/type/baseType.hh"
#include "3tle3wa/ir/value/type/scalarType.hh"

namespace ExprFlod {

BaseValuePtr UnaryOperate(const OpCode, const ConstantPtr);

BaseValuePtr BinaryOperate(const OpCode, const ConstantPtr, const ConstantPtr);
}  // namespace ExprFlod