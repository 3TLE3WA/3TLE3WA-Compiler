#pragma once

#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/value/type/scalarType.hh"

class ReturnInst;
using RetInstPtr = std::shared_ptr<ReturnInst>;

class ReturnInst final : public Instruction {
   private:
    ScalarTypePtr ret_type;
    BaseValuePtr ret_value;

   public:
    ReturnInst(ScalarTypePtr, BaseValuePtr, CfgNodePtr);
    ~ReturnInst() = default;

    static RetInstPtr CreatePtr(ScalarTypePtr, BaseValuePtr, CfgNodePtr);

    ScalarTypePtr GetRetType() const;
    BaseValuePtr GetRetValue() const;

    void RemoveResParent() final override;

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    const BaseValueList GetOprands() const final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class JumpInst;
using JumpInstPtr = std::shared_ptr<JumpInst>;

class JumpInst final : public Instruction {
   private:
    CfgNodePtr dest;

   public:
    JumpInst(CfgNodePtr, CfgNodePtr);
    ~JumpInst() = default;

    static JumpInstPtr CreatePtr(CfgNodePtr, CfgNodePtr);

    void SetTarget(CfgNodePtr);

    CfgNodePtr GetTarget();

    void ReplaceTarget(CfgNodePtr, CfgNodePtr) final override;

    void RemoveResParent() final override;

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    const BaseValueList GetOprands() const final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class BranchInst;
using BranchInstPtr = std::shared_ptr<BranchInst>;

class BranchInst final : public Instruction {
   private:
    BaseValuePtr cond;
    CfgNodePtr iftrue;
    CfgNodePtr iffalse;

   public:
    BranchInst(BaseValuePtr, CfgNodePtr, CfgNodePtr, CfgNodePtr);
    ~BranchInst() = default;

    static BranchInstPtr CreatePtr(BaseValuePtr, CfgNodePtr, CfgNodePtr, CfgNodePtr);

    void SetTrueTarget(CfgNodePtr);
    void SetFalseTarget(CfgNodePtr);

    BaseValuePtr GetCondition();
    CfgNodePtr GetTrueTarget();
    CfgNodePtr GetFalseTarget();

    void ReplaceTarget(CfgNodePtr, CfgNodePtr) final override;

    void RemoveResParent() final override;

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    const BaseValueList GetOprands() const final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};
