#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include "3tle3wa/backend/Interface.hh"

// asm class declaration
class AsmGlobalValue;
class AsmLocalConstant;
class AsmProgress;

class AsmGen final : public Serializable {
    // for storage
    std::list<std::unique_ptr<AsmGlobalValue>> gv_storage_;
    std::list<std::unique_ptr<AsmLocalConstant>> lc_storage_;
    std::list<std::unique_ptr<AsmProgress>> pg_storage_;

    void formatString(FILE *fp) final;

   public:
    void PushAsmGlobalValue(std::unique_ptr<AsmGlobalValue> agv);

    void PushAsmLocalConstant(std::unique_ptr<AsmLocalConstant> alc);

    void PushAsmProgress(std::unique_ptr<AsmProgress> ap);
};