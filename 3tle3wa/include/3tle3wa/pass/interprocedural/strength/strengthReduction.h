#pragma once

#include "3tle3wa/ir/IR.hh"

namespace strengthReduction {

    void StrengthReduction(const NormalFuncPtr&);
    void start(Loop *);
    void divInst2sub(InstPtr divInst, Loop *loop, int64_t step, InstPtr originInst);

    void red(const std::string&);
    void blue(const std::string&);
    void green(const std::string&);
    void yellow(const std::string&);

    void red2(const std::string&);

    void mulInst2Add(InstPtr mulInst, Loop *loop, int64_t step, InstPtr originInst);

    std::map<InstPtr, int> detectInstInBlk(const BaseValuePtr &lhs, const BlockPtr &blk, Loop *loop);

    bool isNotChanged(const BaseValuePtr &sharedPtr, Loop *pLoop, InstPtr &oInst);

    void insertMulValInit(const CfgNodePtr &blk, const VariablePtr& vp, int64_t val, VariablePtr ptr, InstPtr originInst);
    void insertDivValInit(const CfgNodePtr &blk, VariablePtr vp, int64_t val, VariablePtr ptr);

    bool isNotUsedInCurLoop(VariablePtr var, Loop *pLoop, const CfgNodePtr& ptr);


}