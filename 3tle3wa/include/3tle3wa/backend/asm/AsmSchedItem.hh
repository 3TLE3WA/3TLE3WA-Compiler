#pragma once

#include "3tle3wa/backend/scheduler/SchedItem.hh"
#include "3tle3wa/backend/asm/AsmInstruction.hh"

struct AsmSchedItem final : public SchedItem {
    AsmInstruction *inst;
};