#pragma once

#include <list>

#include "3tle3wa/backend/scheduler/Enums.hh"

struct SchedItem;

class SchedPolicy {
   protected:
    std::list<SchedItem *> view_;

   public:
    SchedPolicy() : view_() {}

    virtual ~SchedPolicy() = default;

    virtual void Push(SchedItem *item) = 0;
    virtual void Sched() = 0;

    std::list<SchedItem *> &View() { return view_; }
};