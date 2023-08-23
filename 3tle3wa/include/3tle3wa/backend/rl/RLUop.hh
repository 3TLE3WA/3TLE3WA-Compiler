#pragma once

#include <cstdint>
#include <list>
#include <unordered_set>
#include <vector>

#include "3tle3wa/backend/Interface.hh"
#include "3tle3wa/backend/rl/Enums.hh"
#include "3tle3wa/backend/scheduler/Enums.hh"
#include "3tle3wa/ir/instruction/opCode.hh"

class VirtualRegister;
class AsmBasicBlock;
class RLBasicBlock;
class RLPlanner;

class UopGeneral : public Serializable {
   protected:
    size_t uop_idx_{};

   public:
    size_t GetUopIdx() const { return uop_idx_; }
    void SetUopIdx(size_t idx) { uop_idx_ = idx; }

    virtual const std::vector<VirtualRegister *> GetOperands() const = 0;
    virtual VirtualRegister *GetResult() const = 0;

    virtual bool IsCall() { return false; }
    virtual bool IsStore() { return false; }
    virtual bool IsLoad() { return false; }

    virtual void ToAsm(AsmBasicBlock *abb, RLPlanner *plan) = 0;

    virtual void Rewrite(std::list<UopGeneral *>::iterator it, RLBasicBlock *rlbb, RLPlanner *plan) = 0;

    virtual SCHED_TYPE GetSchedType() = 0;

    virtual ~UopGeneral() = default;
};

#define VIRTTBL()                                                                                          \
    virtual const std::vector<VirtualRegister *> GetOperands() const final;                                \
    virtual VirtualRegister *GetResult() const final;                                                      \
    virtual void ToAsm(AsmBasicBlock *abb, RLPlanner *plan) final;                                         \
    virtual void Rewrite(std::list<UopGeneral *>::iterator it, RLBasicBlock *rlbb, RLPlanner *plan) final; \
    virtual SCHED_TYPE GetSchedType() final;

class UopRet : public UopGeneral {
    VirtualRegister *retval_{nullptr};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetRetVal(VirtualRegister *retval);
};

class UopCall : public UopGeneral {
    std::vector<VirtualRegister *> params_{};

    VirtualRegister *retval_{nullptr};

    std::string callee_{};

    std::unordered_set<VirtualRegister *> living_regs_{};

    bool libcall_{false};
    bool tailcall_{false};
    bool callself_{false};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    bool IsCall() { return true; }

    void SetRetVal(VirtualRegister *retval);
    void SetCallee(std::string &callee);
    void SetLibCall(bool libcall);
    void SetTailCall(bool tailcall);
    void SetCallSelf(bool callself);

    void PushParam(VirtualRegister *param);
    void PushLiver(VirtualRegister *liver);
    void PushParamLiver();
};

class UopLui : public UopGeneral {
    VirtualRegister *dst_{nullptr};

    uint32_t imm_up20_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDst(VirtualRegister *dst);
    void SetImm(uint32_t imm);
};

class UopLi : public UopGeneral {
    VirtualRegister *dst_{nullptr};

    int32_t imm32_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDst(VirtualRegister *dst);
    void SetImm(int32_t imm);
};

class UopMv : public UopGeneral {
    VirtualRegister *dst_{nullptr};
    VirtualRegister *src_{nullptr};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDst(VirtualRegister *dst);
    void SetSrc(VirtualRegister *src);
};

class UopCvtS2W : public UopGeneral {
    VirtualRegister *dst_{nullptr};
    VirtualRegister *src_{nullptr};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDst(VirtualRegister *dst);
    void SetSrc(VirtualRegister *src);
};

class UopCvtW2S : public UopGeneral {
    VirtualRegister *dst_{nullptr};
    VirtualRegister *src_{nullptr};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDst(VirtualRegister *dst);
    void SetSrc(VirtualRegister *src);
};

class UopBranch : public UopGeneral {
    VirtualRegister *cond_{nullptr};

    size_t dst_idx_{};
    bool on_true_{false};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetCond(VirtualRegister *cond);
    void SetOnTrue(bool cond);
    void SetDstIdx(size_t dst_idx);
};

class UopJump : public UopGeneral {
    size_t dst_idx_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDstIdx(size_t dst_idx);
};

class UopLla : public UopGeneral {
    VirtualRegister *dst_{nullptr};

    std::string src_{};

    size_t off_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDst(VirtualRegister *dst);
    void SetSrc(std::string src);
    void SetOff(size_t off);
};

class UopLoad : public UopGeneral {
    VirtualRegister *dst_{nullptr};
    VirtualRegister *base_{nullptr};

    int32_t off_lo12_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDst(VirtualRegister *dst);
    void SetBase(VirtualRegister *base);
    void SetOff(int32_t off);
};

class UopStore : public UopGeneral {
    VirtualRegister *src_{nullptr};
    VirtualRegister *base_{nullptr};

    int32_t off_lo12_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetSrc(VirtualRegister *src);
    void SetBase(VirtualRegister *base);
    void SetOff(int32_t off);
};

class UopFLoad : public UopGeneral {
    VirtualRegister *dst_{nullptr};
    VirtualRegister *base_{nullptr};

    int32_t off_lo12_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDst(VirtualRegister *dst);
    void SetBase(VirtualRegister *base);
    void SetOff(int32_t off);
};

class UopFLoadLB : public UopGeneral {
    VirtualRegister *dst_{nullptr};
    VirtualRegister *helper_{nullptr};
    std::string sym_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetDst(VirtualRegister *dst);
    void SetHelper(VirtualRegister *helper);
    void SetSym(std::string sym);
};

class UopFStore : public UopGeneral {
    VirtualRegister *src_{nullptr};
    VirtualRegister *base_{nullptr};

    int32_t off_lo12_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetSrc(VirtualRegister *src);
    void SetBase(VirtualRegister *base);
    void SetOff(int32_t off);
};

class UopFCmp : public UopGeneral {
    VirtualRegister *lhs_{nullptr};
    VirtualRegister *rhs_{nullptr};

    VirtualRegister *dst_{nullptr};

    COMP_KIND kind_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    COMP_KIND GetKind() const;

    void SetLhs(VirtualRegister *lhs);
    void SetRhs(VirtualRegister *rhs);
    void SetDst(VirtualRegister *dst);
    void SetKind(COMP_KIND kind);
};

class UopIBin : public UopGeneral {
    VirtualRegister *lhs_{nullptr};
    VirtualRegister *rhs_{nullptr};

    VirtualRegister *dst_{nullptr};

    IBIN_KIND kind_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    IBIN_KIND GetKind() const;

    void SetLhs(VirtualRegister *lhs);
    void SetRhs(VirtualRegister *rhs);
    void SetDst(VirtualRegister *dst);
    void SetKind(IBIN_KIND kind);
};

class UopIBin64 : public UopGeneral {
    VirtualRegister *lhs_{nullptr};
    VirtualRegister *rhs_{nullptr};

    VirtualRegister *dst_{nullptr};

    IBIN_KIND kind_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    IBIN_KIND GetKind() const;

    void SetLhs(VirtualRegister *lhs);
    void SetRhs(VirtualRegister *rhs);
    void SetDst(VirtualRegister *dst);
    void SetKind(IBIN_KIND kind);
};

class UopIBinImm : public UopGeneral {
    VirtualRegister *lhs_{nullptr};
    int32_t imm_lo12_{};

    VirtualRegister *dst_{nullptr};

    IBIN_KIND kind_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    IBIN_KIND GetKind() const;

    void SetLhs(VirtualRegister *lhs);
    void SetImm(int32_t imm);
    void SetDst(VirtualRegister *dst);
    void SetKind(IBIN_KIND kind);
};

class UopIBinImm64 : public UopGeneral {
    VirtualRegister *lhs_{nullptr};
    int32_t imm_lo12_{};

    VirtualRegister *dst_{nullptr};

    IBIN_KIND kind_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    IBIN_KIND GetKind() const;

    void SetLhs(VirtualRegister *lhs);
    void SetImm(int32_t imm);
    void SetDst(VirtualRegister *dst);
    void SetKind(IBIN_KIND kind);
};

class UopFBin : public UopGeneral {
    VirtualRegister *lhs_{nullptr};
    VirtualRegister *rhs_{nullptr};

    VirtualRegister *dst_{nullptr};

    FBIN_KIND kind_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    FBIN_KIND GetKind() const;

    void SetLhs(VirtualRegister *lhs);
    void SetRhs(VirtualRegister *rhs);
    void SetDst(VirtualRegister *dst);
    void SetKind(FBIN_KIND kind);
};

class UopFTri : public UopGeneral {
    VirtualRegister *lhs_{nullptr};
    VirtualRegister *rhs_{nullptr};
    VirtualRegister *ahs_{nullptr};

    VirtualRegister *dst_{nullptr};

    FTRI_KIND kind_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    FTRI_KIND GetKind() const;

    void SetLhs(VirtualRegister *lhs);
    void SetRhs(VirtualRegister *rhs);
    void SetAhs(VirtualRegister *ahs);
    void SetDst(VirtualRegister *dst);
    void SetKind(FTRI_KIND kind);
};

class UopICmpBranch : public UopGeneral {
    VirtualRegister *lhs_{nullptr};
    VirtualRegister *rhs_{nullptr};

    size_t dst_idx_{};

    COMP_KIND kind_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();

    void SetLhs(VirtualRegister *lhs);
    void SetRhs(VirtualRegister *rhs);
    void SetDstIdx(size_t dst_idx);
    void SetKind(COMP_KIND kind);
};

class UopNop : public UopGeneral {
    size_t dst_idx_{};

    void formatString(FILE *fp) final;

   public:
    VIRTTBL();
};