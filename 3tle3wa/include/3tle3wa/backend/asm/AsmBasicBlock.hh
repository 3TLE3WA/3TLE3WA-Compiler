#pragma once

#include <cstddef>
#include <list>
#include <memory>

#include "3tle3wa/backend/Interface.hh"

class AsmInstruction;
class AsmProgress;

class AsmBasicBlock final : public Serializable {
    size_t lbidx_;

    const AsmProgress *father_;

    std::list<std::unique_ptr<AsmInstruction>> insts_;

    std::list<AsmInstruction *> insts_view_;

    bool is_ret_;

    void formatString(FILE *fp) final;

   public:
    AsmBasicBlock(size_t lbidx, AsmProgress *father);

    void Push(AsmInstruction *inst);

    void Pop();

    void SetIsRet(bool on);
    bool IsRet();

    size_t Lbidx();

    const char *FatherLabel() const;

    size_t SearchFirst();

    size_t GetBlockIdx();

    void ArchSchedule();

    void Peepholes();
};