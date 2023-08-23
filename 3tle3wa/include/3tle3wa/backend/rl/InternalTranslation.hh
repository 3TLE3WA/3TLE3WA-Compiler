#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "3tle3wa/backend/Interface.hh"

struct ConstValueInfo;

class NormalFunction;
class CtrlFlowGraphNode;

class AsmGlobalValue;
class RLProgress;
class RLBasicBlock;
class RLPlanner;
class VirtualRegister;
class AsmProgress;

// control flow inst
class ReturnInst;
class JumpInst;
class BranchInst;

// compare inst
class ICmpInst;
class FCmpInst;

// binary inst
class IBinaryInst;
class FBinaryInst;
class FNegInst;
class FAbsInst;

// memory inst
class AllocaInst;
class StoreInst;
class LoadInst;
class GetElementPtrInst;

// type convert
class SitoFpInst;
class FptoSiInst;
class ZextInst;

// other inst
class CallInst;
class BitCastInst;
class PhiInst;

using NormalFuncPtr = std::shared_ptr<NormalFunction>;

struct InternalTranslationContext {
    std::unordered_map<size_t, ICmpInst *> icmp_map;
    std::unordered_map<size_t, bool> fcmp_map;
    std::unordered_map<size_t, std::pair<size_t, size_t>> gep_map;
    std::unordered_map<size_t, VirtualRegister *> lui_map;
    std::unordered_map<size_t, VirtualRegister *> fimm_map;
    std::vector<std::pair<PhiInst *, size_t>> phi_vec;

    CtrlFlowGraphNode *cur_cfg;
    CtrlFlowGraphNode *nxt_cfg;

    RLBasicBlock *cur_blk;
    RLPlanner *planner;

    bool meet_call;
    bool meet_tail;
};

class InternalTranslation final : public Serializable {
    const NormalFuncPtr &fptr_;
    const std::unordered_map<uint32_t, size_t> &lc_map_;
    const std::unordered_map<size_t, AsmGlobalValue *> &gv_map_;

    std::unique_ptr<RLProgress> rlps_;

    std::unique_ptr<AsmProgress> apg_;

    void li(VirtualRegister *dst, ConstValueInfo &cinfo, InternalTranslationContext &ctx);
    void lf(VirtualRegister *dst, ConstValueInfo &cinfo, InternalTranslationContext &ctx);
    void dephi(InternalTranslationContext &ctx);

    void formatString(FILE *fp);

   public:
    InternalTranslation(const NormalFuncPtr &fptr, const std::unordered_map<uint32_t, size_t> &lc_map,
                        const std::unordered_map<size_t, AsmGlobalValue *> &gv_map);

    std::unique_ptr<AsmProgress> ExportAP();

    // instruction select actually
    void DoTranslation();

    // register allocation
    void DoAssignment();

    // translate to asm
    void DoTranslateToAsm();

    // instruction selections

    void Translate(ReturnInst *ll, InternalTranslationContext &ctx);
    void Translate(JumpInst *ll, InternalTranslationContext &ctx);
    void Translate(BranchInst *ll, InternalTranslationContext &ctx);

    void Translate(ICmpInst *ll, InternalTranslationContext &ctx);
    void Translate(FCmpInst *ll, InternalTranslationContext &ctx);

    void Translate(IBinaryInst *ll, InternalTranslationContext &ctx);
    void Translate(FBinaryInst *ll, InternalTranslationContext &ctx);
    void Translate(FNegInst *ll, InternalTranslationContext &ctx);
    void Translate(FAbsInst *ll, InternalTranslationContext &ctx);

    void Translate(AllocaInst *ll, InternalTranslationContext &ctx);
    void Translate(StoreInst *ll, InternalTranslationContext &ctx);
    void Translate(LoadInst *ll, InternalTranslationContext &ctx);
    void Translate(GetElementPtrInst *ll, InternalTranslationContext &ctx);

    void Translate(SitoFpInst *ll, InternalTranslationContext &ctx);
    void Translate(FptoSiInst *ll, InternalTranslationContext &ctx);
    void Translate(ZextInst *ll, InternalTranslationContext &ctx);

    void Translate(CallInst *ll, InternalTranslationContext &ctx);
    void Translate(BitCastInst *ll, InternalTranslationContext &ctx);
    void Translate(PhiInst *ll, InternalTranslationContext &ctx);
};