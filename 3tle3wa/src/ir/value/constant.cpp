#include "3tle3wa/ir/value/constant.hh"

Constant::Constant(ScalarTypePtr _type, ConstType _value) : BaseValue(_type), value(_value) {
    // NONE-VOID
    assert(!base_type->VoidType());
    // Immutable, NotPtr, Scalar
    assert(base_type->IsImMutable() && base_type->IsNotPtr() && base_type->IsScalar());
}

const ConstType &Constant::GetValue() const { return this->value; }

bool Constant::IsConstant() const { return true; }

ConstantPtr Constant::CreatePtr(const ConstType &value) {
    ConstantPtr constant = nullptr;
    std::visit(
        [&constant, &value](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, bool>) {
                constant = std::make_shared<Constant>(type_const_bool, value);
            } else if constexpr (std::is_same_v<T, char>) {
                constant = std::make_shared<Constant>(type_const_char, value);
            } else if constexpr (std::is_same_v<T, int32_t>) {
                constant = std::make_shared<Constant>(type_const_int, value);
            } else if constexpr (std::is_same_v<T, float>) {
                constant = std::make_shared<Constant>(type_const_float, value);
            } else if constexpr (std::is_same_v<T, int64_t>) {
                constant = std::make_shared<Constant>(type_const_longlong, value);
            } else {
                assert(false);
            }
        },
        value);
    return constant;
}

std::string Constant::tollvmIR() {
    std::stringstream ss;

    BaseTypePtr base_type = this->GetBaseType();
    std::visit(
        [&ss, base_type](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, float>) {
                assert(base_type->FloatType());
                double double_value = arg;
                union {
                    double f64;
                    uint64_t iu64;
                } cvt = {.f64 = double_value};
                uint64_t uint64_value = cvt.iu64;
                char buf[20];
                snprintf(buf, 20, "0x%016" PRIx64, uint64_value);
                ss << buf;
            } else {
                ss << static_cast<int64_t>(arg);
            }
        },
        value);

    return ss.str();
}

const ConstantAllocator::ConstantAllocator &ConstantAllocator::GetConstantAllocator() { return constant_allocator; }

void ConstantAllocator::InitConstantAllocator() {
    constant_allocator[static_cast<bool>(0)] = Constant::CreatePtr(static_cast<bool>(0));
    constant_allocator[static_cast<bool>(1)] = Constant::CreatePtr(static_cast<bool>(1));
    constant_allocator[static_cast<char>(0)] = Constant::CreatePtr(static_cast<char>(0));
    constant_allocator[static_cast<char>(-1)] = Constant::CreatePtr(static_cast<char>(-1));
    constant_allocator[static_cast<int32_t>(0)] = Constant::CreatePtr(static_cast<int32_t>(0));
    constant_allocator[static_cast<int32_t>(-1)] = Constant::CreatePtr(static_cast<int32_t>(-1));
    constant_allocator[static_cast<float>(0)] = Constant::CreatePtr(static_cast<float>(0));
}

void ConstantAllocator::InsertConstantPtr(const ConstType &constant) {
    assert(constant_allocator.count(constant) == 0);
    ConstantPtr constant_ptr = Constant::CreatePtr(constant);
    constant_allocator[constant] = constant_ptr;
}

ConstantPtr ConstantAllocator::FindConstantPtr(const ConstType &constant) {
    if (constant_allocator.count(constant) == 0) {
        InsertConstantPtr(constant);
    }
    return constant_allocator[constant];
}
