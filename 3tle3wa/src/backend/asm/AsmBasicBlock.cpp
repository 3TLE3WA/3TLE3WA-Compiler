#include "3tle3wa/backend/asm/AsmBasicBlock.hh"

#include <cinttypes>

#include "3tle3wa/backend/asm/AsmInstruction.hh"
#include "3tle3wa/backend/asm/AsmProgress.hh"

AsmBasicBlock::AsmBasicBlock(size_t lbidx, AsmProgress *father) : lbidx_(lbidx), father_(father), is_ret_(false) {}

void AsmBasicBlock::Push(AsmInstruction *inst) { insts_.push_back(std::unique_ptr<AsmInstruction>(inst)); }

void AsmBasicBlock::Pop() { insts_.pop_back(); }

void AsmBasicBlock::formatString(FILE *fp) {
    fprintf(fp, ".L.%s.b%" PRIu64 ":\n", father_->Label(), lbidx_);
    for (auto &&inst : insts_view_) {
        fprintf(fp, "%s", inst->CString());
    }
}

void AsmBasicBlock::SetIsRet(bool on) { is_ret_ = on; }

bool AsmBasicBlock::IsRet() { return is_ret_; }

size_t AsmBasicBlock::Lbidx() { return lbidx_; }

size_t AsmBasicBlock::GetBlockIdx() { return lbidx_; }

const char *AsmBasicBlock::FatherLabel() const { return father_->Label(); }

size_t AsmBasicBlock::SearchFirst() { return father_->FirstBlk(); }
