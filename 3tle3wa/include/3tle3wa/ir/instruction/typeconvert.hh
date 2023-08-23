#pragma once

#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instruction/compareInst.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/valueHeader.hh"

class SitoFpInst;
using SitoFpInstPtr = std::shared_ptr<SitoFpInst>;

class SitoFpInst final : public UnaryInstruction {
   private:
    static SitoFpInstPtr CreatePtr(VariablePtr, BaseValuePtr, CfgNodePtr);

   public:
    SitoFpInst(VariablePtr, BaseValuePtr, CfgNodePtr);
    ~SitoFpInst() = default;

    static VariablePtr DoSitoFp(BaseValuePtr, CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class FptoSiInst;
using FptoSiInstPtr = std::shared_ptr<FptoSiInst>;

class FptoSiInst final : public UnaryInstruction {
   private:
    static FptoSiInstPtr CreatePtr(VariablePtr, BaseValuePtr, CfgNodePtr);

   public:
    FptoSiInst(VariablePtr, BaseValuePtr, CfgNodePtr);
    ~FptoSiInst() = default;

    static VariablePtr DoFptoSi(ATTR_TYPE, BaseValuePtr, CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class ZextInst;
using ZextInstPtr = std::shared_ptr<ZextInst>;

class ZextInst final : public UnaryInstruction {
   private:
    static ZextInstPtr CreatePtr(VariablePtr, BaseValuePtr, CfgNodePtr);

   public:
    ZextInst(VariablePtr, BaseValuePtr, CfgNodePtr);
    ~ZextInst() = default;

    static VariablePtr DoZeroExt(BaseValuePtr, CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};
