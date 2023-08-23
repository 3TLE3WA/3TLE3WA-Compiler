#include "3tle3wa/ir/IR.hh"

#include <cassert>

SymbolTable &CompilationUnit::getGlbTable() { return this->glb_table; }

void CompilationUnit::InsertSymbol(std::string &name, BaseValuePtr value) { glb_table.InsertSymbol(name, value); }

NormalFuncList &CompilationUnit::GetNormalFuncTable() { return func_talbe.GetNormalFuncTable(); }

BaseFuncPtr CompilationUnit::GetFunction(std::string &name) { return this->func_talbe.GetFunction(name); }

void CompilationUnit::InsertFunction(NormalFuncPtr func_ptr) { func_talbe.InsertFunction(func_ptr); }

void CompilationUnit::InsertConstantToGlbTable() {
    std::map<Constant *, bool> ConstantMap;
    for (auto [name, value] : glb_table.GetNameValueMap()) {
        if (value->IsConstant()) {
            ConstantMap[std::static_pointer_cast<Constant>(value).get()] = true;
        }
    }
    for (auto [type, constant] : ConstantAllocator::GetConstantAllocator()) {
        if (ConstantMap[constant.get()] == false && constant->GetBaseType()->FloatType() &&
            std::get<float>(constant->GetValue()) != static_cast<float>(0)) {
            std::string name = "Float_" + constant->tollvmIR();
            InsertSymbol(name, constant);
        }
    }
    return;
}

void CompilationUnit::generatellvmIR(std::string irfile) {
    std::ofstream llir;
    llir.open(irfile, std::ios_base::out);
    if (llir.is_open() == false) {
        assert(0);
    }
    llir << "target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"" << endl;
    for (auto [name, glb_value] : glb_table.GetNameValueMap()) {
        BaseTypePtr &&type = glb_value->GetBaseType();
        // // there is no need to emit global-constant llvmIR
        if (type->IsImMutable() && type->IsScalar()) {
            if (type->IntType()) {
                llir << "; @" << name << " = " << type->tollvmIR() << ' ' << glb_value->tollvmIR() << ", align 4";
            } else if (type->FloatType()) {
                llir << "@" << name << " = global " << type->tollvmIR() << ' ' << glb_value->tollvmIR() << ", align 4";
            } else {
                assert(false);
            }
        } else {
            llir << glb_value->tollvmIR() << " = ";
            if (type->IsImMutable()) {
                llir << "constant ";
            } else if (type->IsGlobal()) {
                llir << "global ";
            }
            BaseValuePtr init_value = std::static_pointer_cast<GlobalValue>(glb_value)->GetInitValue();
            llir << init_value->GetBaseType()->tollvmIR() << " " << init_value->tollvmIR() << ", align 4";
        }
        llir << "; " << name << ' ' << glb_value->GetBaseType()->tollvmIR() << endl;
    }
    llir << endl << this->func_talbe << endl;

    llir.flush();
}