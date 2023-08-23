#pragma once

#include <list>

#include "3tle3wa/ir/function/function.hh"

using NormalFuncList = std::list<NormalFuncPtr>;
using LibFuncList = std::list<LibFuncPtr>;

class FunctionTable {
   private:
    NormalFuncList normalFuncTable;
    LibFuncList libraryFuncTable;

   public:
    FunctionTable();

    BaseFuncPtr GetFunction(std::string &);

    void InsertFunction(NormalFuncPtr);

    NormalFuncList &GetNormalFuncTable();

    friend std::ostream &operator<<(std::ostream &, FunctionTable);
};
