#pragma once

#include <algorithm>
#include <list>
#include <memory>
#include <set>

class Use;
using UsePtr = std::shared_ptr<Use>;

class Instruction;
using InstPtr = std::shared_ptr<Instruction>;

typedef std::set<InstPtr> UserList;

// As a Base Class of BaseValue
// record Instructions who use this Value
class Use {
   protected:
    UserList user_list;

   public:
    Use() = default;
    ~Use() = default;
};