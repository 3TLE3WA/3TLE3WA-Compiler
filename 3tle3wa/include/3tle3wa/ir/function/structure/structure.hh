#pragma once

#include <list>

#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/ir/function/cfgNode.hh"

typedef size_t depth_t;

struct Structure {
    enum Type { Loop, Branch } type;

    depth_t depth;
    Structure *parent;

    std::list<Structure *> sub_structures;

    Structure(Type _type, depth_t _depth, Structure *_parent) : type(_type), depth(_depth), parent(_parent){};
    ~Structure() = default;

    bool IsLoop() const { return type == Type::Loop; };
    bool IsBranch() const { return type == Type::Branch; };

    virtual CfgNodeList GetEntireStructure() const = 0;

    virtual void PrintCurStructure() const = 0;
    virtual void PrintStructure() const = 0;
};