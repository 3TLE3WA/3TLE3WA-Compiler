#pragma once

#include <list>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "3tle3wa/backend/scheduler/Enums.hh"
#include "3tle3wa/backend/scheduler/SchedPolicy.hh"

struct SchedItem;

class SchedFastUse final : public SchedPolicy {
    struct AOVNode {
        SchedItem *self{nullptr};

        std::unordered_set<AOVNode *> post{};
        std::unordered_set<AOVNode *> prev{};

        bool issued{false};
    };

    struct Wrapper {
        AOVNode *node;

        bool operator<(const Wrapper &other) const;
    };

    struct RWStatus {
        AOVNode *writer{nullptr};
        std::unordered_set<AOVNode *> reader{};
    };

    AOVNode *fencer_;
    AOVNode *call_;
    std::unordered_map<size_t, RWStatus> status_;
    RWStatus memory_;

    std::list<std::unique_ptr<AOVNode>> storage_;

    std::vector<Wrapper> aovfree_;

   public:
    SchedFastUse();

    void Push(SchedItem *item);
    void Sched();
};