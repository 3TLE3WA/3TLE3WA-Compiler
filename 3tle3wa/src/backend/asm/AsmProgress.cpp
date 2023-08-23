#include "3tle3wa/backend/asm/AsmProgress.hh"

#include <cinttypes>
#include <string>

#include "3tle3wa/backend/asm/AsmBasicBlock.hh"
#include "3tle3wa/backend/asm/AsmInstruction.hh"
#include "3tle3wa/utils/logs.hh"

AsmProgress::AsmProgress(const std::string &name) : ablks_(), ret_(nullptr), first_bidx_(0), align_(1) {
    FILE *fp = open_memstream(&label_, &label_len_);
    fprintf(fp, "%s", name.c_str());
    fflush(fp);
    fclose(fp);
}

AsmProgress::~AsmProgress() {
    if (label_ != nullptr) {
        free(label_);
        label_ = nullptr;
        label_len_ = 0;
    }
}

void AsmProgress::Push(std::unique_ptr<AsmBasicBlock> ablk, bool is_ret) {
    if (is_ret) {
        if (ret_ != nullptr) {
            panic("double ret block");
        }
        ret_ = ablk.get();
    }
    ablks_.push_back(std::move(ablk));
}

AsmBasicBlock *AsmProgress::CreateEntryBlock() {
    ablks_.push_front(std::make_unique<AsmBasicBlock>(0, this));
    return ablks_.front().get();
}

AsmBasicBlock *AsmProgress::ReturnBlock() { return ret_; }

const char *AsmProgress::Label() const { return label_; }

void AsmProgress::formatString(FILE *fp) {
    fprintf(fp,
            "\t.text\n"

            // since JH7110 just has a dtim for s7 monitor core
            // .itim will do nothing
            // "\t.section\t.itim,\"ax\",@progbits\n"

            "\t.align\t%" PRIu64
            "\n"
            "\t.global\t%s\n"
            "\t.type\t%s, @function\n"
            "%s:\n",
            align_, label_, label_, label_);
    for (auto &&blk : ablks_) {
        fprintf(fp, "%s", blk->CString());
    }
    fprintf(fp, "\t.size\t%s, .-%s\n", label_, label_);
}

size_t AsmProgress::FirstBlk() const { return first_bidx_; }

void AsmProgress::SetFirstBlk(size_t idx) { first_bidx_ = idx; }