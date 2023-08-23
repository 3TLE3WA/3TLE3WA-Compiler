#include <cmath>

#include "3tle3wa/backend/asm/AsmAbi.hh"
#include "3tle3wa/backend/asm/AsmGlobalValue.hh"
#include "3tle3wa/backend/rl/InternalTranslation.hh"
#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLPlanner.hh"
#include "3tle3wa/backend/rl/RLProgress.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/backend/utils.hh"
#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/ir/instruction/opCode.hh"

void InternalTranslation::li(VirtualRegister *dst, ConstValueInfo &cinfo, InternalTranslationContext &ctx) {
    int32_t imm = 0;

    // assuming no 64bit imm
    if (cinfo.width_ == 32) {
        imm = cinfo.v32_.i32_;
    } else {
        imm = cinfo.v64_.i64_;
    }

    if (ImmWithin(12, imm)) {
        auto uop = new UopLi;
        uop->SetImm(imm);
        uop->SetDst(dst);

        ctx.cur_blk->PushUop(uop);
        return;
    }

    uint32_t msk = 0xfff;
    uint32_t up20 = 0;

    int32_t lo12 = Sext32(12, msk & imm);
    up20 = ((imm >> 12) + !!(lo12 < 0)) & 0xfffff;

    Assert(up20 != 0, "if up20 == 0, it should not use lui");

    VirtualRegister *upper = nullptr;

    if (auto fnd = ctx.lui_map.find(up20); fnd != ctx.lui_map.end()) {
        upper = fnd->second;
    } else {
        upper = ctx.planner->NewVReg(VREG_TYPE::INT);
        auto uop_lui = new UopLui;
        uop_lui->SetImm(up20);
        uop_lui->SetDst(upper);
        ctx.cur_blk->PushUop(uop_lui);

        ctx.lui_map.emplace(up20, upper);
    }

    if (imm < 0 or lo12 != 0) {
        auto uop = new UopIBinImm;
        uop->SetImm(lo12);
        uop->SetLhs(upper);
        uop->SetDst(dst);
        uop->SetKind(IBIN_KIND::ADD);
        ctx.cur_blk->PushUop(uop);
    } else {
        auto uop = new UopMv;
        uop->SetSrc(upper);
        uop->SetDst(dst);
        ctx.cur_blk->PushUop(uop);
    }
}

void InternalTranslation::lf(VirtualRegister *dst, ConstValueInfo &cinfo, InternalTranslationContext &ctx) {
    if (cinfo.v32_.u32_ == 0) {
        auto uop = new UopLi;
        uop->SetImm(0);
        uop->SetDst(dst);

        ctx.cur_blk->PushUop(uop);
    } else {
        auto lc_idx = lc_map_.at(cinfo.v32_.u32_);

        if (auto fnd = ctx.fimm_map.find(cinfo.v32_.u32_); fnd != ctx.fimm_map.end()) {
            auto uop = new UopMv;
            uop->SetSrc(fnd->second);
            uop->SetDst(dst);

            ctx.cur_blk->PushUop(uop);
        } else {
            auto lbname = std::string(".LC") + std::to_string(lc_idx);

            auto uoplo = new UopFLoadLB;
            uoplo->SetHelper(nullptr);
            uoplo->SetSym(lbname);
            uoplo->SetDst(dst);

            ctx.cur_blk->PushUop(uoplo);

            ctx.fimm_map.emplace(cinfo.v32_.u32_, dst);
        }
    }
}

void RLPlanner::RegisterParams(ParamList &plst) {
    size_t cnt_i{0}, cnt_f{0}, cnt_stk{0};

    for (auto &&param : plst) {
        auto &&ptype = param->GetBaseType();
        auto &&var = dynamic_cast<Variable *>(param.get());
        Assert(var != nullptr, "param should not be constant");

        VirtualRegister *vr = nullptr;

        if (ptype->IsPointer()) {
            vr = AllocVReg(VREG_TYPE::PTR, var->GetVariableIdx());

            if (cnt_i < abi_info.i.nm_arg) {
                vr->SetRealRegIdx(cnt_i + abi_info.i.arg_bgn);
                vr->SetParamRegIdx(cnt_i);
                vr->SetOnStack(false);

                cnt_i += 1;
            } else {
                auto stk = Alloca(8);
                stk->SetParamOff(cnt_stk * 8);

                vr->SetStackInfo(stk);
                vr->SetOnStack(true);

                cnt_stk += 1;
            }
        } else if (ptype->FloatType()) {
            vr = AllocVReg(VREG_TYPE::FLT, var->GetVariableIdx());

            if (cnt_f < abi_info.f.nm_arg) {
                vr->SetRealRegIdx(cnt_f + abi_info.f.arg_bgn);
                vr->SetParamRegIdx(cnt_f);
                vr->SetOnStack(false);

                cnt_f += 1;
            } else {
                auto stk = Alloca(8);
                stk->SetParamOff(cnt_stk * 8);

                vr->SetStackInfo(stk);
                vr->SetOnStack(true);

                cnt_stk += 1;
            }
        } else if (ptype->IntType()) {
            vr = AllocVReg(VREG_TYPE::INT, var->GetVariableIdx());

            if (cnt_i < abi_info.i.nm_arg) {
                vr->SetRealRegIdx(cnt_i + abi_info.i.arg_bgn);
                vr->SetParamRegIdx(cnt_i);
                vr->SetOnStack(false);

                cnt_i += 1;
            } else {
                auto stk = Alloca(8);
                stk->SetParamOff(cnt_stk * 8);

                vr->SetStackInfo(stk);
                vr->SetOnStack(true);

                cnt_stk += 1;
            }
        }

        Assert(vr != nullptr, "illegel param");

        params_.push_back(vr);
    }
}

void InternalTranslation::Translate(ReturnInst *ll, InternalTranslationContext &ctx) {
    auto uop = new UopRet;

    if (auto &&retval = ll->GetRetValue(); retval != nullptr) {
        VirtualRegister *ret = nullptr;

        if (retval->IsVariable()) {
            auto var = dynamic_cast<Variable *>(retval.get());
            Assert(var, "bad dynamic cast");

            ret = ctx.planner->GetVReg(var->GetVariableIdx());
        } else if (retval->IsConstant()) {
            auto cst = dynamic_cast<Constant *>(retval.get());
            Assert(cst, "bad dynamic cast");

            auto &&cinfo = XConstValue(cst->GetValue());

            if (cinfo.isflt_) {
                ret = ctx.planner->NewVReg(VREG_TYPE::FLT);
                lf(ret, cinfo, ctx);
            } else {
                ret = ctx.planner->NewVReg(VREG_TYPE::INT);
                li(ret, cinfo, ctx);
            }
        } else {
            panic("unexpected");
        }

        uop->SetRetVal(ret);
    }

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(JumpInst *ll, InternalTranslationContext &ctx) {
    auto tgid = ll->GetTarget()->GetBlockIdx();

    if (ctx.nxt_cfg and ctx.nxt_cfg->GetBlockIdx() == tgid) {
        auto nop = new UopNop;
        ctx.cur_blk->PushUop(nop);
        return;
    }

    auto uop = new UopJump;
    uop->SetDstIdx(tgid);

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(BranchInst *ll, InternalTranslationContext &ctx) {
    auto nxtblkidx = ctx.nxt_cfg->GetBlockIdx();

    auto tridx = ll->GetTrueTarget()->GetBlockIdx();
    auto faidx = ll->GetFalseTarget()->GetBlockIdx();

    auto &&cond = ll->GetCondition();

    if (cond->IsConstant()) {
        panic("constant condition should be optimized");
    }

    auto var_cond = dynamic_cast<Variable *>(cond.get());
    auto cvridx = var_cond->GetVariableIdx();

    if (auto fnd = ctx.icmp_map.find(cvridx); fnd != ctx.icmp_map.end()) {
        auto icmp = fnd->second;

        auto lhs = icmp->GetLHS();
        auto rhs = icmp->GetRHS();

        if (lhs->IsConstant() and rhs->IsConstant()) {
            panic("unexpected");
        }

        VirtualRegister *vrlhs;
        VirtualRegister *vrrhs;

        if (lhs->IsConstant()) {
            auto cst = dynamic_cast<Constant *>(lhs.get());
            Assert(cst, "bad dynamic cast");

            auto &&cinfo = XConstValue(cst->GetValue());
            Assert(not cinfo.isflt_, "unexpected");

            if (cinfo.v32_.u32_ == 0) {
                vrlhs = nullptr;
            } else {
                vrlhs = ctx.planner->NewVReg(VREG_TYPE::INT);
                li(vrlhs, cinfo, ctx);
            }
        } else if (lhs->IsVariable()) {
            auto var = dynamic_cast<Variable *>(lhs.get());
            Assert(var, "bad dynamic cast");

            vrlhs = ctx.planner->GetVReg(var->GetVariableIdx());
        } else {
            panic("unexpected");
        }

        if (rhs->IsConstant()) {
            auto cst = dynamic_cast<Constant *>(rhs.get());
            Assert(cst, "bad dynamic cast");

            auto &&cinfo = XConstValue(cst->GetValue());
            Assert(not cinfo.isflt_, "unexpected");

            if (cinfo.v32_.u32_ == 0) {
                vrrhs = nullptr;
            } else {
                vrrhs = ctx.planner->NewVReg(VREG_TYPE::INT);

                li(vrrhs, cinfo, ctx);
            }
        } else if (rhs->IsVariable()) {
            auto var = dynamic_cast<Variable *>(rhs.get());
            Assert(var, "bad dynamic cast");

            vrrhs = ctx.planner->GetVReg(var->GetVariableIdx());
        } else {
            panic("unexpected");
        }

        if (vrlhs == nullptr and vrrhs == nullptr) {
            panic("unexpected");
        }

        auto uop = new UopICmpBranch;
        uop->SetLhs(vrlhs);
        uop->SetRhs(vrrhs);

        if (nxtblkidx == tridx) {
            switch (icmp->GetOpCode()) {
                case OP_EQU:
                    uop->SetKind(COMP_KIND::NEQ);
                    break;
                case OP_NEQ:
                    uop->SetKind(COMP_KIND::EQU);
                    break;
                case OP_LTH:
                    uop->SetKind(COMP_KIND::GEQ);
                    break;
                case OP_GTH:
                    uop->SetKind(COMP_KIND::LEQ);
                    break;
                case OP_LEQ:
                    uop->SetKind(COMP_KIND::GTH);
                    break;
                case OP_GEQ:
                    uop->SetKind(COMP_KIND::LTH);
                    break;
                default:
                    panic("unexpected");
            }
            uop->SetDstIdx(faidx);
        } else if (nxtblkidx == faidx) {
            uop->SetKind((COMP_KIND)icmp->GetOpCode());
            uop->SetDstIdx(tridx);
        } else {
            panic("unexpected");
        }

        ctx.cur_blk->PushUop(uop);

        return;
    }

    if (auto fnd = ctx.fcmp_map.find(cvridx); fnd != ctx.fcmp_map.end()) {
        auto ontrue = fnd->second;

        auto uop = new UopBranch;

        auto vr_cond = ctx.planner->GetVReg(var_cond->GetVariableIdx());
        uop->SetCond(vr_cond);

        if (nxtblkidx == tridx) {
            uop->SetOnTrue(not ontrue);
            uop->SetDstIdx(faidx);
        } else if (nxtblkidx == faidx) {
            uop->SetOnTrue(ontrue);
            uop->SetDstIdx(tridx);
        } else {
            panic("unexpected");
        }

        ctx.cur_blk->PushUop(uop);
        return;
    }

    panic("need fix");
}

void InternalTranslation::Translate(ICmpInst *ll, InternalTranslationContext &ctx) {
    // if (ll->IsIntegerBool()) {
    //     auto &&res = ctx.planner->AllocVReg(VREG_TYPE::INT, ll->GetResult()->GetVariableIdx());

    //     // assuming always true
    //     auto var = dynamic_cast<Variable *>(ll->GetLHS().get());
    //     auto lhs = ctx.planner->GetVReg(var->GetVariableIdx());

    //     auto uop = new UopIBin;
    //     uop->SetDst(res);
    //     uop->SetLhs(lhs);
    //     uop->SetRhs(nullptr);
    //     uop->SetKind(IBIN_KIND::LBOOL);
    //     ctx.cur_blk->PushUop(uop);
    // } else if (ll->IsIntegerNot()) {
    //     auto &&res = ctx.planner->AllocVReg(VREG_TYPE::INT, ll->GetResult()->GetVariableIdx());

    //     // assuming always true
    //     auto var = dynamic_cast<Variable *>(ll->GetLHS().get());
    //     auto lhs = ctx.planner->GetVReg(var->GetVariableIdx());

    //     auto uop = new UopIBin;
    //     uop->SetDst(res);
    //     uop->SetLhs(lhs);
    //     uop->SetRhs(nullptr);
    //     uop->SetKind(IBIN_KIND::LNOT);
    //     ctx.cur_blk->PushUop(uop);
    // }

    ctx.icmp_map.emplace(ll->GetResult()->GetVariableIdx(), ll);
}

void InternalTranslation::Translate(FCmpInst *ll, InternalTranslationContext &ctx) {
    auto &&res = ll->GetResult();
    auto lhs = ll->GetLHS();
    auto rhs = ll->GetRHS();
    auto opcode = ll->GetOpCode();

    auto ontrue = false;

    switch (opcode) {
        case OP_LEQ:
        case OP_LTH:
        case OP_EQU:
            ontrue = true;
            break;
        case OP_GEQ: {
            auto tmp = std::move(lhs);
            lhs = std::move(rhs);
            rhs = std::move(tmp);
        }
            opcode = OP_LEQ;
            ontrue = true;
            break;
        case OP_GTH: {
            auto tmp = std::move(lhs);
            lhs = std::move(rhs);
            rhs = std::move(tmp);
        }
            opcode = OP_LTH;
            ontrue = true;
            break;
        case OP_NEQ:
            opcode = OP_EQU;
            ontrue = false;
            break;
        default:
            panic("unexpected");
    }

    ctx.fcmp_map.emplace(res->GetVariableIdx(), ontrue);

    VirtualRegister *vrlhs;
    VirtualRegister *vrrhs;

    if (lhs->IsConstant()) {
        auto cst = dynamic_cast<Constant *>(lhs.get());
        Assert(cst, "bad dynamic cast");

        auto &&cinfo = XConstValue(cst->GetValue());
        Assert(cinfo.isflt_, "unexpected");

        vrlhs = ctx.planner->NewVReg(VREG_TYPE::FLT);
        lf(vrlhs, cinfo, ctx);
    } else {
        auto var = dynamic_cast<Variable *>(lhs.get());
        Assert(var, "bad dynamic cast");

        vrlhs = ctx.planner->GetVReg(var->GetVariableIdx());
    }

    if (rhs->IsConstant()) {
        auto cst = dynamic_cast<Constant *>(rhs.get());
        Assert(cst, "bad dynamic cast");

        auto &&cinfo = XConstValue(cst->GetValue());
        Assert(cinfo.isflt_, "unexpected");

        vrrhs = ctx.planner->NewVReg(VREG_TYPE::FLT);

        lf(vrrhs, cinfo, ctx);
    } else {
        auto var = dynamic_cast<Variable *>(rhs.get());
        Assert(var, "bad dynamic cast");

        vrrhs = ctx.planner->GetVReg(var->GetVariableIdx());
    }

    if (lhs->IsConstant() and rhs->IsConstant()) {
        panic("unexpected");
    }

    if (vrlhs == nullptr or vrrhs == nullptr) {
        panic("unexpected");
    }

    auto dst = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());

    auto uop = new UopFCmp;
    uop->SetLhs(vrlhs);
    uop->SetRhs(vrrhs);
    uop->SetDst(dst);
    uop->SetKind((COMP_KIND)opcode);

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(IBinaryInst *ll, InternalTranslationContext &ctx) {
    auto &&res = ll->GetResult();
    auto lhs = ll->GetLHS();
    auto rhs = ll->GetRHS();
    auto opcode = ll->GetOpCode();

    if (lhs->IsConstant() and rhs->IsConstant()) {
        panic("unexpected");
    }

    bool swappable = false;
    switch (opcode) {
        case OP_MUL:
        case OP_ADD:
            swappable = true;
            break;
        default:
            break;
    }

    VirtualRegister *vrlhs;
    VirtualRegister *vrrhs;

    if (lhs->IsConstant() and swappable) {
        std::swap(lhs, rhs);
    }

    if (lhs->IsConstant()) {
        auto cst = dynamic_cast<Constant *>(lhs.get());
        Assert(cst, "bad dynamic cast");

        auto &&cinfo = XConstValue(cst->GetValue());
        Assert(not cinfo.isflt_, "unexpected");

        vrlhs = ctx.planner->NewVReg(VREG_TYPE::INT);

        li(vrlhs, cinfo, ctx);
    } else if (lhs->IsVariable()) {
        auto var = dynamic_cast<Variable *>(lhs.get());
        Assert(var, "bad dynamic cast");

        vrlhs = ctx.planner->GetVReg(var->GetVariableIdx());
    } else {
        panic("unexpected");
    }

    if (rhs->IsConstant()) {
        auto cst = dynamic_cast<Constant *>(rhs.get());
        Assert(cst, "bad dynamic cast");

        auto &&cinfo = XConstValue(cst->GetValue());
        Assert(not cinfo.isflt_, "unexpected");

        // if div/rem is too slow
        // change if log2(imm) <= 36 - (additional operation)

        if (ImmWithin(12, cinfo.v32_.u32_)) {
            switch (opcode) {
                case OP_MUL:
                    if (__builtin_popcount(cinfo.v32_.u32_) == 1) {
                        auto ctz = __builtin_ctz(cinfo.v32_.u32_);

                        auto dst = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());
                        auto uop_imm = new UopIBinImm;
                        uop_imm->SetImm(ctz);
                        uop_imm->SetLhs(vrlhs);
                        uop_imm->SetDst(dst);
                        uop_imm->SetKind(IBIN_KIND::SLL);

                        ctx.cur_blk->PushUop(uop_imm);

                        return;
                    }
                    break;

                case OP_DIV:
                    if (__builtin_popcount(cinfo.v32_.u32_) == 1) {
                        auto ctz = __builtin_ctz(cinfo.v32_.u32_);

                        auto dst = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());

                        auto c1 = ctx.planner->NewVReg(VREG_TYPE::INT);
                        auto uop_sraiw = new UopIBinImm;
                        uop_sraiw->SetImm(31);
                        uop_sraiw->SetLhs(vrlhs);
                        uop_sraiw->SetDst(c1);
                        uop_sraiw->SetKind(IBIN_KIND::SRA);
                        ctx.cur_blk->PushUop(uop_sraiw);

                        auto c2 = ctx.planner->NewVReg(VREG_TYPE::INT);
                        auto uop_slliw = new UopIBinImm;
                        uop_slliw->SetImm(32 - ctz);
                        uop_slliw->SetLhs(c1);
                        uop_slliw->SetDst(c2);
                        uop_slliw->SetKind(IBIN_KIND::SLL);
                        ctx.cur_blk->PushUop(uop_slliw);

                        auto c3 = ctx.planner->NewVReg(VREG_TYPE::INT);
                        auto uop_srliw = new UopIBinImm;
                        uop_srliw->SetImm(32 - ctz);
                        uop_srliw->SetLhs(c2);
                        uop_srliw->SetDst(c3);
                        uop_srliw->SetKind(IBIN_KIND::SRL);
                        ctx.cur_blk->PushUop(uop_srliw);

                        auto b1 = ctx.planner->NewVReg(VREG_TYPE::INT);
                        auto uop_addw = new UopIBin;
                        uop_addw->SetRhs(c3);
                        uop_addw->SetLhs(vrlhs);
                        uop_addw->SetDst(b1);
                        uop_addw->SetKind(IBIN_KIND::ADD);
                        ctx.cur_blk->PushUop(uop_addw);

                        auto uop_res = new UopIBinImm;
                        uop_res->SetImm(ctz);
                        uop_res->SetLhs(b1);
                        uop_res->SetDst(dst);
                        uop_res->SetKind(IBIN_KIND::SRA);
                        ctx.cur_blk->PushUop(uop_res);

                        return;
                    }
                    break;

                case OP_REM:
                    if (__builtin_popcount(cinfo.v32_.u32_) == 1) {
                        auto ctz = __builtin_ctz(cinfo.v32_.u32_);

                        auto msk = (1 << ctz) - 1;

                        auto dst = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());

                        if (ImmWithin(12, msk)) {
                            auto c1 = ctx.planner->NewVReg(VREG_TYPE::INT);
                            auto uop_sraiw = new UopIBinImm;
                            uop_sraiw->SetImm(31);
                            uop_sraiw->SetLhs(vrlhs);
                            uop_sraiw->SetDst(c1);
                            uop_sraiw->SetKind(IBIN_KIND::SRA);
                            ctx.cur_blk->PushUop(uop_sraiw);

                            auto c2 = ctx.planner->NewVReg(VREG_TYPE::INT);
                            auto uop_srliw0 = new UopIBinImm;
                            uop_srliw0->SetImm(32 - ctz);
                            uop_srliw0->SetLhs(c1);
                            uop_srliw0->SetDst(c2);
                            uop_srliw0->SetKind(IBIN_KIND::SRL);
                            ctx.cur_blk->PushUop(uop_srliw0);

                            auto b1 = ctx.planner->NewVReg(VREG_TYPE::INT);
                            auto uop_addw = new UopIBin;
                            uop_addw->SetRhs(c2);
                            uop_addw->SetLhs(vrlhs);
                            uop_addw->SetDst(b1);
                            uop_addw->SetKind(IBIN_KIND::ADD);
                            ctx.cur_blk->PushUop(uop_addw);

                            auto b2 = ctx.planner->NewVReg(VREG_TYPE::INT);
                            auto uop_andi = new UopIBinImm;
                            uop_andi->SetImm(msk);
                            uop_andi->SetLhs(b1);
                            uop_andi->SetDst(b2);
                            uop_andi->SetKind(IBIN_KIND::AND);
                            ctx.cur_blk->PushUop(uop_andi);

                            auto uop_res = new UopIBin;
                            uop_res->SetRhs(c2);
                            uop_res->SetLhs(b2);
                            uop_res->SetDst(dst);
                            uop_res->SetKind(IBIN_KIND::SUB);
                            ctx.cur_blk->PushUop(uop_res);
                        } else {
                            auto c1 = ctx.planner->NewVReg(VREG_TYPE::INT);
                            auto uop_sraiw = new UopIBinImm;
                            uop_sraiw->SetImm(31);
                            uop_sraiw->SetLhs(vrlhs);
                            uop_sraiw->SetDst(c1);
                            uop_sraiw->SetKind(IBIN_KIND::SRA);
                            ctx.cur_blk->PushUop(uop_sraiw);

                            auto c2 = ctx.planner->NewVReg(VREG_TYPE::INT);
                            auto uop_srliw0 = new UopIBinImm;
                            uop_srliw0->SetImm(32 - ctz);
                            uop_srliw0->SetLhs(c1);
                            uop_srliw0->SetDst(c2);
                            uop_srliw0->SetKind(IBIN_KIND::SRL);
                            ctx.cur_blk->PushUop(uop_srliw0);

                            auto b1 = ctx.planner->NewVReg(VREG_TYPE::INT);
                            auto uop_addw = new UopIBin;
                            uop_addw->SetRhs(c2);
                            uop_addw->SetLhs(vrlhs);
                            uop_addw->SetDst(b1);
                            uop_addw->SetKind(IBIN_KIND::ADD);
                            ctx.cur_blk->PushUop(uop_addw);

                            auto b2 = ctx.planner->NewVReg(VREG_TYPE::INT);
                            auto uop_slliw = new UopIBinImm;
                            uop_slliw->SetImm(32 - ctz);
                            uop_slliw->SetLhs(b1);
                            uop_slliw->SetDst(b2);
                            uop_slliw->SetKind(IBIN_KIND::SLL);
                            ctx.cur_blk->PushUop(uop_slliw);

                            auto b3 = ctx.planner->NewVReg(VREG_TYPE::INT);
                            auto uop_srliw1 = new UopIBinImm;
                            uop_srliw1->SetImm(32 - ctz);
                            uop_srliw1->SetLhs(b2);
                            uop_srliw1->SetDst(b3);
                            uop_srliw1->SetKind(IBIN_KIND::SRL);
                            ctx.cur_blk->PushUop(uop_srliw1);

                            auto uop_res = new UopIBin;
                            uop_res->SetRhs(c2);
                            uop_res->SetLhs(b3);
                            uop_res->SetDst(dst);
                            uop_res->SetKind(IBIN_KIND::SUB);
                            ctx.cur_blk->PushUop(uop_res);
                        }
                        return;
                    }
                    break;

                case OP_ADD: {
                    auto dst = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());
                    auto uop_imm = new UopIBinImm;
                    uop_imm->SetImm(cinfo.v32_.i32_);
                    uop_imm->SetLhs(vrlhs);
                    uop_imm->SetDst(dst);
                    uop_imm->SetKind(IBIN_KIND::ADD);

                    ctx.cur_blk->PushUop(uop_imm);

                    return;
                } break;

                case OP_SUB:
                    if (ImmWithin(12, -cinfo.v32_.i32_)) {
                        auto dst = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());
                        auto uop_imm = new UopIBinImm;
                        uop_imm->SetImm(-cinfo.v32_.i32_);
                        uop_imm->SetLhs(vrlhs);
                        uop_imm->SetDst(dst);
                        uop_imm->SetKind(IBIN_KIND::ADD);

                        ctx.cur_blk->PushUop(uop_imm);

                        return;
                    }
                    break;

                default:
                    break;
            };
        }

        vrrhs = ctx.planner->NewVReg(VREG_TYPE::INT);

        if (opcode == OP_DIV and std::log(cinfo.v32_.i32_) <= 16) {
            auto magic = Magika(cinfo.v32_.i32_);
            ConstValueInfo cvi;
            cvi.width_ = 32;
            cvi.v32_.i32_ = magic.magic_number;

            li(vrrhs, cvi, ctx);

            auto mulres = ctx.planner->NewVReg(VREG_TYPE::INT);
            auto upper32 = ctx.planner->NewVReg(VREG_TYPE::INT);

            auto op0 = new UopIBin64;
            op0->SetLhs(vrrhs);
            op0->SetRhs(vrlhs);
            op0->SetDst(mulres);
            op0->SetKind(IBIN_KIND::MUL);
            ctx.cur_blk->PushUop(op0);

            auto opsh = new UopIBinImm64;
            opsh->SetImm(32);
            opsh->SetLhs(mulres);
            opsh->SetDst(upper32);
            opsh->SetKind(IBIN_KIND::SRL);
            ctx.cur_blk->PushUop(opsh);

            auto inter1 = ctx.planner->NewVReg(VREG_TYPE::INT);
            if (cinfo.v32_.i32_ > 0 and magic.magic_number < 0) {
                auto op1 = new UopIBin;
                op1->SetLhs(upper32);
                op1->SetRhs(vrlhs);
                op1->SetDst(inter1);
                op1->SetKind(IBIN_KIND::ADD);
                ctx.cur_blk->PushUop(op1);

            } else if (cinfo.v32_.i32_ < 0 and magic.magic_number > 0) {
                auto op1 = new UopIBin;
                op1->SetLhs(upper32);
                op1->SetRhs(vrlhs);
                op1->SetDst(inter1);
                op1->SetKind(IBIN_KIND::SUB);
                ctx.cur_blk->PushUop(op1);
            } else {
                auto op1 = new UopMv;
                op1->SetSrc(upper32);
                op1->SetDst(inter1);
                ctx.cur_blk->PushUop(op1);
            }

            auto inter2 = ctx.planner->NewVReg(VREG_TYPE::INT);
            if (magic.shift_amount > 0) {
                auto op2 = new UopIBinImm;
                op2->SetImm(magic.shift_amount);
                op2->SetLhs(inter1);
                op2->SetDst(inter2);
                op2->SetKind(IBIN_KIND::SRA);
                ctx.cur_blk->PushUop(op2);
            } else {
                auto op2 = new UopMv;
                op2->SetSrc(inter1);
                op2->SetDst(inter2);
                ctx.cur_blk->PushUop(op2);
            }

            auto sgn = ctx.planner->NewVReg(VREG_TYPE::INT);
            auto op3 = new UopIBinImm;
            op3->SetImm(31);
            op3->SetLhs(inter2);
            op3->SetDst(sgn);
            op3->SetKind(IBIN_KIND::SRL);
            ctx.cur_blk->PushUop(op3);

            auto dst = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());
            auto op4 = new UopIBin;
            op4->SetLhs(sgn);
            op4->SetRhs(inter2);
            op4->SetDst(dst);
            op4->SetKind(IBIN_KIND::ADD);
            ctx.cur_blk->PushUop(op4);

            return;
        } else if (opcode == OP_REM and std::log(cinfo.v32_.i32_) <= 16) {
            auto magic = Magika(cinfo.v32_.i32_);
            ConstValueInfo cvi;
            cvi.width_ = 32;
            cvi.v32_.i32_ = magic.magic_number;

            li(vrrhs, cvi, ctx);

            auto mulres = ctx.planner->NewVReg(VREG_TYPE::INT);
            auto upper32 = ctx.planner->NewVReg(VREG_TYPE::INT);

            auto op0 = new UopIBin64;
            op0->SetLhs(vrrhs);
            op0->SetRhs(vrlhs);
            op0->SetDst(mulres);
            op0->SetKind(IBIN_KIND::MUL);
            ctx.cur_blk->PushUop(op0);

            auto opsh = new UopIBinImm64;
            opsh->SetImm(32);
            opsh->SetLhs(mulres);
            opsh->SetDst(upper32);
            opsh->SetKind(IBIN_KIND::SRL);
            ctx.cur_blk->PushUop(opsh);

            auto inter1 = ctx.planner->NewVReg(VREG_TYPE::INT);
            if (cinfo.v32_.i32_ > 0 and magic.magic_number < 0) {
                auto op1 = new UopIBin;
                op1->SetLhs(upper32);
                op1->SetRhs(vrlhs);
                op1->SetDst(inter1);
                op1->SetKind(IBIN_KIND::ADD);
                ctx.cur_blk->PushUop(op1);

            } else if (cinfo.v32_.i32_ < 0 and magic.magic_number > 0) {
                auto op1 = new UopIBin;
                op1->SetLhs(upper32);
                op1->SetRhs(vrlhs);
                op1->SetDst(inter1);
                op1->SetKind(IBIN_KIND::SUB);
                ctx.cur_blk->PushUop(op1);
            } else {
                auto op1 = new UopMv;
                op1->SetSrc(upper32);
                op1->SetDst(inter1);
                ctx.cur_blk->PushUop(op1);
            }

            auto inter2 = ctx.planner->NewVReg(VREG_TYPE::INT);
            if (magic.shift_amount > 0) {
                auto op2 = new UopIBinImm;
                op2->SetImm(magic.shift_amount);
                op2->SetLhs(inter1);
                op2->SetDst(inter2);
                op2->SetKind(IBIN_KIND::SRA);
                ctx.cur_blk->PushUop(op2);
            } else {
                auto op2 = new UopMv;
                op2->SetSrc(inter1);
                op2->SetDst(inter2);
                ctx.cur_blk->PushUop(op2);
            }

            auto sgn = ctx.planner->NewVReg(VREG_TYPE::INT);
            auto op3 = new UopIBinImm;
            op3->SetImm(31);
            op3->SetLhs(inter2);
            op3->SetDst(sgn);
            op3->SetKind(IBIN_KIND::SRL);
            ctx.cur_blk->PushUop(op3);

            auto q = ctx.planner->NewVReg(VREG_TYPE::INT);
            auto op4 = new UopIBin;
            op4->SetLhs(sgn);
            op4->SetRhs(inter2);
            op4->SetDst(q);
            op4->SetKind(IBIN_KIND::ADD);
            ctx.cur_blk->PushUop(op4);

            auto t = ctx.planner->NewVReg(VREG_TYPE::INT);
            auto d = ctx.planner->NewVReg(VREG_TYPE::INT);
            li(d, cinfo, ctx);

            auto op5 = new UopIBin;
            op5->SetLhs(q);
            op5->SetRhs(d);
            op5->SetDst(t);
            op5->SetKind(IBIN_KIND::MUL);
            ctx.cur_blk->PushUop(op5);

            auto dst = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());
            auto op6 = new UopIBin;
            op6->SetLhs(vrlhs);
            op6->SetRhs(t);
            op6->SetDst(dst);
            op6->SetKind(IBIN_KIND::SUB);
            ctx.cur_blk->PushUop(op6);

            return;
        } else {
            li(vrrhs, cinfo, ctx);
        }

    } else if (rhs->IsVariable()) {
        auto var = dynamic_cast<Variable *>(rhs.get());
        Assert(var, "bad dynamic cast");

        vrrhs = ctx.planner->GetVReg(var->GetVariableIdx());
    } else {
        panic("unexpected");
    }

    if (vrlhs == nullptr or vrrhs == nullptr) {
        panic("unexpected");
    }

    auto dst = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());

    auto uop = new UopIBin;
    uop->SetLhs(vrlhs);
    uop->SetRhs(vrrhs);
    uop->SetDst(dst);
    uop->SetKind((IBIN_KIND)opcode);

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(FBinaryInst *ll, InternalTranslationContext &ctx) {
    auto &&res = ll->GetResult();
    auto lhs = ll->GetLHS();
    auto rhs = ll->GetRHS();
    auto opcode = ll->GetOpCode();

    if (lhs->IsConstant() and rhs->IsConstant()) {
        panic("unexpected");
    }

    VirtualRegister *vrlhs;
    VirtualRegister *vrrhs;

    if (lhs->IsConstant()) {
        auto cst = dynamic_cast<Constant *>(lhs.get());
        Assert(cst, "bad dynamic cast");

        auto &&cinfo = XConstValue(cst->GetValue());
        Assert(cinfo.isflt_, "unexpected");

        vrlhs = ctx.planner->NewVReg(VREG_TYPE::FLT);

        lf(vrlhs, cinfo, ctx);
    } else {
        auto var = dynamic_cast<Variable *>(lhs.get());
        Assert(var, "bad dynamic cast");

        vrlhs = ctx.planner->GetVReg(var->GetVariableIdx());
    }

    if (rhs->IsConstant()) {
        auto cst = dynamic_cast<Constant *>(rhs.get());
        Assert(cst, "bad dynamic cast");

        auto &&cinfo = XConstValue(cst->GetValue());
        Assert(cinfo.isflt_, "unexpected");

        vrrhs = ctx.planner->NewVReg(VREG_TYPE::FLT);

        lf(vrrhs, cinfo, ctx);
    } else {
        auto var = dynamic_cast<Variable *>(rhs.get());
        Assert(var, "bad dynamic cast");

        vrrhs = ctx.planner->GetVReg(var->GetVariableIdx());
    }

    if (vrlhs == nullptr or vrrhs == nullptr) {
        panic("unexpected");
    }

    auto dst = ctx.planner->AllocVReg(VREG_TYPE::FLT, res->GetVariableIdx());

    auto uop = new UopFBin;
    uop->SetLhs(vrlhs);
    uop->SetRhs(vrrhs);
    uop->SetDst(dst);
    uop->SetKind((FBIN_KIND)opcode);

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(SitoFpInst *ll, InternalTranslationContext &ctx) {
    auto oprand = ll->GetOprand();
    auto var = dynamic_cast<Variable *>(oprand.get());
    Assert(var, "SitoFp should op on var");

    auto &&res = ll->GetResult();

    auto rsvr = ctx.planner->GetVReg(var->GetVariableIdx());
    auto nwvr = ctx.planner->AllocVReg(VREG_TYPE::FLT, res->GetVariableIdx());

    auto uop = new UopCvtW2S;
    uop->SetSrc(rsvr);
    uop->SetDst(nwvr);

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(FptoSiInst *ll, InternalTranslationContext &ctx) {
    auto oprand = ll->GetOprand();
    auto var = dynamic_cast<Variable *>(oprand.get());
    Assert(var, "FptoSi should op on var");

    auto &&res = ll->GetResult();

    auto rsvr = ctx.planner->GetVReg(var->GetVariableIdx());
    auto nwvr = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());

    auto uop = new UopCvtS2W;
    uop->SetSrc(rsvr);
    uop->SetDst(nwvr);

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(ZextInst *ll, InternalTranslationContext &ctx) {
    auto oprand = ll->GetOprand();
    auto var = dynamic_cast<Variable *>(oprand.get());
    Assert(var, "bitcast should op on var");

    ctx.planner->Link(ll->GetResult()->GetVariableIdx(), var->GetVariableIdx());
}

void InternalTranslation::Translate(BitCastInst *ll, InternalTranslationContext &ctx) {
    auto oprand = ll->GetOprand();
    auto var = dynamic_cast<Variable *>(oprand.get());
    Assert(var, "bitcast should op on var");

    auto &&res = ll->GetResult();

    auto varidx = var->GetVariableIdx();
    if (auto fnd = ctx.gep_map.find(varidx); fnd != ctx.gep_map.end()) {
        auto &&[vridx, offset] = fnd->second;

        if (offset == 0) {
            ctx.planner->Link(ll->GetResult()->GetVariableIdx(), vridx);
            return;
        }

        VirtualRegister *nwvr = ctx.planner->AllocVReg(VREG_TYPE::PTR, res->GetVariableIdx());
        VirtualRegister *base = ctx.planner->GetVReg(vridx);

        auto uop = new UopIBinImm64;
        uop->SetImm(offset);
        uop->SetLhs(base);
        uop->SetDst(nwvr);
        uop->SetKind(IBIN_KIND::ADD);

        ctx.cur_blk->PushUop(uop);
        return;
    }

    ctx.planner->Link(ll->GetResult()->GetVariableIdx(), var->GetVariableIdx());
}

void InternalTranslation::Translate(AllocaInst *ll, InternalTranslationContext &ctx) {
    if (auto &&type = ll->GetAllocaType(); type->IsScalar()) {
        size_t slen = 0;
        if (type->IsPointer()) {
            slen = 8;
        } else if (type->IntType() or type->FloatType()) {
            slen = 4;
        } else {
            panic("unexpected");
        }

        auto stkvr = ctx.planner->Alloca(ll->GetResult()->GetVariableIdx(), slen);
        auto uop = new UopLla;
        uop->SetDst(stkvr);

        ctx.cur_blk->PushUop(uop);
    } else {
        auto lstty = dynamic_cast<ListType *>(type.get());
        Assert(lstty, "bad dynamic cast");

        if (type->IsPointer()) {
            panic("unexpected");
        }

        auto stkvr = ctx.planner->Alloca(ll->GetResult()->GetVariableIdx(), lstty->GetCapacity() * 4);
        auto uop = new UopLla;
        uop->SetDst(stkvr);

        ctx.cur_blk->PushUop(uop);
    }
}

void InternalTranslation::Translate(StoreInst *ll, InternalTranslationContext &ctx) {
    auto &&s_val = ll->GetStoreValue();
    auto &&s_addr = ll->GetStoreAddr();

    VirtualRegister *srcvr = nullptr;
    bool onflt = false;

    if (s_val->IsVariable()) {
        auto var = dynamic_cast<Variable *>(s_val.get());
        Assert(var, "bad dynamic cast");

        onflt = s_val->GetBaseType()->FloatType();
        srcvr = ctx.planner->GetVReg(var->GetVariableIdx());
    } else if (s_val->IsConstant()) {
        auto cst = dynamic_cast<Constant *>(s_val.get());
        Assert(cst, "bad dynamic cast");

        auto &&cinfo = XConstValue(cst->GetValue());

        if (cinfo.v32_.u32_ == 0) {
            srcvr = nullptr;
        } else {
            srcvr = ctx.planner->NewVReg(VREG_TYPE::INT);

            li(srcvr, cinfo, ctx);
        }
    }

    if (s_addr->IsGlobalValue()) {
        panic("try another way please");
    } else {
        auto addr = dynamic_cast<Variable *>(s_addr.get());
        Assert(addr, "store addr should be var");

        auto avridx = addr->GetVariableIdx();

        auto fnd = ctx.gep_map.find(avridx);

        if (fnd != ctx.gep_map.end()) {
            auto &&[vridx, offset] = fnd->second;

            auto base = ctx.planner->GetVReg(vridx);

            if (onflt) {
                auto uop = new UopFStore;
                uop->SetBase(base);
                uop->SetSrc(srcvr);
                uop->SetOff(offset);

                ctx.cur_blk->PushUop(uop);
            } else {
                auto uop = new UopStore;
                uop->SetBase(base);
                uop->SetSrc(srcvr);
                uop->SetOff(offset);

                ctx.cur_blk->PushUop(uop);
            }
        } else {
            auto base = ctx.planner->GetVReg(avridx);

            if (onflt) {
                auto uop = new UopFStore;
                uop->SetBase(base);
                uop->SetSrc(srcvr);
                uop->SetOff(0);

                ctx.cur_blk->PushUop(uop);
            } else {
                auto uop = new UopStore;
                uop->SetBase(base);
                uop->SetSrc(srcvr);
                uop->SetOff(0);

                ctx.cur_blk->PushUop(uop);
            }
        }
    }
}

void InternalTranslation::Translate(LoadInst *ll, InternalTranslationContext &ctx) {
    auto &&l_dst = ll->GetResult();
    auto &&l_addr = ll->GetOprand();

    VirtualRegister *dstvr = nullptr;
    VREG_TYPE vtype;

    bool onflt = false;
    if (auto type = l_dst->GetBaseType(); type->FloatType()) {
        onflt = true;
        vtype = VREG_TYPE::FLT;
    } else {
        vtype = VREG_TYPE::INT;
    }
    dstvr = ctx.planner->AllocVReg(vtype, l_dst->GetVariableIdx());

    if (l_addr->IsGlobalValue()) {
        panic("try another way please");
    } else {
        auto addr = dynamic_cast<Variable *>(l_addr.get());
        Assert(addr, "load addr should be var");

        auto avridx = addr->GetVariableIdx();

        auto fnd = ctx.gep_map.find(avridx);

        if (fnd != ctx.gep_map.end()) {
            auto &&[vridx, offset] = fnd->second;

            auto base = ctx.planner->GetVReg(vridx);

            if (onflt) {
                auto uop = new UopFLoad;
                uop->SetBase(base);
                uop->SetDst(dstvr);
                uop->SetOff(offset);

                ctx.cur_blk->PushUop(uop);
            } else {
                auto uop = new UopLoad;
                uop->SetBase(base);
                uop->SetDst(dstvr);
                uop->SetOff(offset);

                ctx.cur_blk->PushUop(uop);
            }
        } else {
            auto base = ctx.planner->GetVReg(avridx);

            if (onflt) {
                auto uop = new UopFLoad;
                uop->SetBase(base);
                uop->SetDst(dstvr);
                uop->SetOff(0);

                ctx.cur_blk->PushUop(uop);
            } else {
                auto uop = new UopLoad;
                uop->SetBase(base);
                uop->SetDst(dstvr);
                uop->SetOff(0);

                ctx.cur_blk->PushUop(uop);
            }
        }
    }
}

void InternalTranslation::Translate(GetElementPtrInst *ll, InternalTranslationContext &ctx) {
    auto &&res = ll->GetResult();

    VirtualRegister *off = nullptr;
    auto &&offlst = ll->GetOffList();
    auto &&offset = offlst.back();
    auto &&base = ll->GetBaseAddr();

    bool immoff = false;
    uint32_t imm = 0;

    if (offset->IsVariable()) {
        auto var = dynamic_cast<Variable *>(offset.get());
        Assert(var, "bad dynamic cast");

        auto offnum = ctx.planner->GetVReg(var->GetVariableIdx());
        off = ctx.planner->NewVReg(VREG_TYPE::PTR);

        auto uop_slli = new UopIBinImm64;
        uop_slli->SetLhs(offnum);
        uop_slli->SetImm(2);
        uop_slli->SetDst(off);
        uop_slli->SetKind(IBIN_KIND::SLL);

        ctx.cur_blk->PushUop(uop_slli);
    } else if (offset->IsConstant()) {
        auto cst = dynamic_cast<Constant *>(offset.get());
        Assert(cst, "bad dynamic cast");

        auto &&cinfo = XConstValue(cst->GetValue());

        if (cinfo.width_ == 32) {
            cinfo.v32_.u32_ <<= 2;
            imm = cinfo.v32_.u32_;
        } else {
            cinfo.v64_.u64_ <<= 2;
            imm = cinfo.v64_.u64_;
        }

        if (not base->IsGlobalValue()) {
            if (ImmWithin(12, imm)) {
                immoff = true;

                bool gain = true;

                for (auto &&use : res->GetUserList()) {
                    switch (use->GetOpCode()) {
                        case BitCast:
                        case Load:
                        case Store:
                            break;
                        default:
                            gain = false;
                            break;
                    }
                    if (not gain) {
                        break;
                    }
                }

                if (base->IsVariable() and gain) {
                    auto addr = dynamic_cast<Variable *>(base.get());
                    Assert(addr, "bad dynamic cast");

                    auto vridx = res->GetVariableIdx();
                    ctx.gep_map[vridx] = std::make_pair(addr->GetVariableIdx(), (size_t)imm);
                    return;
                }
            } else {
                off = ctx.planner->NewVReg(VREG_TYPE::PTR);

                li(off, cinfo, ctx);
            }
        }
    } else {
        panic("unexpected");
    }

    if (base->IsGlobalValue()) {
        auto gv = dynamic_cast<GlobalValue *>(base.get());
        Assert(gv, "bad dynamic cast");

        auto resvr = ctx.planner->AllocVReg(VREG_TYPE::PTR, res->GetVariableIdx());
        auto gvidx = gv->GetGlobalValueIdx();

        if (off == nullptr) {
            auto glb_name = std::string(gv_map_.at(gvidx)->Label());
            auto uop_lla = new UopLla;
            uop_lla->SetSrc(glb_name);
            uop_lla->SetOff(imm);
            uop_lla->SetDst(resvr);
            ctx.cur_blk->PushUop(uop_lla);
        } else {
            auto glb_addr = ctx.planner->NewVReg(VREG_TYPE::PTR);
            auto glb_name = std::string(gv_map_.at(gvidx)->Label());
            auto uop_lla = new UopLla;
            uop_lla->SetSrc(glb_name);
            uop_lla->SetDst(glb_addr);
            uop_lla->SetOff(0);

            auto uop_add = new UopIBin64;
            uop_add->SetLhs(glb_addr);
            uop_add->SetRhs(off);
            uop_add->SetDst(resvr);
            uop_add->SetKind(IBIN_KIND::ADD);

            ctx.cur_blk->PushUop(uop_lla);
            ctx.cur_blk->PushUop(uop_add);
        }
    } else if (base->IsVariable()) {
        auto addr = dynamic_cast<Variable *>(base.get());
        Assert(addr, "bad dynamic cast");

        auto vr_addr = ctx.planner->GetVReg(addr->GetVariableIdx());

        if (immoff) {
            if (imm == 0) {
                ctx.planner->Link(res->GetVariableIdx(), vr_addr->GetVRIdx());
            } else {
                auto resvr = ctx.planner->AllocVReg(VREG_TYPE::PTR, res->GetVariableIdx());
                auto uop_add = new UopIBinImm64;
                uop_add->SetLhs(vr_addr);
                uop_add->SetImm(imm);
                uop_add->SetDst(resvr);
                uop_add->SetKind(IBIN_KIND::ADD);

                ctx.cur_blk->PushUop(uop_add);
            }
        } else {
            auto resvr = ctx.planner->AllocVReg(VREG_TYPE::PTR, res->GetVariableIdx());
            auto uop_add = new UopIBin64;
            uop_add->SetRhs(vr_addr);
            uop_add->SetLhs(off);
            uop_add->SetDst(resvr);
            uop_add->SetKind(IBIN_KIND::ADD);

            ctx.cur_blk->PushUop(uop_add);
        }
    } else {
        panic("unexpected");
    }
}

void InternalTranslation::Translate(CallInst *ll, InternalTranslationContext &ctx) {
    ctx.meet_call = true;

    // decide if is tail call

    auto &&params = ll->GetParamList();
    auto &&callee = ll->GetCalleeFunc();

    auto uop = new UopCall;

    if (callee->GetFuncName() != fptr_->GetFuncName()) {
        rlps_->MeetCallOther();
        uop->SetCallSelf(false);
    } else {
        uop->SetCallSelf(true);
    }

    if (ll->GetTailCall()) {
        uop->SetTailCall(true);
        ctx.meet_tail = true;
    } else {
        uop->SetTailCall(false);
    }

    auto make_param = [&params, &uop, &ctx, this](size_t num) {
        size_t pcnt = 0;

        for (auto &&param : params) {
            auto &&ptype = param->GetBaseType();

            if (param->IsVariable()) {
                auto var = dynamic_cast<Variable *>(param.get());
                Assert(var, "bad dynamic cast");

                auto vr_param = ctx.planner->GetVReg(var->GetVariableIdx());
                uop->PushParam(vr_param);
            } else if (param->IsConstant()) {
                auto cst = dynamic_cast<Constant *>(param.get());
                Assert(cst, "bad dynamic cast");

                auto &&cinfo = XConstValue(cst->GetValue());

                if (cinfo.isflt_) {
                    auto vr_param = ctx.planner->NewVReg(VREG_TYPE::FLT);
                    lf(vr_param, cinfo, ctx);
                    uop->PushParam(vr_param);
                } else {
                    auto vr_param = ctx.planner->NewVReg(VREG_TYPE::INT);
                    li(vr_param, cinfo, ctx);
                    uop->PushParam(vr_param);
                }
            } else {
                panic("unexpected");
            }

            pcnt += 1;
            if (pcnt >= num) {
                break;
            }
        }
    };

    if (callee->IsLibFunction()) {
        bool meet = false;

        do /* sylib */ {
            auto sylib_func = dynamic_cast<SYSYLibFunction *>(callee.get());
            if (sylib_func == nullptr) {
                break;
            }

            uop->SetLibCall(true);

            rlps_->MeetLibCall();

            uop->SetCallee(sylib_func->GetFuncName());

            make_param(-1);

            meet = true;
        } while (0);

        do /* stdlib */ {
            auto llvm_func = dynamic_cast<LLVMLibFunction *>(callee.get());
            if (llvm_func == nullptr) {
                break;
            }

            if (llvm_func->GetFuncName() == "llvm.memset.p0i8.i64") {
                auto fnn = std::string("memset");
                uop->SetCallee(fnn);

                make_param(3);

                meet = true;
            }
        } while (0);

        if (not meet) {
            panic("unrecognizable");
        }
    }

    if (not callee->IsLibFunction() and not params.empty()) {
        uop->SetLibCall(false);
        uop->SetCallee(callee->GetFuncName());

        make_param(-1);
    }

    if (auto &&res = ll->GetResult(); res != nullptr and not res->GetUserList().empty()) {
        VirtualRegister *retvr;
        VREG_TYPE vtype;
        if (auto btype = res->GetBaseType(); btype->FloatType()) {
            vtype = VREG_TYPE::FLT;
        } else if (btype->IntType()) {
            vtype = VREG_TYPE::INT;
        } else {
            panic("unexpected");
        }
        retvr = ctx.planner->AllocVReg(vtype, res->GetVariableIdx());
        uop->SetRetVal(retvr);
    }

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(FNegInst *ll, InternalTranslationContext &ctx) {
    auto uop = new UopFBin;

    auto res = ll->GetResult();
    auto operand = ll->GetOprand();

    auto srcopd = dynamic_cast<Variable *>(operand.get());
    Assert(srcopd != nullptr, "should be fold");

    auto dst = ctx.planner->AllocVReg(VREG_TYPE::FLT, res->GetVariableIdx());
    auto src = ctx.planner->GetVReg(srcopd->GetVariableIdx());

    uop->SetDst(dst);
    uop->SetLhs(src);
    uop->SetRhs(nullptr);
    uop->SetKind(FBIN_KIND::NEG);

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(FAbsInst *ll, InternalTranslationContext &ctx) {
    auto uop = new UopFBin;

    auto res = ll->GetResult();
    auto operand = ll->GetOprand();

    auto srcopd = dynamic_cast<Variable *>(operand.get());
    Assert(srcopd != nullptr, "should be fold");

    auto dst = ctx.planner->AllocVReg(VREG_TYPE::FLT, res->GetVariableIdx());
    auto src = ctx.planner->GetVReg(srcopd->GetVariableIdx());

    uop->SetDst(dst);
    uop->SetLhs(src);
    uop->SetRhs(nullptr);
    uop->SetKind(FBIN_KIND::ABS);

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::Translate(PhiInst *ll, InternalTranslationContext &ctx) {
    auto res = ll->GetResult();
    VirtualRegister *mux, *recv;

    if (res->GetBaseType()->IsPointer()) {
        mux = ctx.planner->NewVReg(VREG_TYPE::PTR);
        recv = ctx.planner->AllocVReg(VREG_TYPE::PTR, res->GetVariableIdx());
    } else if (res->GetBaseType()->FloatType()) {
        mux = ctx.planner->NewVReg(VREG_TYPE::FLT);
        recv = ctx.planner->AllocVReg(VREG_TYPE::FLT, res->GetVariableIdx());
    } else {
        mux = ctx.planner->NewVReg(VREG_TYPE::INT);
        recv = ctx.planner->AllocVReg(VREG_TYPE::INT, res->GetVariableIdx());
    }

    ctx.phi_vec.push_back(std::make_pair(ll, mux->GetVRIdx()));

    auto uop = new UopMv;
    uop->SetSrc(mux);
    uop->SetDst(recv);

    ctx.cur_blk->PushUop(uop);
}

void InternalTranslation::dephi(InternalTranslationContext &ctx) {
    for (auto &&pr : ctx.phi_vec) {
        auto &&res = pr.second;
        auto vrres = ctx.planner->GetVReg(res);

        for (auto &&data : pr.first->GetDataList()) {
            auto lbidx = data.second->GetBlockIdx();
            auto rlbb = rlps_->FindBlkById(lbidx);

            if (data.first->IsConstant()) {
                auto cst = dynamic_cast<Constant *>(data.first.get());
                Assert(cst, "bad dynamic cast");

                auto &&cinfo = XConstValue(cst->GetValue());

                ctx.cur_blk = rlbb;
                rlbb->TempPop();

                if (data.first->GetBaseType()->FloatType()) {
                    lf(vrres, cinfo, ctx);
                } else {
                    li(vrres, cinfo, ctx);
                }

                ctx.cur_blk = nullptr;
            } else {
                auto var = dynamic_cast<Variable *>(data.first.get());
                Assert(var, "bitcast should op on var");
                auto rsvr = ctx.planner->GetVReg(var->GetVariableIdx());

                auto uop = new UopMv;
                uop->SetSrc(rsvr);
                uop->SetDst(vrres);

                rlbb->TempPop();
                rlbb->PushUop(uop);
            }

            rlbb->RecoverUops();
            ctx.lui_map.clear();
            ctx.fimm_map.clear();
        }
    }
}