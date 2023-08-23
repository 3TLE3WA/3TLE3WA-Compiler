#pragma once

#include <vector>

#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"

class ConstArray;
using ConstArrayPtr = std::shared_ptr<ConstArray>;
using ConstArr = std::vector<ConstantPtr>;

// both global-const-array or local-const-array
// will be put in .data section
class ConstArray final : public BaseValue {
   private:
    ConstArr const_arr;

   public:
    ConstArray(ListTypePtr, ConstArr &);
    ~ConstArray() = default;

    bool IsConstArray() const;

    ConstArr &GetConstArr();

    static ConstArrayPtr CreatePtr(ListTypePtr, ConstArr &);

    std::string tollvmIR();
};