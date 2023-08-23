#pragma once

#include <list>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "3tle3wa/backend/Interface.hh"

class AsmGen;
class AsmGlobalValue;
class CompilationUnit;
class InternalTranslation;
class GlobalValue;
class Constant;
class NormalFunction;

using NormalFuncPtr = std::shared_ptr<NormalFunction>;
using NormalFuncList = std::list<NormalFuncPtr>;

class RLGen final : public Serializable {
    // look up table
    std::unordered_map<uint32_t, size_t> lc_map_;
    std::unordered_map<size_t, AsmGlobalValue *> gv_map_;

    std::vector<std::unique_ptr<InternalTranslation>> translation_tasks_;

    std::unique_ptr<AsmGen> asm_gen_;

    void registerGlobalValue(GlobalValue *gvp, const std::string &name);
    void registerLocalConstant(Constant *cvp, const size_t idx);
    void registerNormalFunction(NormalFuncList &nflst);

    void formatString(FILE *fp) final;

   public:
    RLGen();

    std::unique_ptr<AsmGen> &AG();

    void Register(CompilationUnit &comp_unit);

    void SerialGenerate();
};