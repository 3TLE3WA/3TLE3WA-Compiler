#include "3tle3wa/ir/value/baseValue.hh"

static size_t uniquidx = 1;

BaseValue::BaseValue(BaseTypePtr _type) : base_type(_type), glb_uniq_idx_(uniquidx++) {
    // attr_type != VOID
    assert(!base_type->VoidType());
}

BaseTypePtr BaseValue::GetBaseType() const { return this->base_type; }

bool BaseValue::IsOprand() {
    return (!base_type->VoidType() && base_type->IsNotPtr() && base_type->IsScalar()) && (IsConstant() || IsVariable());
}

const InstPtr BaseValue::GetParent() const { return InstPtr(nullptr); }

bool BaseValue::IsConstant() const { return false; }
bool BaseValue::IsConstArray() const { return false; }
bool BaseValue::IsGlobalValue() const { return false; }
bool BaseValue::IsUnInitVar() const { return false; }
bool BaseValue::IsVariable() const { return false; }

UserList &BaseValue::GetUserList() { return user_list; }

void BaseValue::InsertUser(InstPtr inst) {
    if (IsConstant()) return;
    user_list.insert(inst);
}

void BaseValue::RemoveUser(InstPtr inst) { user_list.erase(inst); }

size_t BaseValue::guidx() const { return glb_uniq_idx_; }
