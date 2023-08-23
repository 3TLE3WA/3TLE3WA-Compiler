#include "3tle3wa/ir/instruction/unaryOpInst.hh"

#include <cassert>
#include <cstdint>
#include <memory>
#include <sstream>

#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/type/scalarType.hh"
#include "3tle3wa/ir/value/variable.hh"

//===-----------------------------------------------------------===//
//                     FNegInst Implementation
//===-----------------------------------------------------------===//

FNegInst::FNegInst(VariablePtr _res, BaseValuePtr _rhs, CfgNodePtr _node) : UnaryInstruction(_res, FNeg, _rhs, _node) {
    assert(_res->GetBaseType()->FloatType());
    assert(_rhs->GetBaseType()->FloatType());
}

FNegInstPtr FNegInst::CreatePtr(VariablePtr _res, BaseValuePtr _rhs, CfgNodePtr _node) {
    return std::make_shared<FNegInst>(_res, _rhs, _node);
}

VariablePtr FNegInst::DoFloatNeg(BaseValuePtr _rhs, CfgNodePtr _node) {
    VariablePtr _res = Variable::CreatePtr(type_float_L, nullptr);
    auto inst = CreatePtr(_res, _rhs, _node);
    _res->SetParent(inst);
    _rhs->InsertUser(inst);
    _node->InsertInstBack(inst);
    return _res;
}

bool FNegInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    if (oprand == replacee) {
        assert(replacer->GetBaseType()->FloatType() && replacer->IsOprand());
        oprand = replacer;
        ret = true;
    }
    return ret;
}

std::string FNegInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = fneg float " << oprand->tollvmIR();
    ss << "; Inst_" << GetInstIdx() << " from Block_" << parent->GetBlockIdx();
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     FAbsInst Implementation
//===-----------------------------------------------------------===//

FAbsInst::FAbsInst(VariablePtr _res, BaseValuePtr _oprand, CfgNodePtr _node)
    : UnaryInstruction(_res, FAbs, _oprand, _node) {
    assert(_res->GetBaseType()->FloatType());
    assert(_oprand->GetBaseType()->FloatType());
}

FAbsInstPtr FAbsInst::CreatePtr(VariablePtr _res, BaseValuePtr _oprand, CfgNodePtr _node) {
    return std::make_shared<FAbsInst>(_res, _oprand, _node);
}

bool FAbsInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    if (oprand == replacee) {
        assert(replacer->GetBaseType()->FloatType() && replacer->IsOprand());
        oprand = replacer;
        ret = true;
    }
    return ret;
}

std::string FAbsInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = call float @llvm.fabs.f32(float " << oprand->tollvmIR();
    ss << "); Inst_" << GetInstIdx() << " from Block_" << parent->GetBlockIdx();
    return ss.str();
}
