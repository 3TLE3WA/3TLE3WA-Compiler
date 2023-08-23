#pragma once

#include "3tle3wa/ir/value/type/baseType.hh"

class ScalarType;
using ScalarTypePtr = std::shared_ptr<ScalarType>;

class ScalarType final : public BaseType {
   public:
    ScalarType(ATTR_TYPE, ATTR_MUTABLE, ATTR_POINTER, ATTR_SCALAR, ATTR_POSITION);

    static std::shared_ptr<ScalarType> CreatePtr(ATTR_TYPE, ATTR_MUTABLE, ATTR_POINTER, ATTR_SCALAR, ATTR_POSITION);

    std::string tollvmIR();
};

// VARIABLE USE
static const ScalarTypePtr type_void = ScalarType::CreatePtr(VOID, NONE1, NONE2, NONE3, NONE4);
static const ScalarTypePtr type_bool = ScalarType::CreatePtr(BOOL, MUTABLE, NOTPTR, SCALAR, LOCAL);
static const ScalarTypePtr type_char_ptr = ScalarType::CreatePtr(CHAR, MUTABLE, POINTER, SCALAR, LOCAL);
static const ScalarTypePtr type_int_L = ScalarType::CreatePtr(INT32, MUTABLE, NOTPTR, SCALAR, LOCAL);
static const ScalarTypePtr type_int_ptr_L = ScalarType::CreatePtr(INT32, MUTABLE, POINTER, SCALAR, LOCAL);
static const ScalarTypePtr type_float_L = ScalarType::CreatePtr(FLOAT, MUTABLE, NOTPTR, SCALAR, LOCAL);
static const ScalarTypePtr type_float_ptr_L = ScalarType::CreatePtr(FLOAT, MUTABLE, POINTER, SCALAR, LOCAL);
static const ScalarTypePtr type_int_G = ScalarType::CreatePtr(INT32, MUTABLE, NOTPTR, SCALAR, GLOBAL);
static const ScalarTypePtr type_int_ptr_G = ScalarType::CreatePtr(INT32, MUTABLE, POINTER, SCALAR, GLOBAL);
static const ScalarTypePtr type_float_G = ScalarType::CreatePtr(FLOAT, MUTABLE, NOTPTR, SCALAR, GLOBAL);
static const ScalarTypePtr type_float_ptr_G = ScalarType::CreatePtr(FLOAT, MUTABLE, POINTER, SCALAR, GLOBAL);
// CONSTANT USE
static const ScalarTypePtr type_const_bool = ScalarType::CreatePtr(BOOL, IMMUTABLE, NOTPTR, SCALAR, NONE4);
static const ScalarTypePtr type_const_char = ScalarType::CreatePtr(CHAR, IMMUTABLE, NOTPTR, SCALAR, NONE4);
static const ScalarTypePtr type_const_int = ScalarType::CreatePtr(INT32, IMMUTABLE, NOTPTR, SCALAR, NONE4);
static const ScalarTypePtr type_const_int_ptr = ScalarType::CreatePtr(INT32, IMMUTABLE, POINTER, SCALAR, NONE4);
static const ScalarTypePtr type_const_float = ScalarType::CreatePtr(FLOAT, IMMUTABLE, NOTPTR, SCALAR, NONE4);
static const ScalarTypePtr type_const_float_ptr = ScalarType::CreatePtr(FLOAT, IMMUTABLE, POINTER, SCALAR, NONE4);
static const ScalarTypePtr type_const_longlong = ScalarType::CreatePtr(INT64, IMMUTABLE, NOTPTR, SCALAR, NONE4);
// PARAMETER USE
static const ScalarTypePtr param_bool = ScalarType::CreatePtr(BOOL, MUTABLE, NOTPTR, SCALAR, PARAMETER);
static const ScalarTypePtr param_char = ScalarType::CreatePtr(CHAR, MUTABLE, NOTPTR, SCALAR, PARAMETER);
static const ScalarTypePtr param_charp = ScalarType::CreatePtr(CHAR, MUTABLE, POINTER, SCALAR, PARAMETER);
static const ScalarTypePtr param_int = ScalarType::CreatePtr(INT32, MUTABLE, NOTPTR, SCALAR, PARAMETER);
static const ScalarTypePtr param_intp = ScalarType::CreatePtr(INT32, MUTABLE, POINTER, SCALAR, PARAMETER);
static const ScalarTypePtr param_float = ScalarType::CreatePtr(FLOAT, MUTABLE, NOTPTR, SCALAR, PARAMETER);
static const ScalarTypePtr param_floatp = ScalarType::CreatePtr(FLOAT, MUTABLE, POINTER, SCALAR, PARAMETER);
static const ScalarTypePtr param_longlong = ScalarType::CreatePtr(INT64, MUTABLE, NOTPTR, SCALAR, PARAMETER);
