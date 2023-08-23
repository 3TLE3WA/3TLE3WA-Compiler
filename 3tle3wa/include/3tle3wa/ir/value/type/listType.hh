#pragma once

#include <vector>

#include "3tle3wa/ir/value/type/baseType.hh"

class ListType;
using ListTypePtr = std::shared_ptr<ListType>;
using ArrDims = std::vector<size_t>;

constexpr size_t ARR_SIZE_ALIGN = 4;

class ListType final : public BaseType {
   private:
    ArrDims dims;
    size_t capacity;

   public:
    ListType(ATTR_TYPE, ATTR_MUTABLE, ATTR_POINTER, ATTR_SCALAR, ATTR_POSITION, ArrDims &);

    size_t GetArrSize() const;

    const ArrDims GetDimSize() const;

    const ArrDims GetArrDims() const;

    size_t GetCapacity() const;

    static ListTypePtr CreatePtr(ATTR_TYPE, ATTR_MUTABLE, ATTR_POINTER, ATTR_SCALAR, ATTR_POSITION, ArrDims &);

    std::string tollvmIR();
};