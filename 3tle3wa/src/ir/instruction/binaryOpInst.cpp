#include "3tle3wa/ir/instruction/binaryOpInst.hh"

#include <cstdint>

#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/value/constant.hh"

//===-----------------------------------------------------------===//
//                     IBinaryInst Implementation
//===-----------------------------------------------------------===//

IBinaryInst::IBinaryInst(VariablePtr _res, OpCode _op, BaseValuePtr _lhs, BaseValuePtr _rhs, CfgNodePtr block)
    : BinaryInstruction(_res, _op, _lhs, _rhs, block) {
    assert(lhs->GetBaseType()->IntType() && lhs->IsOprand());
    assert(rhs->GetBaseType()->IntType() && rhs->IsOprand());
}

bool IBinaryInst::IsIBinaryInst() const { return true; }

bool IBinaryInst::IsIntegerNeg() const {
    return (opcode == OP_SUB) &&
           (lhs->IsConstant() && lhs == ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0)));
}

IBinaryInstPtr IBinaryInst::CreatePtr(VariablePtr _res, OpCode _op, BaseValuePtr _lhs, BaseValuePtr _rhs,
                                      CfgNodePtr block) {
    return std::make_shared<IBinaryInst>(_res, _op, _lhs, _rhs, block);
}

VariablePtr IBinaryInst::DoIBinOperate(OpCode _op, BaseValuePtr _lhs, BaseValuePtr _rhs, CfgNodePtr block) {
    VariablePtr _res = Variable::CreatePtr(type_int_L, nullptr);
    auto &&inst = CreatePtr(_res, _op, _lhs, _rhs, block);
    _res->SetParent(inst);
    _lhs->InsertUser(inst);
    _rhs->InsertUser(inst);
    block->InsertInstBack(inst);
    return _res;
}

bool IBinaryInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    assert(replacer->GetBaseType()->IntType() && replacer->IsOprand());
    bool ret = false;
    if (replacee == lhs) {
        lhs = replacer;
        ret = true;
    }
    if (replacee == rhs) {
        rhs = replacer;
        ret = true;
    }
    return ret;
}

std::string IBinaryInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = ";
    switch (opcode) {
        case OP_ADD:
            ss << "add";
            break;
        case OP_SUB:
            ss << "sub";
            break;
        case OP_MUL:
            ss << "mul";
            break;
        case OP_DIV:
            ss << "sdiv";
            break;
        case OP_REM:
            ss << "srem";
            break;
        default:
            assert(0);
            break;
    }
    ss << " i32 " << lhs->tollvmIR() << ", " << rhs->tollvmIR();
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     FBinaryInst Implementation
//===-----------------------------------------------------------===//

FBinaryInst::FBinaryInst(VariablePtr _res, OpCode _op, BaseValuePtr _lhs, BaseValuePtr _rhs, CfgNodePtr block)
    : BinaryInstruction(_res, _op, _lhs, _rhs, block) {
    assert(lhs->GetBaseType()->FloatType() && lhs->IsOprand());
    assert(rhs->GetBaseType()->FloatType() && rhs->IsOprand());
}

bool FBinaryInst::IsFBinaryInst() const { return true; }

FBinaryInstPtr FBinaryInst::CreatePtr(VariablePtr _res, OpCode _op, BaseValuePtr _lhs, BaseValuePtr _rhs,
                                      CfgNodePtr block) {
    return std::make_shared<FBinaryInst>(_res, _op, _lhs, _rhs, block);
}

VariablePtr FBinaryInst::DoFBinOperate(OpCode _op, BaseValuePtr _lhs, BaseValuePtr _rhs, CfgNodePtr block) {
    VariablePtr _res = Variable::CreatePtr(type_float_L, nullptr);
    auto &&inst = CreatePtr(_res, _op, _lhs, _rhs, block);
    _res->SetParent(inst);
    _lhs->InsertUser(inst);
    _rhs->InsertUser(inst);
    block->InsertInstBack(inst);
    return _res;
}

bool FBinaryInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    assert(replacer->GetBaseType()->FloatType() && replacer->IsOprand());
    bool ret = false;
    if (replacee == lhs) {
        lhs = replacer;
        ret = true;
    }
    if (replacee == rhs) {
        rhs = replacer;
        ret = true;
    }
    return ret;
}

std::string FBinaryInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = ";
    switch (opcode) {
        case OP_ADD:
            ss << "fadd";
            break;
        case OP_SUB:
            ss << "fsub";
            break;
        case OP_MUL:
            ss << "fmul";
            break;
        case OP_DIV:
            ss << "fdiv";
            break;
        default:
            assert(0);
            break;
    }
    ss << " float " << lhs->tollvmIR() << ", " << rhs->tollvmIR();
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}
