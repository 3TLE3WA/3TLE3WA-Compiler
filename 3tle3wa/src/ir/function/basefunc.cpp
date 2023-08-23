#include "3tle3wa/ir/function/basefunc.hh"

//===-----------------------------------------------------------===//
//                     BaseFunction Implementation
//===-----------------------------------------------------------===//

BaseFunction::BaseFunction(ScalarTypePtr _type, std::string &_name, ParamList &_list, bool _side_effect)
    : ret_type(_type), func_name(_name), param_list(_list), side_effect(_side_effect), recursive(false) {}

bool BaseFunction::GetSideEffect() const { return side_effect; }
void BaseFunction::SetSideEffect(bool _bool) { side_effect = _bool; }

bool BaseFunction::GetRecursive() const { return recursive; }
void BaseFunction::SetRecursive(bool _bool) { recursive = _bool; }

void BaseFunction::InsertCallWho(BaseFunction *func) { call_who.insert(func); }
void BaseFunction::RemoveCallWho(BaseFunction *func) { call_who.erase(func); }
CallerFunction &BaseFunction::GetCallWho() { return call_who; }

void BaseFunction::InsertWhoCall(BaseFunction *func) { who_call.insert(func); }
void BaseFunction::RemoveWhoCall(BaseFunction *func) { who_call.erase(func); }
CalleeFunction &BaseFunction::GetWhoCall() { return who_call; }

bool BaseFunction::IsBeUsed() const { return (who_call.size() > 0) || (func_name == "main"); }

ScalarTypePtr BaseFunction::GetReturnType() { return this->ret_type; }

std::string &BaseFunction::GetFuncName() { return this->func_name; }

size_t BaseFunction::GetParamSize() const { return param_list.size(); }
ParamList &BaseFunction::GetParamList() { return this->param_list; }