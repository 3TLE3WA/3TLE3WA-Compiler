#include "3tle3wa/ir/value/constarray.hh"

ConstArray::ConstArray(ListTypePtr list_type, ConstArr &_arr) : BaseValue(list_type), const_arr(_arr) {
    // INT32 || FLOAT
    assert(base_type->IntType() || base_type->FloatType());
    // Immutable, NotPtr
    assert(base_type->IsNotPtr() && base_type->IsArray());
    // capacity == arr_size
    assert(const_arr.size() == list_type->GetCapacity());
}

bool ConstArray::IsConstArray() const { return true; }

ConstArr &ConstArray::GetConstArr() { return const_arr; }

ConstArrayPtr ConstArray::CreatePtr(ListTypePtr list_type, ConstArr &_arr) {
    return std::make_shared<ConstArray>(list_type, _arr);
}

std::string ConstArray::tollvmIR() {
    std::stringstream ss;

    ss << '[';
    size_t size = const_arr.size();
    ss << const_arr[0]->GetBaseType()->tollvmIR() << ' ' << const_arr[0]->tollvmIR();
    for (size_t idx = 1; idx < size; ++idx) {
        ss << ", " << const_arr[idx]->GetBaseType()->tollvmIR() << ' ' << const_arr[idx]->tollvmIR();
    }

    ss << ']';

    return ss.str();
}