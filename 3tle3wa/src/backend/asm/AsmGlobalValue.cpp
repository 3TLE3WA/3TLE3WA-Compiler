#include "3tle3wa/backend/asm/AsmGlobalValue.hh"

AsmGlobalValue::AsmGlobalValue(const std::string &name, size_t len, bool uninit, size_t reserve)
    : label_(nullptr), label_len_(0), values_(), on_heap_(false), data_len_(len), uninit_(uninit), align_(4) {
    values_.reserve(reserve);

    FILE *fp = open_memstream(&label_, &label_len_);
    fprintf(fp, "%s", name.c_str());
    fflush(fp);
    fclose(fp);
}

AsmGlobalValue::~AsmGlobalValue() {
    if (label_ == nullptr) {
        free(label_);
        label_ = nullptr;
        label_len_ = 0;
    }
}

void AsmGlobalValue::Push(uint32_t value) { values_.push_back(value); }

bool AsmGlobalValue::OnHeap() const { return on_heap_; }

const char *AsmGlobalValue::Label() const { return label_; }

void AsmGlobalValue::formatString(FILE *fp) {
    if (uninit_) {
        if (data_len_ <= 8) {
            fprintf(fp, "\t.section\t.sbss, \"aw\"\n");
        } else {
            fprintf(fp, "\t.bss\n");
        }
    } else {
        if (data_len_ <= 8) {
            fprintf(fp, "\t.section\t.sdata, \"aw\"\n");
        } else {
            fprintf(fp, "\t.data\n");
        }
    }

    fprintf(fp,
            // declare global
            "\t.global\t%s\n"

            // alignment
            "\t.align\t%" PRIu64
            "\n"

            // type
            "\t.type\t%s, @object\n"

            // data size
            "\t.size\t%s, %" PRIu64
            "\n"

            // label begin
            "%s:\n",
            label_, align_, label_, label_, data_len_, label_);

    if (uninit_) {
        fprintf(fp, "\t.zero\t%" PRIu64 "\n", data_len_);
        return;
    }
    uint64_t cntz = 0;
    for (auto &&v : values_) {
        if (v == 0) {
            cntz += 1;
            continue;
        }

        if (cntz) {
            fprintf(fp, "\t.zero\t%" PRIu64 "\n", cntz * 4);
            cntz = 0;
        }
        fprintf(fp, "\t.word\t%" PRIu32 "\n", v);
    }
    if (cntz) {
        fprintf(fp, "\t.zero\t%" PRIu64 "\n", cntz * 4);
    }
}
