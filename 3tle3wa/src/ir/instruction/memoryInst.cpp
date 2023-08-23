#include "3tle3wa/ir/instruction/memoryInst.hh"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "3tle3wa/ir/instruction/instruction.hh"
#include "3tle3wa/ir/value/baseValue.hh"
#include "3tle3wa/ir/value/constant.hh"
#include "3tle3wa/ir/value/globalvalue.hh"
#include "3tle3wa/ir/value/type/scalarType.hh"

//===-----------------------------------------------------------===//
//                     AllocaInst Implementation
//===-----------------------------------------------------------===//

AllocaInst::AllocaInst(BaseTypePtr _ty_stored, VariablePtr _addr, CfgNodePtr block)
    : Instruction(_addr, Alloca, block), type_stored(_ty_stored) {
    BaseTypePtr type_alloca = result->GetBaseType();
    assert(type_stored->IntType() || type_stored->FloatType());
    assert(type_stored->GetAttrType() == type_alloca->GetAttrType());
    assert(type_stored->IsNotPtr() && type_alloca->IsPointer());
}

AllocaInstPtr AllocaInst::CreatePtr(BaseTypePtr _ty_stored, VariablePtr _addr, CfgNodePtr block) {
    return std::make_shared<AllocaInst>(_ty_stored, _addr, block);
}

VariablePtr AllocaInst::DoAllocaAddr(BaseTypePtr _ty_stored, BaseTypePtr _ty_alloca, CfgNodePtr block) {
    VariablePtr _addr = Variable::CreatePtr(_ty_alloca, nullptr);
    auto &&inst = CreatePtr(_ty_stored, _addr, block);
    _addr->SetParent(inst);
    block->InsertInstBack(inst);
    return _addr;
}

const BaseTypePtr AllocaInst::GetAllocaType() const { return type_stored; }
const VariablePtr AllocaInst::GetAllocaAddr() const { return result; }

void AllocaInst::RemoveResParent() { result->SetParent(nullptr); }

bool AllocaInst::ReplaceSRC(CRVC_UNUSE BaseValuePtr replacee, CRVC_UNUSE BaseValuePtr replacer) { return false; }

const BaseValueList AllocaInst::GetOprands() const { return BaseValueList(); }

std::string AllocaInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = alloca " << type_stored->tollvmIR() << ", align 4";
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     StoreInst Implementation
//===-----------------------------------------------------------===//

StoreInst::StoreInst(BaseValuePtr addr, BaseValuePtr value, CfgNodePtr block)
    : Instruction(nullptr, Store, block), store_addr(addr), store_value(value) {
    BaseTypePtr type_addr = store_addr->GetBaseType();
    BaseTypePtr type_value = store_value->GetBaseType();
    assert(type_addr->GetAttrType() == type_value->GetAttrType());
    assert(type_addr->IsPointer() && type_value->IsNotPtr());
    assert(type_addr->IsScalar() && type_value->IsScalar());
}

StoreInstPtr StoreInst::CreatePtr(BaseValuePtr addr, BaseValuePtr value, CfgNodePtr block) {
    return std::make_shared<StoreInst>(addr, value, block);
}

void StoreInst::DoStoreValue(BaseValuePtr addr, BaseValuePtr value, CfgNodePtr block) {
    BaseTypePtr addr_type = addr->GetBaseType();
    assert(addr_type->IsPointer() && addr_type->IsScalar() && (addr_type->IntType() || addr_type->FloatType()));

    if (addr_type->IsGlobal()) {
        addr = GetElementPtrInst::DoGetPointer(
            addr_type->IntType() ? type_int_L : type_float_L, addr,
            OffsetList(1, ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))), block);
    }

    if (auto [glb_addr, is_glb] = AddrFromGlobal(addr.get()); is_glb) {
        glb_addr->InsertDefiner(block->GetParent());
    }
    // for store, only two target type, `INT32` and `FLOAT`
    assert(value->IsOprand());
    BaseValuePtr convertee = Value::ScalarTypeConvert(addr->GetBaseType()->GetAttrType(), value, block);
    auto &&inst = CreatePtr(addr, convertee, block);
    addr->InsertUser(inst);
    convertee->InsertUser(inst);
    block->InsertInstBack(inst);
}

const BaseValuePtr StoreInst::GetStoreAddr() const { return store_addr; }
BaseValuePtr StoreInst::GetStoreValue() const { return store_value; }

void StoreInst::RemoveResParent() { return; }

bool StoreInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    if (store_addr == replacee) {
        assert(replacer->GetBaseType()->GetAttrType() == store_addr->GetBaseType()->GetAttrType());
        assert(replacer->GetBaseType()->IsPointer() && replacer->GetBaseType()->IsScalar());
        store_addr = replacer;
        ret = true;
    }
    if (store_value == replacee) {
        assert(replacer->GetBaseType()->GetAttrType() == store_value->GetBaseType()->GetAttrType());
        assert(replacer->GetBaseType()->IsNotPtr() && replacer->GetBaseType()->IsScalar());
        store_value = replacer;
        ret = true;
    }
    return ret;
}

const BaseValueList StoreInst::GetOprands() const { return BaseValueList({store_addr, store_value}); }

std::string StoreInst::tollvmIR() {
    std::stringstream ss;
    ss << "store " << store_value->GetBaseType()->tollvmIR() << ' ' << store_value->tollvmIR();
    ss << ", " << store_addr->GetBaseType()->tollvmIR() << ' ' << store_addr->tollvmIR();
    ss << ", align 4";
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     LoadInst Implementation
//===-----------------------------------------------------------===//

LoadInst::LoadInst(VariablePtr value, BaseValuePtr addr, CfgNodePtr block)
    : UnaryInstruction(value, Load, addr, block) {
    BaseTypePtr type_addr = oprand->GetBaseType();
    BaseTypePtr type_value = result->GetBaseType();
    assert(type_addr->GetAttrType() == type_value->GetAttrType());
    assert(type_addr->IsPointer() && type_value->IsNotPtr());
    assert(type_addr->IsScalar() && type_value->IsScalar());
}

LoadInstPtr LoadInst::CreatePtr(VariablePtr value, BaseValuePtr addr, CfgNodePtr block) {
    return std::make_shared<LoadInst>(value, addr, block);
}

BaseValuePtr LoadInst::DoLoadValue(BaseValuePtr addr, CfgNodePtr block) {
    BaseTypePtr addr_type = addr->GetBaseType();
    assert(addr_type->IsPointer() && addr_type->IsScalar() && (addr_type->IntType() || addr_type->FloatType()));

    if (addr_type->IsGlobal()) {
        addr = GetElementPtrInst::DoGetPointer(
            addr_type->IntType() ? type_int_L : type_float_L, addr,
            OffsetList(1, ConstantAllocator::FindConstantPtr(static_cast<int32_t>(0))), block);
    }

    if (auto [glb_addr, is_glb] = AddrFromGlobal(addr.get()); is_glb) {
        glb_addr->InsertUser(block->GetParent());
    }

    VariablePtr value = Variable::CreatePtr(addr_type->IntType() ? type_int_L : type_float_L, nullptr);
    auto &&inst = CreatePtr(value, addr, block);
    value->SetParent(inst);
    addr->InsertUser(inst);
    block->InsertInstBack(inst);
    return value;
}

bool LoadInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    if (oprand == replacee) {
        assert(replacer->GetBaseType()->GetAttrType() == oprand->GetBaseType()->GetAttrType());
        assert(replacer->GetBaseType()->IsPointer() && replacer->GetBaseType()->IsScalar());
        oprand = replacer;
        ret = true;
    }
    return ret;
}

std::string LoadInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = load " << result->GetBaseType()->tollvmIR();
    ss << ", " << oprand->GetBaseType()->tollvmIR() << ' ' << oprand->tollvmIR();
    ss << ", align 4";
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     GetElementPtrInst Implementation
//===-----------------------------------------------------------===//

GetElementPtrInst::GetElementPtrInst(VariablePtr _ptr, BaseTypePtr _type, BaseValuePtr _addr, OffsetList _off,
                                     CfgNodePtr block)
    : Instruction(_ptr, Gep, block), store_type(_type), base_addr(_addr), offset_list(_off) {
    assert(result->GetBaseType()->GetAttrType() == store_type->GetAttrType());
    assert(result->GetBaseType()->GetAttrType() == base_addr->GetBaseType()->GetAttrType());
    assert(store_type->IsNotPtr() && base_addr->GetBaseType()->IsPointer());
    if (store_type->IsScalar()) {
        assert(offset_list.size() == 1);
    } else {
        ListTypePtr list1 = std::static_pointer_cast<ListType>(store_type);
        ListTypePtr list2 = std::static_pointer_cast<ListType>(base_addr->GetBaseType());
        assert(list1->GetArrSize() == list2->GetArrSize());
        // assert(offset_list.size() == 2);
    }
}

GepInstPtr GetElementPtrInst::CreatePtr(VariablePtr _ptr, BaseTypePtr _type, BaseValuePtr _addr, OffsetList _off,
                                        CfgNodePtr block) {
    return std::make_shared<GetElementPtrInst>(_ptr, _type, _addr, _off, block);
}

VariablePtr GetElementPtrInst::DoGetPointer(BaseTypePtr _type, BaseValuePtr _addr, OffsetList _off, CfgNodePtr block) {
    // only have INT32-array or FLOAT-array
    VariablePtr _ptr = Variable::CreatePtr(_type->IntType() ? type_int_ptr_L : type_float_ptr_L, nullptr);
    auto &&inst = CreatePtr(_ptr, _type, _addr, _off, block);
    _ptr->SetParent(inst);
    _addr->InsertUser(inst);
    std::for_each(_off.begin(), _off.end(), [&inst](const auto &offset) { offset->InsertUser(inst); });
    block->InsertInstBack(inst);
    return _ptr;
}

BaseTypePtr GetElementPtrInst::GetStoreType() const { return store_type; }
void GetElementPtrInst::SetStoreType(BaseTypePtr _type) { store_type = _type; }

BaseValuePtr GetElementPtrInst::GetBaseAddr() const { return base_addr; }
void GetElementPtrInst::SetBaseAddr(BaseValuePtr _addr) { base_addr = _addr; }

const OffsetList &GetElementPtrInst::GetOffList() const { return offset_list; }
void GetElementPtrInst::SetOffList(OffsetList &_off_list) { offset_list = _off_list; }

void GetElementPtrInst::RemoveResParent() { result->SetParent(nullptr); }

bool GetElementPtrInst::ReplaceSRC(BaseValuePtr replacee, BaseValuePtr replacer) {
    bool ret = false;
    if (base_addr == replacee) {
        assert(replacer->GetBaseType()->GetAttrType() == base_addr->GetBaseType()->GetAttrType());
        assert(replacer->GetBaseType()->IsPointer() && replacer->GetBaseType()->IsScalar());
        base_addr = replacer;
        ret = true;
    }
    for (auto &&offset : offset_list) {
        if (offset == replacee) {
            assert(replacer->GetBaseType()->IntType() && replacer->GetBaseType()->IsNotPtr() &&
                   replacer->GetBaseType()->IsScalar());
            offset = replacer;
            ret = true;
        }
    }
    return ret;
}

const BaseValueList GetElementPtrInst::GetOprands() const {
    BaseValueList value_list = BaseValueList({base_addr});
    std::for_each(offset_list.begin(), offset_list.end(),
                  [&value_list](const auto &off) { value_list.push_back(off); });
    return value_list;
}

std::string GetElementPtrInst::tollvmIR() {
    std::stringstream ss;
    ss << result->tollvmIR() << " = getelementptr inbounds " << store_type->tollvmIR();
    ss << ", " << base_addr->GetBaseType()->tollvmIR() << ' ' << base_addr->tollvmIR();
    for (auto &&offset : offset_list) {
        ss << ", " << offset->GetBaseType()->tollvmIR() << ' ' << offset->tollvmIR();
    }
    ss << "; Inst_" << GetInstIdx() << " from Block_";
    if (parent == nullptr) {
        ss << "None";
    } else {
        ss << parent->GetBlockIdx();
    }
    return ss.str();
}

//===-----------------------------------------------------------===//
//                     AddrFromGlobal Implementation
//===-----------------------------------------------------------===//

std::pair<GlobalValue *, bool> AddrFromGlobal(BaseValue *addr) {
    std::pair<GlobalValue *, bool> ret = {nullptr, false};
    if (addr->IsGlobalValue()) {
        ret = {static_cast<GlobalValue *>(addr), true};
    }
    if (Instruction *inst = addr->GetParent().get()) {
        if (inst->IsGepInst()) {
            GetElementPtrInst *gep_inst = static_cast<GetElementPtrInst *>(inst);
            BaseValue *base_addr = gep_inst->GetBaseAddr().get();
            if (base_addr->IsGlobalValue()) {
                ret = {static_cast<GlobalValue *>(base_addr), true};
            }
        }
    }
    return ret;
};
