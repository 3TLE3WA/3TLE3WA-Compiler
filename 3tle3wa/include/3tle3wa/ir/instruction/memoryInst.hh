#pragma once

#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/instruction/typeconvert.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/globalvalue.hh"
#include "3tle3wa/ir/value/type/baseType.hh"

class AllocaInst;
using AllocaInstPtr = std::shared_ptr<AllocaInst>;

class AllocaInst final : public Instruction {
   private:
    BaseTypePtr type_stored;

   public:
    AllocaInst(BaseTypePtr, VariablePtr, CfgNodePtr);
    ~AllocaInst() = default;

    static AllocaInstPtr CreatePtr(BaseTypePtr, VariablePtr, CfgNodePtr);

    static VariablePtr DoAllocaAddr(BaseTypePtr, BaseTypePtr, CfgNodePtr);

    const BaseTypePtr GetAllocaType() const;
    const VariablePtr GetAllocaAddr() const;

    void RemoveResParent() final override;

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    const BaseValueList GetOprands() const final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class StoreInst;
using StoreInstPtr = std::shared_ptr<StoreInst>;

class StoreInst final : public Instruction {
   private:
    BaseValuePtr store_addr;
    BaseValuePtr store_value;

   public:
    StoreInst(BaseValuePtr, BaseValuePtr, CfgNodePtr);
    ~StoreInst() = default;

    static StoreInstPtr CreatePtr(BaseValuePtr, BaseValuePtr, CfgNodePtr);

    static void DoStoreValue(BaseValuePtr, BaseValuePtr, CfgNodePtr);

    const BaseValuePtr GetStoreAddr() const;
    BaseValuePtr GetStoreValue() const;

    void RemoveResParent() final override;

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    const BaseValueList GetOprands() const final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class LoadInst;
using LoadInstPtr = std::shared_ptr<LoadInst>;

class LoadInst final : public UnaryInstruction {
   public:
    LoadInst(VariablePtr, BaseValuePtr, CfgNodePtr);
    ~LoadInst() = default;

    static LoadInstPtr CreatePtr(VariablePtr, BaseValuePtr, CfgNodePtr);

    static BaseValuePtr DoLoadValue(BaseValuePtr, CfgNodePtr);

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

class GetElementPtrInst;
using GepInstPtr = std::shared_ptr<GetElementPtrInst>;

typedef std::list<BaseValuePtr> OffsetList;

class GetElementPtrInst final : public Instruction {
   private:
    BaseTypePtr store_type;
    BaseValuePtr base_addr;
    OffsetList offset_list;

   public:
    GetElementPtrInst(VariablePtr, BaseTypePtr, BaseValuePtr, OffsetList, CfgNodePtr);
    ~GetElementPtrInst() = default;

    static GepInstPtr CreatePtr(VariablePtr, BaseTypePtr, BaseValuePtr, OffsetList, CfgNodePtr);

    static VariablePtr DoGetPointer(BaseTypePtr, BaseValuePtr, OffsetList, CfgNodePtr);

    BaseTypePtr GetStoreType() const;
    void SetStoreType(BaseTypePtr);

    BaseValuePtr GetBaseAddr() const;
    void SetBaseAddr(BaseValuePtr);

    const OffsetList &GetOffList() const;
    void SetOffList(OffsetList &);

    void RemoveResParent() final override;

    bool ReplaceSRC(BaseValuePtr, BaseValuePtr) final override;

    const BaseValueList GetOprands() const final override;

    std::string tollvmIR() final override;

    void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx) final override;
};

std::pair<GlobalValue *, bool> AddrFromGlobal(BaseValue *addr);
