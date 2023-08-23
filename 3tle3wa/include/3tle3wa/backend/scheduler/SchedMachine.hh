#pragma once

#include <list>
#include <memory>

#include "3tle3wa/backend/scheduler/Enums.hh"

struct SchedItem;
class SchedPolicy;

class SchedMachine {
    std::list<std::unique_ptr<SchedItem>> items_;

    std::unique_ptr<SchedPolicy> policy_;

   public:
    SchedMachine(std::unique_ptr<SchedPolicy> policy);

    void Push(std::unique_ptr<SchedItem> item);

    void Sched();

    SchedPolicy *Policy();
};