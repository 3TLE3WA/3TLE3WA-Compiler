#include "3tle3wa/ir/symTable.hh"

SymbolTable::SymbolTable(SymbolTable *_p) : parent(_p), sym_table(NameValueMap()) {}

NameValueMap &SymbolTable::GetNameValueMap() { return this->sym_table; }

SymbolTable *SymbolTable::GetParentTable() { return this->parent; }

void SymbolTable::InsertSymbol(std::string &name, BaseValuePtr value) { sym_table.insert({name, value}); }