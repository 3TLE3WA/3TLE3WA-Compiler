#pragma once

#include <cstddef>
#include <cstdint>
#include <list>
#include <memory>

#include "3tle3wa/ir/function/cfgNode.hh"
#include "3tle3wa/ir/function/structure/structure.hh"
#include "3tle3wa/ir/instruction/instruction.hh"

struct Loop final : Structure {
    CfgNodePtr before_blk;

    CfgNodePtr cond_begin;
    CfgNodePtr cond_end;

    CfgNodePtr body_begin;
    // CfgNodePtr body_end;

    CfgNodePtr loop_exit;

    Loop(Structure *_parent = nullptr, depth_t _depth = 0)
        : Structure(Structure::Loop, _depth, _parent),
          before_blk(nullptr),
          cond_begin(nullptr),
          cond_end(nullptr),
          body_begin(nullptr),
          // body_end(nullptr),
          loop_exit(nullptr) {}

    ~Loop() = default;

    CfgNodeList GetCondBodyBlks() const;
    CfgNodeList GetLoopBodyBlks() const;

    CfgNodeList GetEntireStructure() const;

    bool IsSimpleLoop() const;

    void PrintCurStructure() const;
    void PrintStructure() const;
};
