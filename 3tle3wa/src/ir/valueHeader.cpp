#include "3tle3wa/ir/valueHeader.hh"

#include <cstdint>
#include <memory>

#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/instruction/unaryOpInst.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/constarray.hh"
#include "3tle3wa/ir/value/type/baseType.hh"
#include "3tle3wa/ir/value/type/listType.hh"
#include "3tle3wa/ir/value/uninitvar.hh"

BaseValuePtr Value::UnaryOperate(const OpCode op, BaseValuePtr oprand, CfgNodePtr block) {
    assert(oprand->IsOprand());
    if (op == OP_ADD) return oprand;
    if (oprand->IsConstant()) {
        return ExprFlod::UnaryOperate(op, std::static_pointer_cast<Constant>(oprand));
    } else {
        ATTR_TYPE _type = oprand->GetBaseType()->GetAttrType();
        ConstantPtr zero = (_type == BOOL)    ? ConstantAllocator::FindConstantPtr(static_cast<bool>(0))
                           : (_type == INT32) ? ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))
                                              : ConstantAllocator::FindConstantPtr(static_cast<float>(0));
        if (op == OP_MINUS) {
            return BinaryOperate(OP_SUB, zero, oprand, block);
        } else if (op == OP_NOT) {
            return BinaryOperate(OP_EQU, oprand, zero, block);
        }
    }
    assert(0);
}

BaseValuePtr Value::BinaryOperate(const OpCode op, BaseValuePtr lhs, BaseValuePtr rhs, CfgNodePtr block) {
    assert(lhs->IsOprand() && rhs->IsOprand());
    if (lhs->IsConstant() && rhs->IsConstant()) {
        return ExprFlod::BinaryOperate(op, std::static_pointer_cast<Constant>(lhs),
                                       std::static_pointer_cast<Constant>(rhs));
    }
    BaseValuePtr i_lhs = lhs, f_lhs = lhs;
    BaseValuePtr i_rhs = rhs, f_rhs = rhs;

    ATTR_TYPE lhs_type = lhs->GetBaseType()->GetAttrType();
    ATTR_TYPE rhs_type = rhs->GetBaseType()->GetAttrType();

    if (OP_ADD <= op && op <= OP_RSHIFT) {
        // when do arithmetic operation, lhs_type == rhs_type in { INT32, FLOAT }
        if ((lhs_type != rhs_type) || ((lhs_type & rhs_type) == BOOL)) {
            if (lhs_type == FLOAT || rhs_type == FLOAT) {
                f_lhs = ScalarTypeConvert(FLOAT, f_lhs, block);
                f_rhs = ScalarTypeConvert(FLOAT, f_rhs, block);
                lhs_type = rhs_type = FLOAT;
            } else {
                i_lhs = ScalarTypeConvert(INT32, i_lhs, block);
                i_rhs = ScalarTypeConvert(INT32, i_rhs, block);
                lhs_type = rhs_type = INT32;
            }
        }
        assert(lhs_type == rhs_type);
        if (lhs_type == INT32) {
            return IBinaryInst::DoIBinOperate(op, i_lhs, i_rhs, block);
        } else {
            if (op == OP_SUB && f_lhs == ConstantAllocator::FindConstantPtr(static_cast<float>(0))) {
                return FNegInst::DoFloatNeg(f_rhs, block);
            } else {
                return FBinaryInst::DoFBinOperate(op, f_lhs, f_rhs, block);
            }
        }
    } else if (OP_LTH <= op && op <= OP_NEQ) {
        // when do compare operation, lhs_type == rhs_type in { BOOL, INT32, FLOAT }
        if (lhs_type != rhs_type) {
            if (lhs_type == BOOL || rhs_type == BOOL) {  // if one is BOOL, convert the other to BOOL
                i_lhs = ScalarTypeConvert(BOOL, i_lhs, block);
                i_rhs = ScalarTypeConvert(BOOL, i_rhs, block);
                lhs_type = rhs_type = BOOL;
            } else if (lhs_type == FLOAT || rhs_type == FLOAT) {
                f_lhs = ScalarTypeConvert(FLOAT, f_lhs, block);
                f_rhs = ScalarTypeConvert(FLOAT, f_rhs, block);
                lhs_type = rhs_type = FLOAT;
            }  // else, Both type are INT32
        }
        assert(lhs_type == rhs_type);
        if (lhs_type == FLOAT) {
            return FCmpInst::DoFCompare(op, f_lhs, f_rhs, block);
        } else {
            return ICmpInst::DoICompare(op, i_lhs, i_rhs, block);
        }
    }
    assert(0);
}

BaseValuePtr Value::ScalarTypeConvert(ATTR_TYPE type_convert, BaseValuePtr convertee, CfgNodePtr block) {
    assert(convertee->IsOprand());
    ATTR_TYPE type_convertee = convertee->GetBaseType()->GetAttrType();
    // if type_convert == type_convertee, no need to convert
    if (type_convert == type_convertee) {
        return convertee;
    }
    // if convertee is `CONSTANT`, use `FixType` to convert
    if (convertee->IsConstant()) {
        return Value::FixValue(type_convert, convertee);
    }
    // use instruction to convert
    if (type_convert == FLOAT) {
        // convert i1 or i32 to float
        return SitoFpInst::DoSitoFp(convertee, block);
    } else if (type_convert == INT32) {
        if (type_convertee == FLOAT) {
            // convert float to i32 or i1
            return FptoSiInst::DoFptoSi(type_convert, convertee, block);
        } else if (type_convertee == BOOL) {
            // convert i1 to i32
            return ZextInst::DoZeroExt(convertee, block);
        }
    } else {
        if (type_convertee == INT32) {
            // convert i32 to i1
            return ICmpInst::DoICompare(OP_NEQ, convertee, ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0)),
                                        block);
        } else if (type_convertee == FLOAT) {
            // convert float to i1
            return FCmpInst::DoFCompare(OP_NEQ, convertee, ConstantAllocator::FindConstantPtr(static_cast<float>(0)),
                                        block);
        }
    }
    assert(0);
}

bool Value::ValueCompare(const BaseValue *lhs, const BaseValue *rhs) {
    if (lhs == nullptr || rhs == nullptr) return false;
    if (lhs->IsConstant() && rhs->IsConstant()) {
        auto constant_lhs = static_cast<const Constant *>(lhs);
        auto constant_rhs = static_cast<const Constant *>(rhs);
        return (constant_lhs->GetBaseType()->GetAttrType() == constant_rhs->GetBaseType()->GetAttrType()) &&
               (constant_lhs->GetValue() == constant_rhs->GetValue());
    } else if (lhs->IsVariable() && rhs->IsVariable()) {
        return lhs == rhs;
    } else {
        return false;
    }
}

BaseValuePtr Value::FixValue(const ATTR_TYPE type, BaseValuePtr value) {
    if (value->IsConstant()) {
        // For Constant FixType and Return a New One
        ConstantPtr constant = nullptr;
        std::visit(
            [&type, &constant](auto &&arg) {
                using T = std::decay_t<decltype(arg)>;
                assert((std::is_same_v<T, bool> || std::is_same_v<T, int32_t> || std::is_same_v<T, float>));
                if (type == BOOL) {
                    constant = ConstantAllocator::FindConstantPtr(static_cast<bool>(arg));
                } else if (type == INT32) {
                    constant = ConstantAllocator::FindConstantPtr(static_cast<int32_t>(arg));
                } else if (type == FLOAT) {
                    constant = ConstantAllocator::FindConstantPtr(static_cast<float>(arg));
                } else
                    assert(false);
            },
            std::static_pointer_cast<Constant>(value)->GetValue());
        assert(constant != nullptr);
        return constant;
    } else if (value->IsConstArray()) {
        // For ConstArray Fix its Contents and return itself
        auto &&const_arr = std::static_pointer_cast<ConstArray>(value)->GetConstArr();
        for (size_t idx = 0, size = const_arr.size(); idx < size; ++idx) {
            auto fixed_value = FixValue(type, const_arr[idx]);
            assert(fixed_value->IsConstant());
            const_arr[idx] = std::static_pointer_cast<Constant>(fixed_value);
        }
        return value;
    } else if (value->IsGlobalValue()) {  // For GlobalValue, Fix its Init-Value and return itself
        GlobalValuePtr global_value = std::static_pointer_cast<GlobalValue>(value);
        BaseValuePtr init_value = global_value->GetInitValue();
        assert(init_value->IsConstant() || init_value->IsConstArray() || init_value->IsUnInitVar());
        global_value->SetInitValue(FixValue(type, init_value));

        if (init_value->IsConstArray()) {
            auto const_arr = std::static_pointer_cast<ConstArray>(init_value);
            bool all_zero = true;
            auto zero = (type == INT32) ? ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))
                                        : ConstantAllocator::FindConstantPtr(static_cast<float>(0));
            for (auto constant : const_arr->GetConstArr()) {
                if (constant != zero) {
                    all_zero = false;
                    break;
                }
            }
            if (all_zero) {
                global_value->SetInitValue(UnInitVar::CreatePtr(init_value->GetBaseType()));
            }
        }

        return value;
    } else {
        // for Variable or UnInitVar
        // no need to fix value
        // return itself
        return value;
    }
    // unreachable!
    assert(false);
}
