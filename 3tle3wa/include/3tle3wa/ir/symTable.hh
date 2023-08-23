#pragma once

#include <unordered_map>
#include <vector>

#include "3tle3wa/ir/valueHeader.hh"

using NameValue = std::pair<std::string, BaseValuePtr>;
using NameValueMap = std::unordered_map<std::string, BaseValuePtr>;

class SymbolTable {
   private:
    SymbolTable *parent;
    NameValueMap sym_table;

   public:
    SymbolTable(SymbolTable *_p = nullptr);

    NameValueMap &GetNameValueMap();

    SymbolTable *GetParentTable();

    void InsertSymbol(std::string &, BaseValuePtr);
};
