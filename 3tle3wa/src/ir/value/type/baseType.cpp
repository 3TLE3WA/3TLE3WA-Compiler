#include "3tle3wa/ir/value/type/baseType.hh"

BaseType::BaseType(ATTR_TYPE _type, ATTR_MUTABLE _mutable, ATTR_POINTER _pointer, ATTR_SCALAR _scalar,
                   ATTR_POSITION _position)
    : attr_type(_type),
      attr_mutable(_mutable),
      attr_pointer(_pointer),
      attr_scalar(_scalar),
      attr_position(_position) {}

bool BaseType::VoidType() const { return (attr_type == VOID); }
bool BaseType::BoolType() const { return (attr_type == BOOL); }
bool BaseType::CharType() const { return (attr_type == CHAR); }
bool BaseType::IntType() const { return (attr_type == INT32); }
bool BaseType::FloatType() const { return (attr_type == FLOAT); }

bool BaseType::IsMutable() const { return (attr_mutable == MUTABLE); }
bool BaseType::IsImMutable() const { return (attr_mutable == IMMUTABLE); }

bool BaseType::IsPointer() const { return (attr_pointer == POINTER); }
bool BaseType::IsNotPtr() const { return (attr_pointer == NOTPTR); }

bool BaseType::IsScalar() const { return (attr_scalar == SCALAR); }
bool BaseType::IsArray() const { return (attr_scalar == ARRAY); }

bool BaseType::IsLocal() const { return (attr_position == LOCAL); }
bool BaseType::IsParameter() const { return (attr_position == PARAMETER); }
bool BaseType::IsGlobal() const { return (attr_position == GLOBAL); }

ATTR_TYPE BaseType::GetAttrType() const { return attr_type; }

ATTR_MUTABLE BaseType::GetAttrMutable() const { return attr_mutable; }

ATTR_POINTER BaseType::GetAttrPointer() const { return attr_pointer; }

ATTR_SCALAR BaseType::GetAttrScalar() const { return attr_scalar; }
