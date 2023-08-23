#include <cinttypes>
#include <unordered_set>

#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/backend/utils.hh"

void UopRet::SetRetVal(VirtualRegister *retval) {
    retval_ = retval;
    retval_->SetIsRetval(true);
}

void UopCall::SetRetVal(VirtualRegister *retval) {
    retval_ = retval;
    retval_->SetIsRetval(true);
}

void UopCall::SetCallee(std::string &callee) { callee_ = callee; }

void UopCall::SetLibCall(bool libcall) { libcall_ = libcall; }

void UopCall::SetTailCall(bool tailcall) { tailcall_ = tailcall; }

void UopCall::SetCallSelf(bool callself) { callself_ = callself; }

void UopCall::PushParam(VirtualRegister *param) { params_.push_back(param); }

void UopCall::PushLiver(VirtualRegister *liver) {
    if (liver == retval_) {
        return;
    }
    living_regs_.insert(liver);
}

void UopCall::PushParamLiver() { living_regs_.insert(params_.begin(), params_.end()); }

void UopLui::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopLui::SetImm(uint32_t imm) { imm_up20_ = imm; }

void UopLi::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopLi::SetImm(int32_t imm) { imm32_ = imm; }

void UopMv::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopMv::SetSrc(VirtualRegister *src) { src_ = src; }

void UopCvtS2W::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopCvtS2W::SetSrc(VirtualRegister *src) { src_ = src; }

void UopCvtW2S::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopCvtW2S::SetSrc(VirtualRegister *src) { src_ = src; }

void UopBranch::SetCond(VirtualRegister *cond) { cond_ = cond; }

void UopBranch::SetOnTrue(bool cond) { on_true_ = cond; }

void UopBranch::SetDstIdx(size_t dst_idx) { dst_idx_ = dst_idx; }

void UopJump::SetDstIdx(size_t dst_idx) { dst_idx_ = dst_idx; }

void UopLla::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopLla::SetSrc(std::string src) { src_ = std::move(src); }

void UopLla::SetOff(size_t off) { off_ = off; }

void UopLoad::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopLoad::SetBase(VirtualRegister *base) { base_ = base; }

void UopLoad::SetOff(int32_t off) {
    if (not ImmWithin(12, off)) {
        panic("illegel immediate <%" PRIx32 ">", off);
    }
    off_lo12_ = off;
}

void UopStore::SetSrc(VirtualRegister *src) { src_ = src; }

void UopStore::SetBase(VirtualRegister *base) { base_ = base; }

void UopStore::SetOff(int32_t off) {
    if (not ImmWithin(12, off)) {
        panic("illegel immediate <%" PRIx32 ">", off);
    }
    off_lo12_ = off;
}

void UopFLoad::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopFLoad::SetBase(VirtualRegister *base) { base_ = base; }

void UopFLoad::SetOff(int32_t off) {
    if (not ImmWithin(12, off)) {
        panic("illegel immediate <%" PRIx32 ">", off);
    }
    off_lo12_ = off;
}

void UopFStore::SetSrc(VirtualRegister *src) { src_ = src; }

void UopFStore::SetBase(VirtualRegister *base) { base_ = base; }

void UopFStore::SetOff(int32_t off) {
    if (not ImmWithin(12, off)) {
        panic("illegel immediate <%" PRIx32 ">", off);
    }
    off_lo12_ = off;
}

void UopFLoadLB::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopFLoadLB::SetHelper(VirtualRegister *helper) { helper_ = helper; }

void UopFLoadLB::SetSym(std::string sym) { sym_ = std::move(sym); }

void UopFCmp::SetLhs(VirtualRegister *lhs) { lhs_ = lhs; }

void UopFCmp::SetRhs(VirtualRegister *rhs) { rhs_ = rhs; }

void UopFCmp::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopFCmp::SetKind(COMP_KIND kind) { kind_ = kind; }

void UopIBin::SetLhs(VirtualRegister *lhs) { lhs_ = lhs; }

void UopIBin::SetRhs(VirtualRegister *rhs) { rhs_ = rhs; }

void UopIBin::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopIBin::SetKind(IBIN_KIND kind) { kind_ = kind; }

void UopIBin64::SetLhs(VirtualRegister *lhs) { lhs_ = lhs; }

void UopIBin64::SetRhs(VirtualRegister *rhs) { rhs_ = rhs; }

void UopIBin64::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopIBin64::SetKind(IBIN_KIND kind) { kind_ = kind; }

void UopIBinImm::SetLhs(VirtualRegister *lhs) { lhs_ = lhs; }

void UopIBinImm::SetImm(int32_t imm) {
    if (not ImmWithin(12, imm)) {
        panic("illegel immediate <%" PRIx32 ">", imm);
    }
    imm_lo12_ = imm;
}

void UopIBinImm::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopIBinImm::SetKind(IBIN_KIND kind) { kind_ = kind; }

void UopIBinImm64::SetLhs(VirtualRegister *lhs) { lhs_ = lhs; }

void UopIBinImm64::SetImm(int32_t imm) {
    if (not ImmWithin(12, imm)) {
        panic("illegel immediate <%" PRIx32 ">", imm);
    }
    imm_lo12_ = imm;
}

void UopIBinImm64::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopIBinImm64::SetKind(IBIN_KIND kind) { kind_ = kind; }

void UopFBin::SetLhs(VirtualRegister *lhs) { lhs_ = lhs; }

void UopFBin::SetRhs(VirtualRegister *rhs) { rhs_ = rhs; }

void UopFBin::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopFBin::SetKind(FBIN_KIND kind) { kind_ = kind; }

void UopFTri::SetLhs(VirtualRegister *lhs) { lhs_ = lhs; }

void UopFTri::SetRhs(VirtualRegister *rhs) { rhs_ = rhs; }

void UopFTri::SetAhs(VirtualRegister *ahs) { ahs_ = ahs; }

void UopFTri::SetDst(VirtualRegister *dst) { dst_ = dst; }

void UopFTri::SetKind(FTRI_KIND kind) { kind_ = kind; }

void UopICmpBranch::SetLhs(VirtualRegister *lhs) { lhs_ = lhs; }

void UopICmpBranch::SetRhs(VirtualRegister *rhs) { rhs_ = rhs; }

void UopICmpBranch::SetDstIdx(size_t dst_idx) { dst_idx_ = dst_idx; }

void UopICmpBranch::SetKind(COMP_KIND kind) { kind_ = kind; }
