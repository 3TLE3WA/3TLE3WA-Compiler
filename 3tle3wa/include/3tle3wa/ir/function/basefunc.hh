#pragma once

#include <string>

#include "3tle3wa/ir/valueHeader.hh"

class BaseFunction;
using BaseFuncPtr = std::shared_ptr<BaseFunction>;

typedef std::set<BaseFunction *> CallerFunction;
typedef std::set<BaseFunction *> CalleeFunction;

class BaseFunction {
   protected:
    ScalarTypePtr ret_type;
    std::string func_name;
    ParamList param_list;

    bool side_effect;  // modify array or global-variable
    bool recursive;

    CallerFunction call_who;  // this call who
    CalleeFunction who_call;  // who call this

   public:
    BaseFunction(ScalarTypePtr, std::string &, ParamList &, bool _side_effect = false);
    ~BaseFunction() = default;

    virtual bool IsLibFunction() const = 0;

    bool GetSideEffect() const;
    void SetSideEffect(bool);

    bool GetRecursive() const;
    void SetRecursive(bool);

    void InsertCallWho(BaseFunction *);
    void RemoveCallWho(BaseFunction *);
    CallerFunction &GetCallWho();

    void InsertWhoCall(BaseFunction *);
    void RemoveWhoCall(BaseFunction *);
    CalleeFunction &GetWhoCall();

    bool IsBeUsed() const;

    ScalarTypePtr GetReturnType();

    std::string &GetFuncName();

    size_t GetParamSize() const;
    ParamList &GetParamList();

    virtual std::string tollvmIR() = 0;
};