#pragma once

#include "3tle3wa/ir/value/baseValue.hh"

class Variable;
using VariablePtr = std::shared_ptr<Variable>;

class Variable final : public BaseValue {
   private:
    size_t idx;
    InstPtr parent;

    static size_t var_idx;

   public:
    Variable(BaseTypePtr, InstPtr);
    ~Variable() = default;

    void SetParent(InstPtr);
    const InstPtr GetParent() const;
    const size_t &GetVariableIdx() const;

    bool IsVariable() const;

    static void ResetVarIdx();
    static size_t GetVarIdx();
    static void SetVarIdx(size_t);

    static VariablePtr CreatePtr(BaseTypePtr, InstPtr);

    std::string tollvmIR();
};