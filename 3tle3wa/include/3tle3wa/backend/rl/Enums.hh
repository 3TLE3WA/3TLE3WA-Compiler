#pragma once

#include "3tle3wa/ir/instruction/opCode.hh"

enum class VREG_TYPE { FLT, PTR, INT };

enum class COMP_KIND {
    // 比较枚举对应 opCode

    LTH = OP_LTH,
    GTH = OP_GTH,
    LEQ = OP_LEQ,
    GEQ = OP_GEQ,
    EQU = OP_EQU,
    NEQ = OP_NEQ,
};

enum class IBIN_KIND {
    // 运算枚举对应 opCode

    ADD = OP_ADD,
    SUB = OP_SUB,
    MUL = OP_MUL,
    DIV = OP_DIV,
    REM = OP_REM,
    SLL = OP_LSHIFT,
    SRA = OP_RSHIFT,
    SRL,
    XOR,
    AND,
    OR,
    LNOT,
    LBOOL,
    // NOT,
    // NEG,
};

enum class FBIN_KIND {
    // 运算枚举对应 opCode

    ADD = OP_ADD,
    SUB = OP_SUB,
    MUL = OP_MUL,
    DIV = OP_DIV,
    NEG,
    MAX,
    MIN,
    ABS,
};

enum class FTRI_KIND {
    MADD,
    MSUB,
    NMADD,
    NMSUB,
};
