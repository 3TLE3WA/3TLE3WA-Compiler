#include "3tle3wa/ir/instruction/instruction.hh"

#include <cassert>
#include <cstdint>
#include <type_traits>

#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/use.hh"

//===-----------------------------------------------------------===//
//                     Instruction Implementation
//===-----------------------------------------------------------===//

size_t Instruction::inst_idx = 1;

Instruction::Instruction(VariablePtr _res, OpCode _code, CfgNodePtr _parent)
    : idx(inst_idx++), result(_res), opcode(_code), parent(_parent) {}

const OpCode &Instruction::GetOpCode() const { return opcode; }
const size_t &Instruction::GetInstIdx() const { return idx; }
const CfgNodePtr &Instruction::GetParent() const { return parent; }
void Instruction::SetParent(CfgNodePtr node) { parent = node; }
void Instruction::ClearParent() { SetParent(nullptr); }

bool Instruction::IsTwoOprandInst() const { return (OP_ADD <= opcode && opcode <= OP_NEQ); }
bool Instruction::IsOneOprandInst() const { return (Load <= opcode && opcode <= FNeg); }

bool Instruction::IsReturnInst() const { return opcode == Ret; }
bool Instruction::IsJumpInst() const { return opcode == Jump; }
bool Instruction::IsBranchInst() const { return opcode == Branch; }

bool Instruction::IsAllocaInst() const { return opcode == Alloca; }
bool Instruction::IsLoadInst() const { return opcode == Load; }
bool Instruction::IsStoreInst() const { return opcode == Store; }
bool Instruction::IsGepInst() const { return opcode == Gep; }

bool Instruction::IsCallInst() const { return opcode == Call; }
bool Instruction::IsPhiInst() const { return opcode == Phi; }
bool Instruction::IsBitCast() const { return opcode == BitCast; }

bool Instruction::IsFNegInst() const { return opcode == FNeg; }
bool Instruction::IsFAbsInst() const { return opcode == FAbs; }

bool Instruction::IsCriticalOperation() const {
    return IsStoreInst() || IsCallInst() || IsReturnInst() || IsJumpInst() || IsBranchInst();
}

bool Instruction::IsValueNumberInst() const { return IsTwoOprandInst() || IsGepInst(); }

VariablePtr Instruction::GetResult() const { return result; }

BaseValuePtr Instruction::DoFlod() const { return nullptr; }

void Instruction::ReplaceTarget(CfgNodePtr, CfgNodePtr) { return; }

//===-----------------------------------------------------------===//
//                     UnaryInstruction Implementation
//===-----------------------------------------------------------===//

UnaryInstruction::UnaryInstruction(VariablePtr _res, OpCode _op, BaseValuePtr _opr, CfgNodePtr node)
    : Instruction(_res, _op, node), oprand(_opr) {}

BaseValuePtr UnaryInstruction::GetOprand() const { return oprand; }

BaseValuePtr UnaryInstruction::DoFlod() const { return DoUnaryFlod(opcode, oprand); }

void UnaryInstruction::RemoveResParent() { result->SetParent(nullptr); }

const BaseValueList UnaryInstruction::GetOprands() const { return BaseValueList({oprand}); }

//===-----------------------------------------------------------===//
//                     BinaryInstruction Implementation
//===-----------------------------------------------------------===//

BinaryInstruction::BinaryInstruction(VariablePtr _res, OpCode _op, BaseValuePtr _lhs, BaseValuePtr _rhs,
                                     CfgNodePtr node)
    : Instruction(_res, _op, node), lhs(_lhs), rhs(_rhs) {}

void BinaryInstruction::SetOpCode(OpCode _opcode) { opcode = _opcode; }

void BinaryInstruction::SetLHS(BaseValuePtr _lhs) { lhs = _lhs; }
BaseValuePtr BinaryInstruction::GetLHS() const { return lhs; }
void BinaryInstruction::SetRHS(BaseValuePtr _rhs) { rhs = _rhs; }
BaseValuePtr BinaryInstruction::GetRHS() const { return rhs; }

void BinaryInstruction::SwapOprand() {
    if (IsCommutative(opcode)) {
        if (lhs->IsConstant()) {
            std::swap(lhs, rhs);
        } else if (lhs->IsVariable() && rhs->IsVariable()) {
            auto l = reinterpret_cast<uint64_t>(lhs.get());
            auto r = reinterpret_cast<uint64_t>(rhs.get());
            if (l > r) {
                std::swap(lhs, rhs);
            }
        }
    }
    return;
}

bool BinaryInstruction::IsIBinaryInst() const { return false; }
bool BinaryInstruction::IsFBinaryInst() const { return false; }
bool BinaryInstruction::IsICmpInst() const { return false; }
bool BinaryInstruction::IsFCmpInst() const { return false; }

BaseValuePtr BinaryInstruction::DoFlod() const { return DoBinaryFlod(opcode, lhs, rhs); }

void BinaryInstruction::RemoveResParent() { result->SetParent(nullptr); }

const BaseValueList BinaryInstruction::GetOprands() const { return BaseValueList({lhs, rhs}); }

//===-----------------------------------------------------------===//
//                     DoFlod Implementation
//===-----------------------------------------------------------===//

BaseValuePtr DoUnaryFlod(OpCode opcode, BaseValuePtr oprand) {
    if (oprand->IsConstant()) {
        ConstType value;

        auto constant = std::static_pointer_cast<Constant>(oprand);
        std::visit(
            [&value, op = opcode](auto &&arg) {
                using T = std::decay_t<decltype(arg)>;
                assert((std::is_same_v<T, bool> || std::is_same_v<T, int32_t> || std::is_same_v<T, float>));
                switch (op) {
                    case SiToFp:
                        assert((std::is_same_v<T, bool>) || (std::is_same_v<T, int32_t>));
                        value = static_cast<float>(arg);
                        break;
                    case FpToSi:
                        assert((std::is_same_v<T, float>));
                        value = static_cast<int32_t>(arg);
                        break;
                    case Zext:
                        assert((std::is_same_v<T, bool>));
                        value = static_cast<int32_t>(arg);
                        break;
                    case FNeg:
                        assert((std::is_same_v<T, float>));
                        value = -static_cast<float>(arg);
                        break;
                    default:
                        assert(false);
                        break;
                }
            },
            constant->GetValue());

        return ConstantAllocator::FindConstantPtr(value);
    }
    return nullptr;
}

BaseValuePtr DoBinaryFlod(OpCode opcode, BaseValuePtr lhs, BaseValuePtr rhs) {
    BaseValuePtr replacer = nullptr;
    if (lhs == rhs) {
        if (opcode == OP_SUB) {
            if (lhs->GetBaseType()->IntType()) {
                return ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0));
            } else if (rhs->GetBaseType()->FloatType()) {
                return ConstantAllocator::FindConstantPtr(static_cast<float>(0));
            } else {
                assert(false);
            }
        } else if (opcode == OP_DIV) {
            if (lhs->GetBaseType()->IntType()) {
                return ConstantAllocator::FindConstantPtr(static_cast<int32_t>(1));
            } else if (rhs->GetBaseType()->FloatType()) {
                return ConstantAllocator::FindConstantPtr(static_cast<float>(1));
            } else {
                assert(false);
            }
        } else if (opcode == OP_REM) {
            if (lhs->GetBaseType()->IntType()) {
                return ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0));
            } else {
                assert(false);
            }
        }
    }

    if (lhs->IsConstant() && rhs->IsConstant()) {
        replacer = ExprFlod::BinaryOperate(opcode, std::static_pointer_cast<Constant>(lhs),
                                           std::static_pointer_cast<Constant>(rhs));
    } else if (OP_ADD <= opcode && opcode <= OP_RSHIFT) {
        // only care about arithmetic operation
        if (lhs->IsConstant()) {
            auto constant_lhs = std::static_pointer_cast<Constant>(lhs);
            std::visit(
                [&](auto &&arg) {
                    using T = std::decay_t<decltype(arg)>;
                    assert((std::is_same_v<T, int32_t> || std::is_same_v<T, float>));
                    if (opcode == OP_ADD && arg == static_cast<T>(0)) {
                        replacer = rhs;
                    } else if (opcode == OP_MUL) {
                        if (arg == static_cast<T>(0)) {
                            replacer = lhs;
                        } else if (arg == static_cast<T>(1)) {
                            replacer = rhs;
                        }
                    } else if (opcode == OP_DIV && arg == static_cast<T>(0)) {
                        replacer = lhs;
                    } else if (opcode == OP_REM && arg == static_cast<T>(0)) {
                        assert((std::is_same_v<T, int32_t>));
                        replacer = lhs;
                    }
                },
                constant_lhs->GetValue());
        }
        if (rhs->IsConstant()) {
            auto constant_rhs = std::static_pointer_cast<Constant>(rhs);
            std::visit(
                [&](auto &&arg) {
                    using T = std::decay_t<decltype(arg)>;
                    assert((std::is_same_v<T, int32_t> || std::is_same_v<T, float>));
                    if (opcode == OP_ADD && arg == static_cast<T>(0)) {
                        replacer = lhs;
                    } else if (opcode == OP_SUB && arg == static_cast<T>(0)) {
                        replacer = lhs;
                    } else if (opcode == OP_MUL) {
                        if (arg == static_cast<T>(0)) {
                            replacer = rhs;
                        } else if (arg == static_cast<T>(1)) {
                            replacer = lhs;
                        }
                    } else if (opcode == OP_DIV && arg == static_cast<T>(1)) {
                        replacer = lhs;
                    } else if (opcode == OP_REM && arg == static_cast<T>(1)) {
                        assert((std::is_same_v<T, int32_t>));
                        replacer = ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0));
                    }
                },
                constant_rhs->GetValue());
        }
    }
    return replacer;
}

//===-----------------------------------------------------------===//
//                     ReplaceSRC Implementation
//===-----------------------------------------------------------===//

void ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    if (replacee == replacer) return;
    UserList del_list;
    for (auto user : replacee->GetUserList()) {
        assert(user->ReplaceSRC(replacee, replacer));
        replacer->InsertUser(user);
        del_list.insert(user);
    }
    for (auto del : del_list) {
        replacee->RemoveUser(del);
    }
    // after replace use-count of `replacee` will be automatically deleted
}

//===-----------------------------------------------------------===//
//                     RemoveInst Implementation
//===-----------------------------------------------------------===//

void RemoveInst(InstPtr inst) {
    auto &&used_value = inst->GetOprands();
    std::for_each(used_value.begin(), used_value.end(), [&inst](BaseValuePtr value) { value->RemoveUser(inst); });
    inst->RemoveResParent();
}