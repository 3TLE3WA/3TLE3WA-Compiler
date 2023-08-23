#pragma once

#include <vector>

#include "3tle3wa/backend/scheduler/Enums.hh"

struct SchedItem {
    std::vector<size_t> reads;
    std::vector<size_t> writes;
    SCHED_TYPE type;

    bool memop;

    virtual ~SchedItem() = default;
};