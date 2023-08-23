#pragma once

#include <cinttypes>
#include <iomanip>
#include <type_traits>
#include <unordered_map>
#include <variant>

#include "3tle3wa/ir/value/baseValue.hh"

class Constant;
using ConstantPtr = std::shared_ptr<Constant>;
using ConstType = std::variant<bool, char, int32_t, float, int64_t>;

class Constant final : public BaseValue {
   private:
    ConstType value;

   public:
    Constant(ScalarTypePtr, ConstType);
    ~Constant() = default;

    const ConstType &GetValue() const;

    bool IsConstant() const;

    static ConstantPtr CreatePtr(const ConstType &value);

    std::string tollvmIR();
};

namespace ConstantAllocator {

using ConstantAllocator = std::unordered_map<ConstType, ConstantPtr>;

static ConstantAllocator constant_allocator;

const ConstantAllocator &GetConstantAllocator();

void InitConstantAllocator();

void InsertConstantPtr(const ConstType &);

ConstantPtr FindConstantPtr(const ConstType &);
}  // namespace ConstantAllocator
