#pragma once

#include "3tle3wa/ir/function/basefunc.hh"
#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/value/baseValue.hh"

class BaseFunction;
using BaseFuncPtr = std::shared_ptr<BaseFunction>;

class CallInst;
using CallInstPtr = std::shared_ptr<CallInst>;

class CallInst final : public Instruction {
   private:
    ScalarTypePtr ret_type;
    BaseFuncPtr callee_func;
    ParamList rparam_list;

    bool tail_call;

   public:
    CallInst(ScalarTypePtr, VariablePtr, BaseFuncPtr, ParamList &, CfgNodePtr);
    ~CallInst() = default;

    static CallInstPtr CreatePtr(ScalarTypePtr, VariablePtr, BaseFuncPtr, ParamList &, CfgNodePtr);

    static BaseValuePtr DoCallFunction(ScalarTypePtr, BaseFuncPtr, ParamList &, CfgNodePtr);

    ScalarTypePtr GetRetType() const;
    BaseFuncPtr GetCalleeFunc() const;
    ParamList &GetParamList();

    void SetTailCall();
    bool GetTailCall() const;

    void RemoveResParent() final override;

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    const BaseValueList GetOprands() const final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class BitCastInst;
using BitCastInstPtr = std::shared_ptr<BitCastInst>;

class BitCastInst final : public UnaryInstruction {
   private:
    static BitCastInstPtr CreatePtr(VariablePtr, BaseValuePtr, CfgNodePtr);

   public:
    BitCastInst(VariablePtr, BaseValuePtr, CfgNodePtr);
    ~BitCastInst() = default;

    static VariablePtr DoBitCast(BaseValuePtr, CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class PhiInst;
using PhiInstPtr = std::shared_ptr<PhiInst>;

class AllocaInst;
using AllocaInstPtr = std::shared_ptr<AllocaInst>;
using PhiInstList = std::list<PhiInstPtr>;

class PhiInst final : public Instruction {
   private:
    AllocaInstPtr origin_alloca;

    std::list<std::pair<BaseValuePtr, CfgNodePtr>> datalist;

   public:
    PhiInst(VariablePtr, CfgNodePtr);
    ~PhiInst() = default;

    static PhiInstPtr CreatePtr(BaseTypePtr, CfgNodePtr);

    void SetOriginAlloca(AllocaInstPtr);

    AllocaInstPtr GetOriginAlloca();

    static void InsertPhiData(PhiInstPtr, BaseValuePtr, CfgNodePtr);

    std::list<std::pair<BaseValuePtr, CfgNodePtr>> &GetRefList();
    const std::list<std::pair<BaseValuePtr, CfgNodePtr>> &GetDataList() const;

    void RemoveResParent() final override;

    BaseValuePtr FindInComingUse(CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    const BaseValueList GetOprands() const final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};
