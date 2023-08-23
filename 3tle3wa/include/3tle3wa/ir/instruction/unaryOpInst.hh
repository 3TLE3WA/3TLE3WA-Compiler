#pragma once

#include <memory>

#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/variable.hh"

class FNegInst;
using FNegInstPtr = std::shared_ptr<FNegInst>;

class FNegInst final : public UnaryInstruction {
   public:
    FNegInst(VariablePtr, BaseValuePtr, CfgNodePtr);
    ~FNegInst() = default;

    static FNegInstPtr CreatePtr(VariablePtr, BaseValuePtr, CfgNodePtr);

    static VariablePtr DoFloatNeg(BaseValuePtr, CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class FAbsInst;
using FAbsInstPtr = std::shared_ptr<FAbsInst>;

class FAbsInst final : public UnaryInstruction {
   public:
    FAbsInst(VariablePtr, BaseValuePtr, CfgNodePtr);
    ~FAbsInst() = default;

    static FAbsInstPtr CreatePtr(VariablePtr, BaseValuePtr, CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};