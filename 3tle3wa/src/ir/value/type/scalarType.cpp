#include "3tle3wa/ir/value/type/scalarType.hh"

ScalarType::ScalarType(ATTR_TYPE _type, ATTR_MUTABLE _mutable, ATTR_POINTER _pointer, ATTR_SCALAR _scalar,
                       ATTR_POSITION _position)
    : BaseType(_type, _mutable, _pointer, _scalar, _position) {
    // no need to check for ScalarType
}

ScalarTypePtr ScalarType::CreatePtr(ATTR_TYPE _type, ATTR_MUTABLE _mutable, ATTR_POINTER _pointer, ATTR_SCALAR _scalar,
                                    ATTR_POSITION _position) {
    return std::make_shared<ScalarType>(_type, _mutable, _pointer, _scalar, _position);
}

std::string ScalarType::tollvmIR() {
    std::stringstream ss;

    switch (attr_type) {
        case VOID:
            ss << "void";
            break;
        case BOOL:
            ss << "i1";
            break;
        case CHAR:
            ss << "i8";
            break;
        case INT32:
            ss << "i32";
            break;
        case FLOAT:
            ss << "float";
            break;
        case INT64:
            ss << "i64";
            break;
        default:
            assert(0);
            break;
    }
    if (IsPointer()) {
        ss << "*";
    }

    return ss.str();
}