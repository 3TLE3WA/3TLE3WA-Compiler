#include "3tle3wa/backend/asm/AsmLocalConstant.hh"

AsmLocalConstant::AsmLocalConstant(size_t idx, uint32_t values) : idx_(idx), values_(values), data_len_(4), align_(4) {}

void AsmLocalConstant::formatString(FILE *fp) {
    fprintf(fp,
            // alignment
            "\t.align\t%" PRIu64
            "\n"

            // type
            "\t.type\t.LC%" PRIu64
            ", @object\n"

            // data size
            "\t.size\t.LC%" PRIu64 ", %" PRIu64
            "\n"

            // label begin
            ".LC%" PRIu64
            ":\n"

            "\t.word\t%" PRIu32 "\n",
            align_, idx_, idx_, data_len_, idx_, values_);
}