#pragma once

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "3tle3wa/backend/Interface.hh"

class AsmLocalConstant final : public Serializable {
    size_t idx_;

    uint32_t values_;

    size_t data_len_;

    size_t align_;

    void formatString(FILE *fp) final;

   public:
    AsmLocalConstant(size_t idx, uint32_t values);
};