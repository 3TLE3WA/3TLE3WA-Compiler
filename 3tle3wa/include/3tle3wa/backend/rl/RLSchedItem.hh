#pragma once

#include "3tle3wa/backend/scheduler/SchedItem.hh"
#include "3tle3wa/backend/rl/RLUop.hh"

struct RLSchedItem final : public SchedItem {
    UopGeneral *uop;
};