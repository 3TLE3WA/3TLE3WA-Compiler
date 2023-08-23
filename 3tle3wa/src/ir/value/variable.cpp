#include "3tle3wa/ir/value/variable.hh"

size_t Variable::var_idx = 1;

Variable::Variable(BaseTypePtr _type, InstPtr inst) : BaseValue(_type), idx(var_idx++), parent(inst) {
    // attr_type != VOID, Mutable
    assert(!base_type->VoidType() && base_type->IsMutable());
    // PARAMETER or LOCAL
    assert(base_type->IsParameter() || base_type->IsLocal());
}

void Variable::SetParent(InstPtr inst) { parent = inst; }
const InstPtr Variable::GetParent() const { return parent; }
const size_t &Variable::GetVariableIdx() const { return idx; }

bool Variable::IsVariable() const { return true; }

void Variable::ResetVarIdx() { var_idx = 1; }
size_t Variable::GetVarIdx() { return var_idx; }
void Variable::SetVarIdx(size_t _var_idx) { var_idx = _var_idx; }

VariablePtr Variable::CreatePtr(BaseTypePtr _type, InstPtr inst) { return std::make_shared<Variable>(_type, inst); }

std::string Variable::tollvmIR() {
    std::stringstream ss;
    ss << "%Reg_" << this->idx;
    return ss.str();
}