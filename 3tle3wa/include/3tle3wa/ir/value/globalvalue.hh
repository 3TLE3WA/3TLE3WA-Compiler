#pragma once

#include <set>

#include "3tle3wa/ir/value/baseValue.hh"

class GlobalValue;
using GlobalValuePtr = std::shared_ptr<GlobalValue>;

class BaseFunction;

class GlobalValue final : public BaseValue {
   private:
    size_t idx;
    BaseValuePtr init_value;

    std::set<BaseFunction *> param_use;
    std::set<BaseFunction *> used_in;
    std::set<BaseFunction *> define_in;

    static size_t glb_idx;

   public:
    GlobalValue(BaseTypePtr, BaseValuePtr);

    void SetInitValue(BaseValuePtr);
    BaseValuePtr GetInitValue() const;
    size_t GetGlobalValueIdx() const;

    bool IsGlobalValue() const;

    void AddParamUse(BaseFunction *);
    void RmvParamUse(BaseFunction *);
    bool IsParamBy(BaseFunction *) const;
    bool IsAsParam() const;

    void InsertUser(BaseFunction *);
    void RemoveUser(BaseFunction *);
    bool IsBeenUsed() const;
    bool IsUsedBy(BaseFunction *);
    const std::set<BaseFunction *> &GetUsedIn() const;

    void InsertDefiner(BaseFunction *);
    void RemoveDefiner(BaseFunction *);
    bool IsBeenDefined() const;
    bool IsDefinedBy(BaseFunction *) const;
    const std::set<BaseFunction *> &GetDefineIn() const;

    static GlobalValuePtr CreatePtr(BaseTypePtr, BaseValuePtr);

    std::string tollvmIR();
};
