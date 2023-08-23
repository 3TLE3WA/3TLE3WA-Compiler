#include "3tle3wa/ir/value/type/listType.hh"

ListType::ListType(ATTR_TYPE _type, ATTR_MUTABLE _mutable, ATTR_POINTER _pointer, ATTR_SCALAR _scalar,
                   ATTR_POSITION _position, ArrDims &_dims)
    : BaseType(_type, _mutable, _pointer, _scalar, _position), dims(_dims) {
    // INT32 || FLOAT
    assert(IntType() || FloatType());
    // ARRAY
    assert(IsArray());
    capacity =
        GetArrSize() + ((GetArrSize() % ARR_SIZE_ALIGN == 0) ? 0 : ARR_SIZE_ALIGN - GetArrSize() % ARR_SIZE_ALIGN);
}

size_t ListType::GetArrSize() const {
    size_t size = 1;
    for (auto &&dim : dims) {
        size *= dim;
    }
    return size;
}

const ArrDims ListType::GetDimSize() const {
    ArrDims ret;
    size_t size = 1;
    for (size_t idx = dims.size() - 1; idx < dims.size(); --idx) {
        ret.push_back(size);
        size *= dims[idx];
    }
    std::sort(ret.rbegin(), ret.rend());
    return ret;
}

const ArrDims ListType::GetArrDims() const { return dims; }

size_t ListType::GetCapacity() const { return capacity; }

ListTypePtr ListType::CreatePtr(ATTR_TYPE _type, ATTR_MUTABLE _mutable, ATTR_POINTER _pointer, ATTR_SCALAR _scalar,
                                ATTR_POSITION _position, ArrDims &_dims) {
    return std::make_shared<ListType>(_type, _mutable, _pointer, _scalar, _position, _dims);
}

std::string ListType::tollvmIR() {
    std::stringstream ss;

    ss << '[' << capacity << " x ";
    switch (attr_type) {
        case INT32:
            ss << "i32";
            break;
        case FLOAT:
            ss << "float";
            break;
        default:
            assert(0);
            break;
    }
    ss << ']';
    if (attr_pointer == POINTER) {
        ss << "*";
    }

    return ss.str();
}