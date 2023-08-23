#pragma once

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "3tle3wa/backend/Interface.hh"

class AsmLabelArray final : public Serializable {
   public:
    AsmLabelArray() = default;
};