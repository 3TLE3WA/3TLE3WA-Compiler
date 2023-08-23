#include "3tle3wa/ir/instruction/otherInst.hh"

//===-----------------------------------------------------------===//
//                     CallInst Implementation
//===-----------------------------------------------------------===//

CallInst::CallInst(ScalarTypePtr _type, VariablePtr _ret, BaseFuncPtr _func, ParamList &_list, CfgNodePtr block)
    : Instruction(_ret, Call, block), ret_type(_type), callee_func(_func), rparam_list(_list), tail_call(false) {
    if (ret_type->VoidType()) {
        assert(_ret == nullptr);
    } else {
        assert(ret_type->GetAttrType() == result->GetBaseType()->GetAttrType());
        assert(result->IsOprand());
    }
    // param-type have been checked at `visitFuncRParams`
}

CallInstPtr CallInst::CreatePtr(ScalarTypePtr _type, VariablePtr _ret, BaseFuncPtr _func, ParamList &_list,
                                CfgNodePtr block) {
    return std::make_shared<CallInst>(_type, _ret, _func, _list, block);
}

BaseValuePtr CallInst::DoCallFunction(ScalarTypePtr _type, BaseFuncPtr _func, ParamList &_list, CfgNodePtr block) {
    VariablePtr _ret =
        (_type->VoidType()) ? nullptr : Variable::CreatePtr(_type->IntType() ? type_int_L : type_float_L, nullptr);
    auto &&inst = CreatePtr(_type, _ret, _func, _list, block);
    if (_ret != nullptr) {
        _ret->SetParent(inst);
    }
    std::for_each(_list.begin(), _list.end(), [&inst](const auto &param) { param->InsertUser(inst); });
    block->InsertInstBack(inst);
    return _ret;
}

ScalarTypePtr CallInst::GetRetType() const { return ret_type; }
BaseFuncPtr CallInst::GetCalleeFunc() const { return callee_func; }
ParamList &CallInst::GetParamList() { return rparam_list; }

void CallInst::SetTailCall() { tail_call = true; }
bool CallInst::GetTailCall() const { return tail_call; }

void CallInst::RemoveResParent() {
    if (result != nullptr) {
        result->SetParent(nullptr);
    }
}

bool CallInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    for (auto &&param : rparam_list) {
        if (param == replacee) {
            assert(param->GetBaseType()->GetAttrType() == replacer->GetBaseType()->GetAttrType());
            assert(param->GetBaseType()->GetAttrPointer() == replacer->GetBaseType()->GetAttrPointer());
            assert(param->GetBaseType()->GetAttrScalar() == replacer->GetBaseType()->GetAttrScalar());
            param = replacer;
            ret = true;
        }
    }
    return ret;
}

const BaseValueList CallInst::GetOprands() const {
    BaseValueList valuelist = BaseValueList();
    std::for_each(rparam_list.begin(), rparam_list.end(),
                  [&valuelist](const auto &param) { valuelist.push_back(param); });
    return valuelist;
}

std::string CallInst::tollvmIR() {
    std::stringstream ss;
    if (result != nullptr) {
        ss << result->tollvmIR() << " = ";
    }
    ss << "call " << ret_type->tollvmIR() << " @" << callee_func->GetFuncName();
    ss << "(";
    size_t rparam_size = rparam_list.size();
    if (rparam_size > 0) {
        size_t idx = 0;
        ss << rparam_list[idx]->GetBaseType()->tollvmIR() << ' ' << rparam_list[idx]->tollvmIR();
        for (idx = 1; idx < rparam_size; ++idx) {
            ss << ", " << rparam_list[idx]->GetBaseType()->tollvmIR() << ' ' << rparam_list[idx]->tollvmIR();
        }
    }
    ss << "); ";
    if (tail_call) ss << "tail-call, ";
    ss << "Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     BitCastInst Implementation
//===-----------------------------------------------------------===//

BitCastInst::BitCastInst(VariablePtr _res, BaseValuePtr _opr, CfgNodePtr block)
    : UnaryInstruction(_res, BitCast, _opr, block) {
    assert(result->GetBaseType()->CharType() && result->GetBaseType()->IsPointer());
    assert((oprand->GetBaseType()->IntType() || oprand->GetBaseType()->FloatType()) &&
           result->GetBaseType()->IsPointer());
}

BitCastInstPtr BitCastInst::CreatePtr(VariablePtr _res, BaseValuePtr _opr, CfgNodePtr block) {
    return std::make_shared<BitCastInst>(_res, _opr, block);
}

VariablePtr BitCastInst::DoBitCast(BaseValuePtr _opr, CfgNodePtr block) {
    VariablePtr _res = Variable::CreatePtr(type_char_ptr, nullptr);
    auto &&inst = CreatePtr(_res, _opr, block);
    _res->SetParent(inst);
    _opr->InsertUser(inst);
    block->InsertInstBack(inst);
    return _res;
}

bool BitCastInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    if (oprand == replacee) {
        assert((replacer->GetBaseType()->IntType() || replacer->GetBaseType()->FloatType()) &&
               replacer->GetBaseType()->IsPointer());
        oprand = replacer;
        ret = true;
    }
    return ret;
}

std::string BitCastInst::tollvmIR() {
    std::stringstream ss;

    ss << result->tollvmIR() << " = bitcast ";
    ss << oprand->GetBaseType()->tollvmIR() << ' ' << oprand->tollvmIR();
    ss << " to " << result->GetBaseType()->tollvmIR();

    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     PhiInst Implementation
//===-----------------------------------------------------------===//

PhiInst::PhiInst(VariablePtr _res, CfgNodePtr block) : Instruction(_res, Phi, block) {}

PhiInstPtr PhiInst::CreatePtr(BaseTypePtr _type, CfgNodePtr block) {
    assert(_type->IsScalar() && _type->IsNotPtr());
    VariablePtr _res = Variable::CreatePtr(_type, nullptr);
    auto &&inst = std::make_shared<PhiInst>(_res, block);
    _res->SetParent(inst);
    block->InsertInstFront(inst);
    return inst;
}

void PhiInst::SetOriginAlloca(AllocaInstPtr alloca_inst) {
    origin_alloca = alloca_inst;
    // when this alloca is stored
    // no inst use its result
    origin_alloca->GetResult()->GetUserList().clear();
}

AllocaInstPtr PhiInst::GetOriginAlloca() { return origin_alloca; }

void PhiInst::InsertPhiData(PhiInstPtr inst, BaseValuePtr _value, CfgNodePtr block) {
    assert(inst->result->GetBaseType()->GetAttrType() == _value->GetBaseType()->GetAttrType() && _value->IsOprand());
    inst->datalist.push_back({_value, block});
    _value->InsertUser(inst);
}

std::list<std::pair<BaseValuePtr, CfgNodePtr>> &PhiInst::GetRefList() { return datalist; }
const std::list<std::pair<BaseValuePtr, CfgNodePtr>> &PhiInst::GetDataList() const { return datalist; }

void PhiInst::RemoveResParent() { result->SetParent(nullptr); }

BaseValuePtr PhiInst::FindInComingUse(CfgNodePtr node) {
    for (auto [value, block] : datalist) {
        if (node == block) {
            return value;
        }
    }
    return nullptr;
}

bool PhiInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    for (auto &&pair : datalist) {
        if (replacee == pair.first) {
            assert(replacer->IsOprand());
            pair.first = replacer;
            ret = true;
        }
    }
    return ret;
}

const BaseValueList PhiInst::GetOprands() const {
    BaseValueList valuelist = BaseValueList();
    std::for_each(datalist.begin(), datalist.end(),
                  [&valuelist](const auto &pair) { valuelist.push_back(pair.first); });
    return valuelist;
}

std::string PhiInst::tollvmIR() {
    std::stringstream ss;

    ss << result->tollvmIR() << " = phi " << result->GetBaseType()->tollvmIR() << ' ';
    auto &&iter = datalist.begin();
    ss << '[' << (*iter).first->tollvmIR() << ", %Block_" << (*iter).second->GetBlockIdx() << ']';
    for (iter++; iter != datalist.end(); iter++) {
        ss << ", [" << (*iter).first->tollvmIR() << ", %Block_" << (*iter).second->GetBlockIdx() << ']';
    }

    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}
