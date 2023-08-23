#include "3tle3wa/ir/value/uninitvar.hh"

UnInitVar::UnInitVar(BaseTypePtr _type) : BaseValue(_type) {
    // INT32 || FLOAT
    assert(base_type->IntType() || base_type->FloatType());
    assert(base_type->IsNotPtr() && base_type->IsGlobal());
}

bool UnInitVar::IsUnInitVar() const { return true; }

UnInitVarPtr UnInitVar::CreatePtr(BaseTypePtr _type) { return std::make_shared<UnInitVar>(_type); }

std::string UnInitVar::tollvmIR() {
    std::stringstream ss;

    if (base_type->IsArray()) {
        ss << "zeroinitializer";
    } else if (base_type->IntType()) {
        ss << "0";
    } else if (base_type->FloatType()) {
        ss << "0x0000000000000000";
    } else {
        assert(0);
    }

    return ss.str();
}
