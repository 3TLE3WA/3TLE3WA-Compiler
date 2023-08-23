#include "3tle3wa/ir/instruction/typeconvert.hh"

//===-----------------------------------------------------------===//
//                     SitoFpInst Implementation
//===-----------------------------------------------------------===//

SitoFpInst::SitoFpInst(VariablePtr _value1, BaseValuePtr _value2, CfgNodePtr block)
    : UnaryInstruction(_value1, SiToFp, _value2, block) {
    assert(oprand->GetBaseType()->IntType() || oprand->GetBaseType()->BoolType());
    assert(oprand->IsOprand());
}

SitoFpInstPtr SitoFpInst::CreatePtr(VariablePtr _value1, BaseValuePtr _value2, CfgNodePtr block) {
    return std::make_shared<SitoFpInst>(_value1, _value2, block);
}

VariablePtr SitoFpInst::DoSitoFp(BaseValuePtr _si, CfgNodePtr block) {
    VariablePtr _fp = Variable::CreatePtr(type_float_L, nullptr);
    auto &&inst = CreatePtr(_fp, _si, block);
    _fp->SetParent(inst);
    _si->InsertUser(inst);
    block->InsertInstBack(inst);
    return _fp;
}

bool SitoFpInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    if (oprand == replacee) {
        assert(replacer->GetBaseType()->IntType() && replacer->IsOprand());
        oprand = replacer;
        ret = true;
    }
    return ret;
}

std::string SitoFpInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = sitofp " << oprand->GetBaseType()->tollvmIR();
    ss << ' ' << oprand->tollvmIR() << " to " << result->GetBaseType()->tollvmIR();
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     FptoSiInst Implementation
//===-----------------------------------------------------------===//

FptoSiInst::FptoSiInst(VariablePtr _value1, BaseValuePtr _value2, CfgNodePtr block)
    : UnaryInstruction(_value1, FpToSi, _value2, block) {
    assert(oprand->GetBaseType()->FloatType());
    assert(oprand->IsOprand());
}

FptoSiInstPtr FptoSiInst::CreatePtr(VariablePtr _value1, BaseValuePtr _value2, CfgNodePtr block) {
    return std::make_shared<FptoSiInst>(_value1, _value2, block);
}

VariablePtr FptoSiInst::DoFptoSi(ATTR_TYPE _type, BaseValuePtr _fp, CfgNodePtr block) {
    assert(_type == BOOL || _type == INT32);
    VariablePtr _si = Variable::CreatePtr((_type == INT32) ? type_int_L : type_bool, nullptr);
    auto &&inst = CreatePtr(_si, _fp, block);
    _si->SetParent(inst);
    _fp->InsertUser(inst);
    block->InsertInstBack(inst);
    return _si;
}

bool FptoSiInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    if (oprand == replacee) {
        assert(replacer->GetBaseType()->FloatType() && replacer->IsOprand());
        oprand = replacer;
        ret = true;
    }
    return ret;
}

std::string FptoSiInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = fptosi " << oprand->GetBaseType()->tollvmIR();
    ss << ' ' << oprand->tollvmIR() << " to " << result->GetBaseType()->tollvmIR();
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     ZextInst Implementation
//===-----------------------------------------------------------===//

ZextInst::ZextInst(VariablePtr _value1, BaseValuePtr _value2, CfgNodePtr block)
    : UnaryInstruction(_value1, Zext, _value2, block) {
    assert(oprand->GetBaseType()->BoolType());
    assert(oprand->IsOprand());
}

ZextInstPtr ZextInst::CreatePtr(VariablePtr _value1, BaseValuePtr _value2, CfgNodePtr block) {
    return std::make_shared<ZextInst>(_value1, _value2, block);
}

VariablePtr ZextInst::DoZeroExt(BaseValuePtr _sv, CfgNodePtr block) {
    VariablePtr _lv = Variable::CreatePtr(type_int_L, nullptr);
    auto &&inst = CreatePtr(_lv, _sv, block);
    _lv->SetParent(inst);
    _sv->InsertUser(inst);
    block->InsertInstBack(inst);
    return _lv;
}

bool ZextInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    if (oprand == replacee) {
        assert(replacer->GetBaseType()->BoolType() && replacer->IsOprand());
        oprand = replacer;
        ret = true;
    }
    return ret;
}

std::string ZextInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = zext " << oprand->GetBaseType()->tollvmIR();
    ss << ' ' << oprand->tollvmIR() << " to " << result->GetBaseType()->tollvmIR();
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}
