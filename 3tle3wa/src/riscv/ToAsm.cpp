#include <unordered_map>
#include <unordered_set>

#include "3tle3wa/backend/asm/AsmAbi.hh"
#include "3tle3wa/backend/asm/AsmBasicBlock.hh"
#include "3tle3wa/backend/asm/AsmGlobalValue.hh"
#include "3tle3wa/backend/asm/AsmInstruction.hh"
#include "3tle3wa/backend/asm/AsmLocalConstant.hh"
#include "3tle3wa/backend/asm/AsmProgress.hh"
#include "3tle3wa/backend/rl/Enums.hh"
#include "3tle3wa/backend/rl/InternalTranslation.hh"
#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLPlanner.hh"
#include "3tle3wa/backend/rl/RLProgress.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/ir/IR.hh"
#include "3tle3wa/riscv/asm.hh"
#include "3tle3wa/riscv/spec.hh"
#include "3tle3wa/utils/logs.hh"

static inline void UseVirtReg(AsmBasicBlock *abb, RLPlanner *plan, VirtualRegister *vr) {
    if (vr == nullptr) {
        return;
    }

    if (vr->IsSaving()) {
        auto off = vr->GetSaveInfo();
        plan->Recover(vr->GetRealRegIdx());

        int64_t real_off = -(int64_t)off;
        if (real_off < 0 and ImmWithin(12, real_off)) {
            if (vr->GetVRType() == VREG_TYPE::INT) {
                abb->Push(new riscv::LW(vr->GetRealRegIdx(), riscv::sp, real_off));
            } else if (vr->GetVRType() == VREG_TYPE::FLT) {
                abb->Push(new riscv::FLW(vr->GetRealRegIdx(), riscv::sp, real_off));
            } else if (vr->GetVRType() == VREG_TYPE::PTR) {
                abb->Push(new riscv::LD(vr->GetRealRegIdx(), riscv::sp, real_off));
            } else {
                panic("unexpected");
            }
        } else {
            panic("plz avoid this");
        }

        vr->SetSavingOff(0);
    }
}

static inline void SaveVirtReg(AsmBasicBlock *abb, VirtualRegister *vr) {
    auto off = vr->GetSaveInfo();
    int64_t real_off = -(int64_t)off;

    if (real_off < 0 and ImmWithin(12, real_off)) {
        if (vr->GetVRType() == VREG_TYPE::INT) {
            abb->Push(new riscv::SW(vr->GetRealRegIdx(), riscv::sp, real_off));
        } else if (vr->GetVRType() == VREG_TYPE::FLT) {
            abb->Push(new riscv::FSW(vr->GetRealRegIdx(), riscv::sp, real_off));
        } else if (vr->GetVRType() == VREG_TYPE::PTR) {
            abb->Push(new riscv::SD(vr->GetRealRegIdx(), riscv::sp, real_off));
        } else {
            panic("unexpected");
        }
    } else {
        panic("plz avoid this");
    }
}

void RLPlanner::Init(CRVC_UNUSE AsmBasicBlock *abb) {
    int callee_save_off = 0;

    callee_save_off -= 8;
    abb->Push(new riscv::SD(riscv::ra, riscv::sp, callee_save_off));

    for (auto &&v : used_real_regs_) {
        if (abi_reg_info.i.callee_save.count(v)) {
            callee_save_off -= 8;
            abb->Push(new riscv::SD(v, riscv::sp, callee_save_off));
        } else if (abi_reg_info.f.callee_save.count(v)) {
            callee_save_off -= 8;
            abb->Push(new riscv::FSW(v, riscv::sp, callee_save_off));
        }
    }

    auto sub = -(int64_t)total_stack_size_;

    if (ImmWithin(12, sub)) {
        abb->Push(new riscv::ADDI(riscv::sp, riscv::sp, sub));
    } else {
        abb->Push(new riscv::LI(riscv::ra, total_stack_size_));
        abb->Push(new riscv::SUB(riscv::sp, riscv::sp, riscv::ra));
    }
}

void RLPlanner::RecoverBeforeRet(CRVC_UNUSE AsmBasicBlock *abb) {
    if (ImmWithin(12, total_stack_size_)) {
        abb->Push(new riscv::ADDI(riscv::sp, riscv::sp, total_stack_size_));
    } else {
        abb->Push(new riscv::LI(riscv::ra, total_stack_size_));
        abb->Push(new riscv::ADD(riscv::sp, riscv::sp, riscv::ra));
    }

    int callee_save_off = 0;

    callee_save_off -= 8;
    abb->Push(new riscv::LD(riscv::ra, riscv::sp, callee_save_off));

    for (auto &&v : used_real_regs_) {
        if (abi_reg_info.i.callee_save.count(v)) {
            callee_save_off -= 8;
            abb->Push(new riscv::LD(v, riscv::sp, callee_save_off));
        } else if (abi_reg_info.f.callee_save.count(v)) {
            callee_save_off -= 8;
            abb->Push(new riscv::FLW(v, riscv::sp, callee_save_off));
        }
    }
}

void RLPlanner::Recover(size_t idx) {
    auto &&tpl = save_stack_.at(idx);
    std::get<1>(tpl) = false;
    std::get<2>(tpl) = nullptr;
}

size_t RLPlanner::BeforeCall(AsmBasicBlock *abb, const std::unordered_set<VirtualRegister *> &livings) {
    for (auto &&live : livings) {
        if (not live->IsSaving()) {
            auto rridx = live->GetRealRegIdx();

            if (auto fnd = save_stack_.find(rridx); fnd != save_stack_.end()) {
                auto &&tpl = fnd->second;
                Assert(std::get<1>(tpl) == false and std::get<2>(tpl) == nullptr, "should free before saving again");
                live->SetSavingOff(std::get<0>(tpl));
                std::get<1>(tpl) = true;
                std::get<2>(tpl) = live;
            } else {
                auto siz = save_stack_.size();
                auto tpl = std::make_tuple((siz + 1) * 8, true, live);
                live->SetSavingOff(std::get<0>(tpl));
                save_stack_.emplace(rridx, std::move(tpl));
            }

            SaveVirtReg(abb, live);
        }
    }

    return save_stack_.size() * 8;
}

void RLPlanner::RecoverCall(AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    for (auto &&[rridx, elem] : save_stack_) {
        if (std::get<1>(elem)) {
            UseVirtReg(abb, this, std::get<2>(elem));
        }
    }
    save_stack_.clear();
}

void UopRet::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    abb->SetIsRet(true);

    if (retval_ != nullptr) {
        UseVirtReg(abb, plan, retval_);
        auto rridx = retval_->GetRealRegIdx();

        if (retval_->UseFGPR()) {
            if (riscv::fa0 != rridx) {
                abb->Push(new riscv::FMV_S(riscv::fa0, rridx));
            }
        } else if (retval_->UseIGPR()) {
            if (riscv::a0 != rridx) {
                abb->Push(new riscv::MV(riscv::a0, rridx));
            }
        } else {
            panic("unexpected");
        }
    }

    plan->RecoverBeforeRet(abb);

    abb->Push(new riscv::RET());
}

void UopCall::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    std::unordered_set<VirtualRegister *> arg_param;
    for (auto &&p : params_) {
        if (p->IsOnStk()) {
            continue;
        }
        if ((abi_reg_info.i.caller_save.count(p->GetRealRegIdx()) or abi_reg_info.f.caller_save.count(p->GetRealRegIdx())) and
            not living_regs_.count(p)) {
            arg_param.insert(p);
        }
    }

    auto pstk_siz = plan->BeforeCall(abb, living_regs_);
    pstk_siz = plan->BeforeCall(abb, arg_param);

    std::unordered_map<size_t, std::pair<size_t, VirtualRegister *>> used_args;

    struct pinfo {
        bool onstack{false};
        enum class P_INFO { F32, I32, I64 } info{P_INFO::I32};
        int64_t imm{0};
    };

    std::vector<pinfo> pinfos(params_.size());

    size_t ips = 0;
    size_t fps = 0;
    size_t sps = 0;

    size_t len = params_.size();

    // scan for args
    for (size_t i = 0; i < len; ++i) {
        auto &&p = params_[i];
        auto &&pi = pinfos[i];

        if ((p->GetRealRegIdx() >= riscv::a0 and p->GetRealRegIdx() <= riscv::a7) or
            (p->GetRealRegIdx() >= riscv::fa0 and p->GetRealRegIdx() <= riscv::fa7)) {
            used_args.emplace(p->GetRealRegIdx(), std::make_pair(i, p));
        }

        if (p->UseFGPR()) {
            if (fps < abi_info.f.nm_arg) {
                pi.imm = fps + abi_info.f.arg_bgn;
                fps += 1;

                pi.info = pinfo::P_INFO::F32;
            } else {
                pi.imm = sps;
                pi.onstack = true;
                pi.info = pinfo::P_INFO::F32;
                sps += 1;
            }
        } else if (p->UseIGPR()) {
            if (ips < abi_info.i.nm_arg) {
                pi.imm = ips + abi_info.i.arg_bgn;
                ips += 1;

                if (p->GetVRType() == VREG_TYPE::INT) {
                    pi.info = pinfo::P_INFO::I32;
                } else if (p->GetVRType() == VREG_TYPE::PTR) {
                    pi.info = pinfo::P_INFO::I64;
                } else {
                    panic("unexpected");
                }
            } else {
                pi.imm = sps;

                if (p->GetVRType() == VREG_TYPE::INT) {
                    pi.info = pinfo::P_INFO::I32;
                } else if (p->GetVRType() == VREG_TYPE::PTR) {
                    pi.info = pinfo::P_INFO::I64;
                } else {
                    panic("unexpected");
                }
                pi.onstack = true;
                sps += 1;
            }
        } else {
            panic("unexpected");
        }
    }

    int64_t stk_top = RoundUp(16, pstk_siz + 8 * sps);
    for (auto &&info : pinfos) {
        if (info.onstack) {
            auto real_off = stk_top - info.imm * 8;
            info.imm = -real_off;
        }
    }

    // std::unordered_map<size_t, size_t> visit;

    // constexpr size_t UNVISITED = 0;
    // constexpr size_t VISITING = 1;
    // constexpr size_t VISITED = 2;

    std::vector<size_t> fsavers(abi_reg_info.f.tmp.begin(), abi_reg_info.f.tmp.end());
    std::vector<size_t> isavers(abi_reg_info.i.tmp.begin(), abi_reg_info.i.tmp.end());

    size_t it = 0;

    for (size_t i = 0; i < len; ++i) {
        auto &&p = params_[i];
        auto &&pi = pinfos[i];

        if (p->IsOnStk()) {
            auto off = p->GetStackInfo()->GetStackOff();
            if (pi.onstack) {
                if (pi.info == pinfo::P_INFO::F32) {
                    auto tmp = fsavers.at(it % fsavers.size());
                    it += 1;
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::FLW(tmp, riscv::sp, off));
                        abb->Push(new riscv::FSW(tmp, riscv::sp, pi.imm));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::FLW(tmp, off_tmp, 0));
                        abb->Push(new riscv::FSW(tmp, riscv::sp, pi.imm));
                    }
                } else if (pi.info == pinfo::P_INFO::I32) {
                    auto tmp = isavers.at(it % isavers.size());
                    it += 1;
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::LW(tmp, riscv::sp, off));
                        abb->Push(new riscv::SW(tmp, riscv::sp, pi.imm));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::LW(tmp, off_tmp, 0));
                        abb->Push(new riscv::SW(tmp, riscv::sp, pi.imm));
                    }
                } else if (pi.info == pinfo::P_INFO::I64) {
                    auto tmp = isavers.at(it % isavers.size());
                    it += 1;
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::LD(tmp, riscv::sp, off));
                        abb->Push(new riscv::SD(tmp, riscv::sp, pi.imm));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::LD(tmp, off_tmp, 0));
                        abb->Push(new riscv::SD(tmp, riscv::sp, pi.imm));
                    }
                }
            } else {
                if (pi.info == pinfo::P_INFO::F32) {
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::FLW(pi.imm, riscv::sp, off));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::FLW(pi.imm, off_tmp, 0));
                    }
                } else if (pi.info == pinfo::P_INFO::I32) {
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::LW(pi.imm, riscv::sp, off));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::LW(pi.imm, off_tmp, 0));
                    }
                } else if (pi.info == pinfo::P_INFO::I64) {
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::LD(pi.imm, riscv::sp, off));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::LD(pi.imm, off_tmp, 0));
                    }
                }
            }
        } else if (p->IsSaving()) {
            int64_t off = -(int64_t)p->GetSaveInfo();
            if (pi.onstack) {
                if (pi.info == pinfo::P_INFO::F32) {
                    auto tmp = fsavers.at(it % fsavers.size());
                    it += 1;
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::FLW(tmp, riscv::sp, off));
                        abb->Push(new riscv::FSW(tmp, riscv::sp, pi.imm));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::FLW(tmp, off_tmp, 0));
                        abb->Push(new riscv::FSW(tmp, riscv::sp, pi.imm));
                    }
                } else if (pi.info == pinfo::P_INFO::I32) {
                    auto tmp = isavers.at(it % isavers.size());
                    it += 1;
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::LW(tmp, riscv::sp, off));
                        abb->Push(new riscv::SW(tmp, riscv::sp, pi.imm));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::LW(tmp, off_tmp, 0));
                        abb->Push(new riscv::SW(tmp, riscv::sp, pi.imm));
                    }
                } else if (pi.info == pinfo::P_INFO::I64) {
                    auto tmp = isavers.at(it % isavers.size());
                    it += 1;
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::LD(tmp, riscv::sp, off));
                        abb->Push(new riscv::SD(tmp, riscv::sp, pi.imm));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::LD(tmp, off_tmp, 0));
                        abb->Push(new riscv::SD(tmp, riscv::sp, pi.imm));
                    }
                }
            } else {
                if (pi.info == pinfo::P_INFO::F32) {
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::FLW(pi.imm, riscv::sp, off));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::FLW(pi.imm, off_tmp, 0));
                    }
                } else if (pi.info == pinfo::P_INFO::I32) {
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::LW(pi.imm, riscv::sp, off));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::LW(pi.imm, off_tmp, 0));
                    }
                } else if (pi.info == pinfo::P_INFO::I64) {
                    if (ImmWithin(12, off)) {
                        abb->Push(new riscv::LD(pi.imm, riscv::sp, off));
                    } else {
                        auto off_tmp = isavers.at(it % isavers.size());
                        it += 1;
                        abb->Push(new riscv::LI(off_tmp, off));
                        abb->Push(new riscv::ADD(off_tmp, off_tmp, riscv::sp));
                        abb->Push(new riscv::LD(pi.imm, off_tmp, 0));
                    }
                }
            }
        } else {
            auto rridx = p->GetRealRegIdx();
            if (pi.onstack) {
                if (pi.info == pinfo::P_INFO::F32) {
                    abb->Push(new riscv::FSW(rridx, riscv::sp, pi.imm));
                } else if (pi.info == pinfo::P_INFO::I32) {
                    abb->Push(new riscv::SW(rridx, riscv::sp, pi.imm));
                } else if (pi.info == pinfo::P_INFO::I64) {
                    abb->Push(new riscv::SD(rridx, riscv::sp, pi.imm));
                }
            } else {
                if (pi.info == pinfo::P_INFO::F32) {
                    abb->Push(new riscv::FMV_S(pi.imm, rridx));
                } else if (pi.info == pinfo::P_INFO::I32) {
                    abb->Push(new riscv::MV(pi.imm, rridx));
                } else if (pi.info == pinfo::P_INFO::I64) {
                    abb->Push(new riscv::MV(pi.imm, rridx));
                }
            }
        }
    }

    for (auto &&p : arg_param) {
        plan->Recover(p->GetRealRegIdx());
        p->SetSavingOff(0);
    }

    if (libcall_) {
        if (stk_top) abb->Push(new riscv::ADDI(riscv::sp, riscv::sp, -stk_top));
        abb->Push(new riscv::CALL(callee_.c_str()));
        if (stk_top) abb->Push(new riscv::ADDI(riscv::sp, riscv::sp, stk_top));
    } else if (callself_ and tailcall_ and sps == 0) {
        auto lbidx = abb->SearchFirst();
        auto label = ".L." + std::string(abb->FatherLabel()) + ".b" + std::to_string(lbidx);
        auto callfunc = new riscv::J(label.c_str());
        abb->Push(callfunc);
        return;
    } else {
        if (stk_top) abb->Push(new riscv::ADDI(riscv::sp, riscv::sp, -stk_top));
        abb->Push(new riscv::JAL(callee_.c_str()));
        if (stk_top) abb->Push(new riscv::ADDI(riscv::sp, riscv::sp, stk_top));
    }

    if (retval_ != nullptr) {
        if (retval_->GetVRType() == VREG_TYPE::FLT) {
            if (retval_->GetRealRegIdx() != riscv::fa0) {
                abb->Push(new riscv::FMV_S(retval_->GetRealRegIdx(), riscv::fa0));
            }
        } else {
            if (retval_->GetRealRegIdx() != riscv::a0) {
                abb->Push(new riscv::MV(retval_->GetRealRegIdx(), riscv::a0));
            }
        }
    }
}

void UopLui::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    abb->Push(new riscv::LUI(dst_->GetRealRegIdx(), imm_up20_));
}

void UopLi::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    if (dst_->UseFGPR()) {
        Assert(imm32_ == 0, "this should be 0");
        abb->Push(new riscv::FMV_W_X(dst_->GetRealRegIdx(), riscv::zero));
        return;
    }
    abb->Push(new riscv::LI(dst_->GetRealRegIdx(), imm32_));
}

void UopMv::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    if (dst_->GetRealRegIdx() == src_->GetRealRegIdx()) {
        return;
    }

    UseVirtReg(abb, plan, src_);

    if (dst_->UseFGPR()) {
        abb->Push(new riscv::FMV_S(dst_->GetRealRegIdx(), src_->GetRealRegIdx()));
    } else if (dst_->UseIGPR()) {
        abb->Push(new riscv::MV(dst_->GetRealRegIdx(), src_->GetRealRegIdx()));
    } else {
        panic("unexpected");
    }
}

void UopCvtS2W::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, src_);

    abb->Push(new riscv::FCVT_W_S(dst_->GetRealRegIdx(), src_->GetRealRegIdx()));
}

void UopCvtW2S::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, src_);

    abb->Push(new riscv::FCVT_S_W(dst_->GetRealRegIdx(), src_->GetRealRegIdx()));
}

void UopBranch::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, cond_);
    auto label = ".L." + std::string(abb->FatherLabel()) + ".b" + std::to_string(dst_idx_);

    if (on_true_) {
        abb->Push(new riscv::BNEZ(cond_->GetRealRegIdx(), label.c_str()));
    } else {
        abb->Push(new riscv::BEQZ(cond_->GetRealRegIdx(), label.c_str()));
    }
}

void UopJump::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    auto label = ".L." + std::string(abb->FatherLabel()) + ".b" + std::to_string(dst_idx_);
    abb->Push(new riscv::J(label.c_str()));
}

void UopLla::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    if (src_.empty()) {
        auto addr = dst_->GetAddressInfo();

        auto off = addr->GetStackOff();

        if (ImmWithin(12, off)) {
            abb->Push(new riscv::ADDI(dst_->GetRealRegIdx(), riscv::sp, off));
        } else {
            abb->Push(new riscv::LI(dst_->GetRealRegIdx(), off));
            abb->Push(new riscv::ADD(dst_->GetRealRegIdx(), dst_->GetRealRegIdx(), riscv::sp));
        }

        return;
    }

    auto src = src_;

    if (off_ != 0) {
        src += '+' + std::to_string(off_);
    }

    abb->Push(new riscv::LLA_LB(dst_->GetRealRegIdx(), src.c_str()));
}

void UopLoad::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, base_);

    size_t base = riscv::sp;
    bool w64 = false;

    if (base_ != nullptr) {
        base = base_->GetRealRegIdx();
    }

    w64 = dst_->GetVRType() == VREG_TYPE::PTR;

    if (w64) {
        abb->Push(new riscv::LD(dst_->GetRealRegIdx(), base, off_lo12_));
    } else {
        abb->Push(new riscv::LW(dst_->GetRealRegIdx(), base, off_lo12_));
    }
}

void UopStore::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, base_);
    UseVirtReg(abb, plan, src_);

    size_t base = riscv::sp, src = riscv::zero;
    bool w64 = false;

    if (base_ != nullptr) {
        base = base_->GetRealRegIdx();
    }

    if (src_ != nullptr) {
        src = src_->GetRealRegIdx();
        w64 = src_->GetVRType() == VREG_TYPE::PTR;
    }

    if (w64) {
        abb->Push(new riscv::SD(src, base, off_lo12_));
    } else {
        abb->Push(new riscv::SW(src, base, off_lo12_));
    }
}

void UopFLoad::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, base_);

    size_t base = riscv::sp;

    if (base_ != nullptr) {
        base = base_->GetRealRegIdx();
    }

    abb->Push(new riscv::FLW(dst_->GetRealRegIdx(), base, off_lo12_));
}

void UopFStore::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, base_);
    UseVirtReg(abb, plan, src_);

    size_t base = riscv::sp;

    if (base_ != nullptr) {
        base = base_->GetRealRegIdx();
    }

    abb->Push(new riscv::FSW(src_->GetRealRegIdx(), base, off_lo12_));
}

void UopFLoadLB::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, helper_);

    auto helper = riscv::ra;

    if (helper_ != nullptr) {
        helper = helper_->GetRealRegIdx();
    }

    abb->Push(new riscv::FLW_LB(dst_->GetRealRegIdx(), sym_.c_str(), helper));
}

void UopFCmp::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, lhs_);
    UseVirtReg(abb, plan, rhs_);

    switch (kind_) {
        case COMP_KIND::EQU:
            abb->Push(new riscv::FEQ(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case COMP_KIND::LTH:
            abb->Push(new riscv::FLT(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case COMP_KIND::LEQ:
            abb->Push(new riscv::FLE(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        default:
            panic("unexpected");
    }
}

void UopIBin::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, lhs_);
    UseVirtReg(abb, plan, rhs_);

    switch (kind_) {
        case IBIN_KIND::ADD:
            abb->Push(new riscv::ADDW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::SUB:
            abb->Push(new riscv::SUBW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::MUL:
            abb->Push(new riscv::MULW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::DIV:
            abb->Push(new riscv::DIVW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::REM:
            abb->Push(new riscv::REMW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::AND:
            abb->Push(new riscv::AND(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::OR:
            abb->Push(new riscv::OR(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::XOR:
            abb->Push(new riscv::XOR(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::SLL:
            abb->Push(new riscv::SLLW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::SRA:
            abb->Push(new riscv::SRAW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::SRL:
            abb->Push(new riscv::SRLW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::LNOT:
            abb->Push(new riscv::SEQZ(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::LBOOL:
            abb->Push(new riscv::SNEZ(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx()));
            break;
    }
}

void UopIBin64::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, lhs_);
    UseVirtReg(abb, plan, rhs_);

    switch (kind_) {
        case IBIN_KIND::ADD:
            abb->Push(new riscv::ADD(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::SUB:
            abb->Push(new riscv::SUB(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::MUL:
            abb->Push(new riscv::MUL(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::DIV:
            abb->Push(new riscv::DIV(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::REM:
            abb->Push(new riscv::REM(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::AND:
            abb->Push(new riscv::AND(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::OR:
            abb->Push(new riscv::OR(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::XOR:
            abb->Push(new riscv::XOR(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::SLL:
            abb->Push(new riscv::SLL(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::SRA:
            abb->Push(new riscv::SRA(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::SRL:
            abb->Push(new riscv::SRL(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::LNOT:
            abb->Push(new riscv::SEQZ(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx()));
            break;
        case IBIN_KIND::LBOOL:
            abb->Push(new riscv::SNEZ(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx()));
            break;
    }
}

void UopIBinImm::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, lhs_);

    switch (kind_) {
        case IBIN_KIND::ADD:
            abb->Push(new riscv::ADDIW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::AND:
            abb->Push(new riscv::ANDI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::OR:
            abb->Push(new riscv::ORI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::XOR:
            abb->Push(new riscv::XORI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::SLL:
            abb->Push(new riscv::SLLIW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::SRA:
            abb->Push(new riscv::SRAIW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::SRL:
            abb->Push(new riscv::SRLIW(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        default:
            panic("unexpected");
    }
}

void UopIBinImm64::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, lhs_);

    switch (kind_) {
        case IBIN_KIND::ADD:
            abb->Push(new riscv::ADDI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::AND:
            abb->Push(new riscv::ANDI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::OR:
            abb->Push(new riscv::ORI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::XOR:
            abb->Push(new riscv::XORI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::SLL:
            abb->Push(new riscv::SLLI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::SRA:
            abb->Push(new riscv::SRAI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        case IBIN_KIND::SRL:
            abb->Push(new riscv::SRLI(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), imm_lo12_));
            break;
        default:
            panic("unexpected");
    }
}

void UopFBin::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, lhs_);
    UseVirtReg(abb, plan, rhs_);

    switch (kind_) {
        case FBIN_KIND::ADD:
            abb->Push(new riscv::FADD_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case FBIN_KIND::SUB:
            abb->Push(new riscv::FSUB_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case FBIN_KIND::MUL:
            abb->Push(new riscv::FMUL_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case FBIN_KIND::DIV:
            abb->Push(new riscv::FDIV_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case FBIN_KIND::MAX:
            abb->Push(new riscv::FMAX_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case FBIN_KIND::MIN:
            abb->Push(new riscv::FMIN_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx()));
            break;
        case FBIN_KIND::ABS:
            abb->Push(new riscv::FABS_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx()));
            break;
        case FBIN_KIND::NEG:
            abb->Push(new riscv::FNEG_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx()));
            break;
    }
}

void UopFTri::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, lhs_);
    UseVirtReg(abb, plan, rhs_);
    UseVirtReg(abb, plan, ahs_);

    switch (kind_) {
        case FTRI_KIND::MADD:
            abb->Push(new riscv::FMADD_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx(),
                                         ahs_->GetRealRegIdx()));
            break;
        case FTRI_KIND::MSUB:
            abb->Push(new riscv::FMSUB_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx(),
                                         ahs_->GetRealRegIdx()));
            break;
        case FTRI_KIND::NMADD:
            abb->Push(new riscv::FNMADD_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx(),
                                          ahs_->GetRealRegIdx()));
            break;
        case FTRI_KIND::NMSUB:
            abb->Push(new riscv::FNMSUB_S(dst_->GetRealRegIdx(), lhs_->GetRealRegIdx(), rhs_->GetRealRegIdx(),
                                          ahs_->GetRealRegIdx()));
            break;
    }
}

void UopICmpBranch::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) {
    UseVirtReg(abb, plan, lhs_);
    UseVirtReg(abb, plan, rhs_);

    auto label = ".L." + std::string(abb->FatherLabel()) + ".b" + std::to_string(dst_idx_);
    size_t lhs{riscv::zero}, rhs{riscv::zero};

    if (lhs_ != nullptr) {
        lhs = lhs_->GetRealRegIdx();
    }
    if (rhs_ != nullptr) {
        rhs = rhs_->GetRealRegIdx();
    }

    switch (kind_) {
        case COMP_KIND::EQU:
            abb->Push(new riscv::BEQ(lhs, rhs, label.c_str()));
            break;
        case COMP_KIND::NEQ:
            abb->Push(new riscv::BNE(lhs, rhs, label.c_str()));
            break;
        case COMP_KIND::LTH:
            abb->Push(new riscv::BLT(lhs, rhs, label.c_str()));
            break;
        case COMP_KIND::GTH:
            abb->Push(new riscv::BGT(lhs, rhs, label.c_str()));
            break;
        case COMP_KIND::LEQ:
            abb->Push(new riscv::BLE(lhs, rhs, label.c_str()));
            break;
        case COMP_KIND::GEQ:
            abb->Push(new riscv::BGE(lhs, rhs, label.c_str()));
            break;
    }
}

void UopNop::ToAsm(CRVC_UNUSE AsmBasicBlock *abb, CRVC_UNUSE RLPlanner *plan) { return; }
