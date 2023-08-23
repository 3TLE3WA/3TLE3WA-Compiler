#pragma once

#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instruction/instruction.hh"

class IBinaryInst;
using IBinaryInstPtr = std::shared_ptr<IBinaryInst>;

class IBinaryInst final : public BinaryInstruction {
   public:
    IBinaryInst(VariablePtr, OpCode, BaseValuePtr, BaseValuePtr, CfgNodePtr);
    ~IBinaryInst() = default;

    static IBinaryInstPtr CreatePtr(VariablePtr, OpCode, BaseValuePtr, BaseValuePtr, CfgNodePtr);

    bool IsIBinaryInst() const final override;

    bool IsIntegerNeg() const;

    static VariablePtr DoIBinOperate(OpCode, BaseValuePtr, BaseValuePtr, CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class FBinaryInst;
using FBinaryInstPtr = std::shared_ptr<FBinaryInst>;

class FBinaryInst final : public BinaryInstruction {
   public:
    FBinaryInst(VariablePtr, OpCode, BaseValuePtr, BaseValuePtr, CfgNodePtr);
    ~FBinaryInst() = default;

    static FBinaryInstPtr CreatePtr(VariablePtr, OpCode, BaseValuePtr, BaseValuePtr, CfgNodePtr);

    bool IsFBinaryInst() const final override;

    static VariablePtr DoFBinOperate(OpCode, BaseValuePtr, BaseValuePtr, CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};
