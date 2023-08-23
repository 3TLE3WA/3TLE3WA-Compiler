#include "3tle3wa/ir/exprFlod.hh"

// Constant do UnaryOperate
// must have type in { BOOL, INT32, FLOAT }
BaseValuePtr ExprFlod::UnaryOperate(const OpCode op, const ConstantPtr oprand) {
    ConstType value;

    std::visit(
        [&value, oper = op](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            assert((std::is_same_v<T, bool> || std::is_same_v<T, int32_t> || std::is_same_v<T, float>));
            switch (oper) {
                case OP_MINUS:
                    value = -arg;
                    return;
                case OP_NOT:
                    value = static_cast<bool>(!arg);
                    return;
                default:
                    assert(false);
            }
        },
        oprand->GetValue());

    return ConstantAllocator::FindConstantPtr(value);
}

// Constant do BinaryOperate
// must have type in { BOOL, INT32, FLOAT }
BaseValuePtr ExprFlod::BinaryOperate(const OpCode op, const ConstantPtr lhs, const ConstantPtr rhs) {
    ConstType value;

    std::visit(
        [&value, oper = op](auto &&l, auto &&r) {
            using type_l = std::decay_t<decltype(l)>;
            using type_r = std::decay_t<decltype(r)>;
            assert((std::is_same_v<type_l, bool> || std::is_same_v<type_l, int32_t> || std::is_same_v<type_l, float>));
            assert((std::is_same_v<type_r, bool> || std::is_same_v<type_r, int32_t> || std::is_same_v<type_r, float>));

            constexpr bool returns_float = std::is_same_v<type_l, float> || std::is_same_v<type_r, float>;

            switch (oper) {
                case OP_ADD:
                    value = l + r;
                    return;
                case OP_SUB:
                    value = l - r;
                    return;
                case OP_MUL:
                    value = l * r;
                    return;
                case OP_DIV:
                    value = l / r;
                    return;
                case OP_REM:
                    if constexpr (returns_float) {
                        assert(false);
                    } else {
                        value = l % r;
                    }
                    return;
                case OP_LTH:
                    value = static_cast<bool>(l < r);
                    return;
                case OP_LEQ:
                    value = static_cast<bool>(l <= r);
                    return;
                case OP_GTH:
                    value = static_cast<bool>(l > r);
                    return;
                case OP_GEQ:
                    value = static_cast<bool>(l >= r);
                    return;
                case OP_EQU:
                    value = static_cast<bool>(l == r);
                    return;
                case OP_NEQ:
                    value = static_cast<bool>(l != r);
                    return;
                default:
                    assert(false);
            }
        },
        lhs->GetValue(), rhs->GetValue());

    return ConstantAllocator::FindConstantPtr(value);
}