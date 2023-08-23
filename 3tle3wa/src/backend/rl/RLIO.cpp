#include <cinttypes>

#include "3tle3wa/backend/asm/AsmAbi.hh"
#include "3tle3wa/backend/rl/InternalTranslation.hh"
#include "3tle3wa/backend/rl/RLBasicBlock.hh"
#include "3tle3wa/backend/rl/RLGen.hh"
#include "3tle3wa/backend/rl/RLPlanner.hh"
#include "3tle3wa/backend/rl/RLProgress.hh"
#include "3tle3wa/backend/rl/RLStackInfo.hh"
#include "3tle3wa/backend/rl/RLUop.hh"
#include "3tle3wa/backend/rl/RLVirtualRegister.hh"
#include "3tle3wa/backend/utils.hh"

void RLGen::formatString(FILE *fp) {
    for (auto &&task : translation_tasks_) {
        fprintf(fp, "%s", task->CString());
    }
}

void InternalTranslation::formatString(FILE *fp) {
    // avoid format
    fprintf(fp, "%s", rlps_->CString());
}

void RLProgress::formatString(FILE *fp) {
    fprintf(fp, "\t# ===== progrem begin =====\n");
    fprintf(fp, "%s:\n", label_);
    for (auto &&bb : rlbbs_) {
        fprintf(fp, "%s", bb->CString());
    }
    fprintf(fp, "\t# =====  stack infos  =====\n");
    fprintf(fp, "%s", planner_->CString());
    fprintf(fp, "\t# =====  progrem end  =====\n\n");
}

void RLBasicBlock::formatString(FILE *fp) {
    fprintf(fp, ".b%" PRIu64 ":\n", lbidx_);

    fprintf(fp, "\t# LDef:");
    for (auto &&def : live_def_) {
        fprintf(fp, " %" PRIu64, def);
    }
    fputc('\n', fp);

    fprintf(fp, "\t# LUse:");
    for (auto &&use : live_use_) {
        fprintf(fp, " %" PRIu64, use);
    }
    fputc('\n', fp);

    fprintf(fp, "\t# LOut:");
    for (auto &&out : live_out_) {
        fprintf(fp, " %" PRIu64, out);
    }
    fputc('\n', fp);

    fprintf(fp, "\t# LIn :");
    for (auto &&in : live_in_) {
        fprintf(fp, " %" PRIu64, in);
    }
    fputc('\n', fp);

    for (auto &&uop : uops_view_) {
        fprintf(fp, "%" PRIu64 "%s", uop->GetUopIdx(), uop->CString());
    }
}

void VirtualRegister::formatString(FILE *fp) {
    fprintf(fp, "%%r%" PRIu64, virt_reg_idx_);

    if (on_stk_ and stk_info_ != nullptr) {
        fprintf(fp, "<%" PRIu64 ":%" PRId64 ">", stk_info_->GetStackIdx(), stk_info_->GetStackLength());
    } else if (on_stk_) {
        panic("ill formed");
    } else if (reg_allocated_) {
        fprintf(fp, "<%s>", gpr[real_reg_idx_]);
    }
}

void RLPlanner::formatString(FILE *fp) {
    fprintf(fp, "stkidx : (    length,             offset) <- assume fp = .\n");
    uint64_t offs = 0;
    for (auto &&stk : stk_storage_) {
        offs -= stk->GetStackLength();
        fprintf(fp, "%06" PRIu64 " : (%010" PRIu64 ", 0x%016" PRIx64 ") <- %" PRId64 "(fp)\n", stk->GetStackIdx(),
                stk->GetStackLength(), stk->GetStackOff(), offs);
    }
    fputc('\n', fp);
}

// ----- uops -----

void UopRet::formatString(FILE *fp) {
    fprintf(fp, "\tret");

    if (retval_ != nullptr) {
        fprintf(fp, "\t%s\n", retval_->CString());
    }

    fputc('\n', fp);
}

void UopCall::formatString(FILE *fp) {
    fprintf(fp, "\t");
    if (retval_ != nullptr) {
        fprintf(fp, "%s = ", retval_->CString());
    }

    if (libcall_) {
        fprintf(fp, "call %s(", callee_.c_str());
    } else if (tailcall_) {
        fprintf(fp, "j %s(", callee_.c_str());
    } else {
        fprintf(fp, "jal %s(", callee_.c_str());
    }

    if (not params_.empty()) {
        fprintf(fp, "%s", params_.front()->CString());
        for (size_t i = 1; i < params_.size(); ++i) {
            fprintf(fp, ", %s", params_.at(i)->CString());
        }
    }

    fprintf(fp, ")\n");
}

void UopLui::formatString(FILE *fp) {
    // avoid format
    fprintf(fp, "\tlui\t%s, 0x%" PRIx32 "\n", dst_->CString(), imm_up20_);
}

void UopLi::formatString(FILE *fp) {
    if (dst_->UseFGPR()) {
        Assert(imm32_ == 0, "this should be 0");
        fprintf(fp, "\tfmv.w.x\t%s, zero\n", dst_->CString());
    } else {
        fprintf(fp, "\tli\t%s, 0x%" PRIx32 "\n", dst_->CString(), imm32_);
    }
}

void UopMv::formatString(FILE *fp) {
    // avoid format
    fprintf(fp, "\tmv\t%s, %s\n", dst_->CString(), src_->CString());
}

void UopCvtS2W::formatString(FILE *fp) {
    // avoid format
    fprintf(fp, "\tfcvt.w.s\t%s, %s\n", dst_->CString(), src_->CString());
}

void UopCvtW2S::formatString(FILE *fp) {
    // avoid format
    fprintf(fp, "\tfcvt.s.w\t%s, %s\n", dst_->CString(), src_->CString());
}

void UopBranch::formatString(FILE *fp) {
    if (on_true_) {
        fprintf(fp, "\tbnez\t%s, .b%" PRIu64 "\n", cond_->CString(), dst_idx_);
    } else {
        fprintf(fp, "\tbeqz\t%s, .b%" PRIu64 "\n", cond_->CString(), dst_idx_);
    }
}

void UopJump::formatString(FILE *fp) {
    // avoid format
    fprintf(fp, "\tj\t.b%" PRIu64 "\n", dst_idx_);
}

void UopLla::formatString(FILE *fp) {
    if (src_.empty()) {
        fprintf(fp, "\tadd\t%s, %s, %" PRIu64 "\n", dst_->CString(), "sp", dst_->GetAddressInfo()->GetStackOff());
        return;
    }
    // need more care
    fprintf(fp, "\tlla\t%s, %s\n", dst_->CString(), src_.c_str());
}

void UopLoad::formatString(FILE *fp) {
    if (base_ == nullptr) {
        fprintf(fp, "\tld\t%s, %" PRId32 "(%s)\n", dst_->CString(), off_lo12_, "sp");
        return;
    }
    fprintf(fp, "\tld\t%s, %" PRId32 "(%s)\n", dst_->CString(), off_lo12_, base_->CString());
}

void UopStore::formatString(FILE *fp) {
    if (base_ == nullptr) {
        if (src_ == nullptr) {
            fprintf(fp, "\tst\tzero, %" PRId32 "(%s)\n", off_lo12_, "sp");
            return;
        }
        fprintf(fp, "\tst\t%s, %" PRId32 "(%s)\n", src_->CString(), off_lo12_, "sp");
        return;
    }
    if (src_ == nullptr) {
        fprintf(fp, "\tst\tzero, %" PRId32 "(%s)\n", off_lo12_, base_->CString());
        return;
    }
    fprintf(fp, "\tst\t%s, %" PRId32 "(%s)\n", src_->CString(), off_lo12_, base_->CString());
}

void UopFLoad::formatString(FILE *fp) {
    if (base_ == nullptr) {
        fprintf(fp, "\tfld\t%s, %" PRId32 "(%s)\n", dst_->CString(), off_lo12_, "sp");
        return;
    }
    fprintf(fp, "\tfld\t%s, %" PRId32 "(%s)\n", dst_->CString(), off_lo12_, base_->CString());
}

void UopFStore::formatString(FILE *fp) {
    if (base_ == nullptr) {
        fprintf(fp, "\tfst\t%s, %" PRId32 "(%s)\n", src_->CString(), off_lo12_, "sp");
        return;
    }
    fprintf(fp, "\tfst\t%s, %" PRId32 "(%s)\n", src_->CString(), off_lo12_, base_->CString());
}

void UopFLoadLB::formatString(FILE *fp) {
    if (helper_ == nullptr) {
        fprintf(fp, "\tfld\t%s, %s\n", dst_->CString(), sym_.c_str());
        return;
    }
    fprintf(fp, "\tfld\t%s, %s, %s\n", dst_->CString(), sym_.c_str(), helper_->CString());
}

void UopFCmp::formatString(FILE *fp) {
    switch (kind_) {
        case COMP_KIND::EQU:
            fprintf(fp, "\tfeq\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case COMP_KIND::LTH:
            fprintf(fp, "\tflt\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case COMP_KIND::LEQ:
            fprintf(fp, "\tfle\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        default:
            panic("unexpected");
    }
}

void UopIBin::formatString(FILE *fp) {
    switch (kind_) {
        case IBIN_KIND::ADD:
            fprintf(fp, "\taddw\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::SUB:
            fprintf(fp, "\tsubw\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::MUL:
            fprintf(fp, "\tmulw\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::DIV:
            fprintf(fp, "\tdivw\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::REM:
            fprintf(fp, "\tremw\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::AND:
            fprintf(fp, "\tand\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::OR:
            fprintf(fp, "\tor\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::XOR:
            fprintf(fp, "\txor\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::SLL:
            fprintf(fp, "\tsllw\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::SRA:
            fprintf(fp, "\tsraw\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::SRL:
            fprintf(fp, "\tsrlw\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::LNOT:
            fprintf(fp, "\tseqz\t%s, %s\n", dst_->CString(), lhs_->CString());
            break;
        case IBIN_KIND::LBOOL:
            fprintf(fp, "\tsnez\t%s, %s\n", dst_->CString(), lhs_->CString());
            break;
    }
}

void UopIBin64::formatString(FILE *fp) {
    switch (kind_) {
        case IBIN_KIND::ADD:
            fprintf(fp, "\tadd\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::SUB:
            fprintf(fp, "\tsub\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::MUL:
            fprintf(fp, "\tmul\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::DIV:
            fprintf(fp, "\tdiv\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::REM:
            fprintf(fp, "\trem\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::AND:
            fprintf(fp, "\tand\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::OR:
            fprintf(fp, "\tor\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::XOR:
            fprintf(fp, "\txor\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::SLL:
            fprintf(fp, "\tsll\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::SRA:
            fprintf(fp, "\tsra\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::SRL:
            fprintf(fp, "\tsrl\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case IBIN_KIND::LNOT:
            fprintf(fp, "\tseqz\t%s, %s\n", dst_->CString(), lhs_->CString());
            break;
        case IBIN_KIND::LBOOL:
            fprintf(fp, "\tsnez\t%s, %s\n", dst_->CString(), lhs_->CString());
            break;
    }
}

void UopIBinImm::formatString(FILE *fp) {
    fprintf(fp, "\t");
    switch (kind_) {
        case IBIN_KIND::ADD:
            fprintf(fp, "add");
            break;
        case IBIN_KIND::SUB:
            fprintf(fp, "sub");
            break;
        case IBIN_KIND::AND:
            fprintf(fp, "and");
            break;
        case IBIN_KIND::OR:
            fprintf(fp, "or");
            break;
        case IBIN_KIND::XOR:
            fprintf(fp, "xor");
            break;
        case IBIN_KIND::SLL:
            fprintf(fp, "sll");
            break;
        case IBIN_KIND::SRA:
            fprintf(fp, "sra");
            break;
        case IBIN_KIND::SRL:
            fprintf(fp, "srl");
            break;
        default:
            panic("unexpected");
    }
    fprintf(fp, "w\t%s, %s, %" PRId32 "\n", dst_->CString(), lhs_->CString(), imm_lo12_);
}

void UopIBinImm64::formatString(FILE *fp) {
    fprintf(fp, "\t");
    switch (kind_) {
        case IBIN_KIND::ADD:
            fprintf(fp, "add");
            break;
        case IBIN_KIND::SUB:
            fprintf(fp, "sub");
            break;
        case IBIN_KIND::AND:
            fprintf(fp, "and");
            break;
        case IBIN_KIND::OR:
            fprintf(fp, "or");
            break;
        case IBIN_KIND::XOR:
            fprintf(fp, "xor");
            break;
        case IBIN_KIND::SLL:
            fprintf(fp, "sll");
            break;
        case IBIN_KIND::SRA:
            fprintf(fp, "sra");
            break;
        case IBIN_KIND::SRL:
            fprintf(fp, "srl");
            break;
        default:
            panic("unexpected");
    }
    fprintf(fp, "\t%s, %s, %" PRId32 "\n", dst_->CString(), lhs_->CString(), imm_lo12_);
}

void UopFBin::formatString(FILE *fp) {
    switch (kind_) {
        case FBIN_KIND::ADD:
            fprintf(fp, "\tfadd\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case FBIN_KIND::SUB:
            fprintf(fp, "\tfsub\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case FBIN_KIND::MUL:
            fprintf(fp, "\tfmul\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case FBIN_KIND::DIV:
            fprintf(fp, "\tfdiv\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case FBIN_KIND::MAX:
            fprintf(fp, "\tfmax\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case FBIN_KIND::MIN:
            fprintf(fp, "\tfmin\t%s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString());
            break;
        case FBIN_KIND::ABS:
            fprintf(fp, "\tfabs\t%s, %s\n", dst_->CString(), lhs_->CString());
            break;
        case FBIN_KIND::NEG:
            fprintf(fp, "\tfneg\t%s, %s\n", dst_->CString(), lhs_->CString());
            break;
    }
}

void UopFTri::formatString(FILE *fp) {
    switch (kind_) {
        case FTRI_KIND::MADD:
            fprintf(fp, "\tfmadd\t%s, %s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString(), ahs_->CString());
            break;
        case FTRI_KIND::MSUB:
            fprintf(fp, "\tfmsub\t%s, %s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString(), ahs_->CString());
            break;
        case FTRI_KIND::NMADD:
            fprintf(fp, "\tfnmadd\t%s, %s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString(), ahs_->CString());
            break;
        case FTRI_KIND::NMSUB:
            fprintf(fp, "\tfnmsub\t%s, %s, %s, %s\n", dst_->CString(), lhs_->CString(), rhs_->CString(), ahs_->CString());
            break;
    }
}

void UopICmpBranch::formatString(FILE *fp) {
    fprintf(fp, "\tb");
    switch (kind_) {
        case COMP_KIND::EQU:
            fprintf(fp, "eq");
            break;
        case COMP_KIND::NEQ:
            fprintf(fp, "ne");
            break;
        case COMP_KIND::LTH:
            fprintf(fp, "lt");
            break;
        case COMP_KIND::GTH:
            fprintf(fp, "gt");
            break;
        case COMP_KIND::LEQ:
            fprintf(fp, "le");
            break;
        case COMP_KIND::GEQ:
            fprintf(fp, "ge");
            break;
    }
    if (lhs_ == nullptr) {
        fprintf(fp, "\tzero, %s, .b%" PRIu64 "\n", rhs_->CString(), dst_idx_);
        return;
    } else if (rhs_ == nullptr) {
        fprintf(fp, "\t%s, zero, .b%" PRIu64 "\n", lhs_->CString(), dst_idx_);
        return;
    }
    fprintf(fp, "\t%s, %s, .b%" PRIu64 "\n", lhs_->CString(), rhs_->CString(), dst_idx_);
}

void UopNop::formatString(FILE *fp) {
    // avoid format
    fprintf(fp, "\tnop\n");
}