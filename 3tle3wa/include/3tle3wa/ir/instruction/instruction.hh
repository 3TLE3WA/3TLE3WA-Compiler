#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

#include "3tle3wa/backend/Interface.hh"
#include "3tle3wa/ir/exprFlod.hh"
#include "3tle3wa/ir/instruction/opCode.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/variable.hh"

class Instruction;
using InstPtr = std::shared_ptr<Instruction>;

class CtrlFlowGraphNode;
using CfgNodePtr = std::shared_ptr<CtrlFlowGraphNode>;

class Instruction : public Translatable {
   protected:
    size_t idx;

    VariablePtr result;
    OpCode opcode;

    CfgNodePtr parent;

   private:
    static size_t inst_idx;

   public:
    Instruction(VariablePtr, OpCode, CfgNodePtr);
    ~Instruction() = default;

    const OpCode &GetOpCode() const;
    const size_t &GetInstIdx() const;
    const CfgNodePtr &GetParent() const;
    void SetParent(CfgNodePtr);
    void ClearParent();

    bool IsTwoOprandInst() const;
    bool IsOneOprandInst() const;

    bool IsReturnInst() const;
    bool IsJumpInst() const;
    bool IsBranchInst() const;

    bool IsAllocaInst() const;
    bool IsStoreInst() const;
    bool IsLoadInst() const;
    bool IsGepInst() const;

    bool IsCallInst() const;
    bool IsPhiInst() const;
    bool IsBitCast() const;

    bool IsFNegInst() const;
    bool IsFAbsInst() const;

    bool IsValueNumberInst() const;

    bool IsCriticalOperation() const;

    VariablePtr GetResult() const;

    virtual const BaseValueList GetOprands() const = 0;

    // only unary-inst binary-inst can be flod
    virtual BaseValuePtr DoFlod() const;

    virtual void ReplaceTarget(CfgNodePtr, CfgNodePtr);

    virtual void RemoveResParent() = 0;

    virtual bool ReplaceSRC(BaseValuePtr, BaseValuePtr) = 0;

    virtual std::string tollvmIR() = 0;
};

class UnaryInstruction;
using UnaryInstPtr = std::shared_ptr<UnaryInstruction>;
class UnaryInstruction : public Instruction {
   protected:
    BaseValuePtr oprand;

   public:
    UnaryInstruction(VariablePtr, OpCode, BaseValuePtr, CfgNodePtr);
    ~UnaryInstruction() = default;

    BaseValuePtr GetOprand() const;

    BaseValuePtr DoFlod() const;

    void RemoveResParent();

    const BaseValueList GetOprands() const;

    virtual std::string tollvmIR() = 0;
};

class BinaryInstruction;
using BinaryInstPtr = std::shared_ptr<BinaryInstruction>;
class BinaryInstruction : public Instruction {
   protected:
    BaseValuePtr lhs;
    BaseValuePtr rhs;

   public:
    BinaryInstruction(VariablePtr, OpCode, BaseValuePtr, BaseValuePtr, CfgNodePtr);
    ~BinaryInstruction() = default;

    void SetOpCode(OpCode);

    void SetLHS(BaseValuePtr);
    BaseValuePtr GetLHS() const;
    void SetRHS(BaseValuePtr);
    BaseValuePtr GetRHS() const;

    void SwapOprand();

    virtual bool IsIBinaryInst() const;
    virtual bool IsFBinaryInst() const;
    virtual bool IsICmpInst() const;
    virtual bool IsFCmpInst() const;

    BaseValuePtr DoFlod() const;

    void RemoveResParent();

    const BaseValueList GetOprands() const;

    virtual std::string tollvmIR() = 0;
};

BaseValuePtr DoUnaryFlod(OpCode, BaseValuePtr);
BaseValuePtr DoBinaryFlod(OpCode, BaseValuePtr, BaseValuePtr);

void ReplaceSRC(BaseValuePtr, BaseValuePtr);
void RemoveInst(InstPtr);
