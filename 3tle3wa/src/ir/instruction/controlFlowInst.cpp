#include "3tle3wa/ir/instruction/controlFlowInst.hh"

//===-----------------------------------------------------------===//
//                     ReturnInst Implementation
//===-----------------------------------------------------------===//

ReturnInst::ReturnInst(ScalarTypePtr _type, BaseValuePtr _value, CfgNodePtr block)
    : Instruction(nullptr, Ret, block), ret_type(_type), ret_value(_value) {
    if (ret_type->VoidType()) {
        assert(_value == nullptr);
    } else {
        assert(ret_type->GetAttrType() == ret_value->GetBaseType()->GetAttrType());
        assert(ret_value->IsOprand());
    }
}

RetInstPtr ReturnInst::CreatePtr(ScalarTypePtr _type, BaseValuePtr _value, CfgNodePtr block) {
    auto &&inst = std::make_shared<ReturnInst>(_type, _value, block);
    if (_value != nullptr) _value->InsertUser(inst);
    return inst;
}

ScalarTypePtr ReturnInst::GetRetType() const { return ret_type; }
BaseValuePtr ReturnInst::GetRetValue() const { return ret_value; }

void ReturnInst::RemoveResParent() { return; }

bool ReturnInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    assert((ret_type->GetAttrType() == replacer->GetBaseType()->GetAttrType()) && replacer->IsOprand());
    if (replacee == ret_value) {
        ret_value = replacer;
        return true;
    }
    return false;
}

const BaseValueList ReturnInst::GetOprands() const {
    BaseValueList valuelist = BaseValueList();
    if (!ret_type->VoidType()) valuelist.push_back(ret_value);
    return valuelist;
}

std::string ReturnInst::tollvmIR() {
    std::stringstream ss;
    if (ret_type->VoidType()) {
        ss << "ret void";
    } else {
        ss << "ret " << ret_type->tollvmIR() << ' ' << ret_value->tollvmIR();
    }
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     JumpInst Implementation
//===-----------------------------------------------------------===//

JumpInst::JumpInst(CfgNodePtr _dest, CfgNodePtr block) : Instruction(nullptr, Jump, block), dest(_dest) {
    assert(block != nullptr);
    if (dest != nullptr) {
        parent->AddSuccessor(dest);
        dest->AddPredecessor(parent);
    }
}

JumpInstPtr JumpInst::CreatePtr(CfgNodePtr _dest, CfgNodePtr _block) {
    return std::make_shared<JumpInst>(_dest, _block);
}

void JumpInst::SetTarget(CfgNodePtr _dest) {
    assert(dest == nullptr);
    dest = _dest;
    parent->AddSuccessor(dest);
    dest->AddPredecessor(parent);
}

CfgNodePtr JumpInst::GetTarget() { return dest; }

void JumpInst::ReplaceTarget(CfgNodePtr i, CfgNodePtr j) {
    if (dest == i) {
        dest = j;
    } else {
        assert(false);
    }
}

void JumpInst::RemoveResParent() { return; }

bool JumpInst::ReplaceSRC(CRVC_UNUSE BaseValuePtr replacee, CRVC_UNUSE BaseValuePtr replacer) { return false; }

const BaseValueList JumpInst::GetOprands() const { return BaseValueList(); }

std::string JumpInst::tollvmIR() {
    std::stringstream ss;
    ss << "br label %Block_" << dest->GetBlockIdx();
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     BranchInst Implementation
//===-----------------------------------------------------------===//

BranchInst::BranchInst(BaseValuePtr _cond, CfgNodePtr _br1, CfgNodePtr _br2, CfgNodePtr block)
    : Instruction(nullptr, Branch, block), cond(_cond), iftrue(_br1), iffalse(_br2) {
    assert(cond->GetBaseType()->BoolType() && cond->IsOprand());
    assert(parent != nullptr);
    if (iftrue != nullptr) {
        parent->AddSuccessor(iftrue);
        iftrue->AddPredecessor(parent);
    }
    if (iffalse != nullptr) {
        parent->AddSuccessor(iffalse);
        iffalse->AddPredecessor(parent);
    }
}

BranchInstPtr BranchInst::CreatePtr(BaseValuePtr _cond, CfgNodePtr _br1, CfgNodePtr _br2, CfgNodePtr block) {
    auto &&inst = std::make_shared<BranchInst>(_cond, _br1, _br2, block);
    _cond->InsertUser(inst);
    return inst;
}

void BranchInst::SetTrueTarget(CfgNodePtr _iftrue) {
    assert(iftrue == nullptr);
    iftrue = _iftrue;
    parent->AddSuccessor(iftrue);
    iftrue->AddPredecessor(parent);
}

void BranchInst::SetFalseTarget(CfgNodePtr _iffalse) {
    assert(iffalse == nullptr);
    iffalse = _iffalse;
    parent->AddSuccessor(iffalse);
    iffalse->AddPredecessor(parent);
}

BaseValuePtr BranchInst::GetCondition() { return cond; }
CfgNodePtr BranchInst::GetTrueTarget() { return iftrue; }
CfgNodePtr BranchInst::GetFalseTarget() { return iffalse; }

void BranchInst::ReplaceTarget(CfgNodePtr i, CfgNodePtr j) {
    bool replace = false;
    if (iftrue == i) {
        iftrue = j;
        replace = true;
    }
    if (iffalse == i) {
        iffalse = j;
        replace = true;
    }
    assert(replace);
}

void BranchInst::RemoveResParent() { return; }

bool BranchInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    assert(replacer->GetBaseType()->BoolType() && replacer->IsOprand());
    if (replacee == cond) {
        cond = replacer;
        return true;
    }
    return false;
}

const BaseValueList BranchInst::GetOprands() const { return BaseValueList({cond}); }

std::string BranchInst::tollvmIR() {
    std::stringstream ss;
    ss << "br i1 " << cond->tollvmIR() << ", ";
    ss << "label %Block_" << iftrue->GetBlockIdx() << ", ";
    ss << "label %Block_" << iffalse->GetBlockIdx();
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}
