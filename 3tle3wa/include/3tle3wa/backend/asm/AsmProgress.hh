#pragma once

#include <cstdio>
#include <list>
#include <memory>
#include <unordered_map>

#include "3tle3wa/backend/Interface.hh"

class AsmBasicBlock;
class AsmGlobalValue;

class AsmProgress final : public Serializable {
    char *label_;
    size_t label_len_;

    std::list<std::unique_ptr<AsmBasicBlock>> ablks_;

    AsmBasicBlock *ret_;

    size_t first_bidx_;

    size_t align_;

    void formatString(FILE *fp) final;

   public:
    AsmProgress(const std::string &name);
    ~AsmProgress();

    void Push(std::unique_ptr<AsmBasicBlock> ablk, bool is_ret = false);

    AsmBasicBlock *CreateEntryBlock();

    AsmBasicBlock *ReturnBlock();

    size_t FirstBlk() const;

    void SetFirstBlk(size_t idx);

    const char *Label() const;

    void DoASchedule();

    void DoOptimization();
};