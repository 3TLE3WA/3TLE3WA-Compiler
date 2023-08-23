#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "3tle3wa/ir/funcTable.hh"
#include "3tle3wa/ir/instHeader.hh"
#include "3tle3wa/ir/symTable.hh"
#include "3tle3wa/ir/valueHeader.hh"

using std::cout;
using std::endl;

class CompilationUnit {
   private:
    SymbolTable glb_table;
    FunctionTable func_talbe;

   public:
    CompilationUnit() = default;
    ~CompilationUnit() = default;

    SymbolTable &getGlbTable();
    void InsertSymbol(std::string &, BaseValuePtr);

    NormalFuncList &GetNormalFuncTable();
    BaseFuncPtr GetFunction(std::string &);
    void InsertFunction(NormalFuncPtr);

    void InsertConstantToGlbTable();

    void generatellvmIR(std::string irile);
};
