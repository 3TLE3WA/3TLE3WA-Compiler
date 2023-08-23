#pragma once

#include <cstddef>
#include <cstdint>
#include <list>
#include <memory>

#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/structure/structure.hh"
#include "3tle3wa/ir/instruction/instruction.hh"

struct Branch_ final : Structure {
    CfgNodePtr before_blk;

    CfgNodePtr cond_begin;
    CfgNodePtr cond_end;

    CfgNodePtr iftrue_begin;
    CfgNodePtr iftrue_end;

    CfgNodePtr iffalse_begin;
    CfgNodePtr iffalse_end;

    CfgNodePtr branch_out;

    Branch_(Structure *_parent = nullptr, depth_t _depth = 0)
        : Structure(Structure::Branch, _depth, _parent),
          before_blk(nullptr),
          cond_begin(nullptr),
          cond_end(nullptr),
          iftrue_begin(nullptr),
          iftrue_end(nullptr),
          iffalse_begin(nullptr),
          iffalse_end(nullptr),
          branch_out(nullptr) {}

    ~Branch_() = default;

    CfgNodeList GetCondBodyBlks() const;

    CfgNodeList GetIftrueBlks() const;
    CfgNodeList GetIffalseBlks() const;

    CfgNodeList GetEntireStructure() const;

    void PrintCurStructure() const {}
    void PrintStructure() const {}
};
