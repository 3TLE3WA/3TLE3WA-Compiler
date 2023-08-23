#pragma once

enum OpCode {
    None,
    Ret,
    Jump,
    Branch,
    Alloca,
    Store,
    Gep,
    Call,
    Phi,
    // Unary
    Load,
    BitCast,
    SiToFp,
    FpToSi,
    Zext,
    FNeg,
    FAbs,
    // only exist in front-end
    OP_NOT,    // unary not, replaced by OP_EQU in expr
    OP_MINUS,  // unary minus, replaced by OP_SUB in expr
    // Binary
    // IBinary
    // FBinary
    OP_ADD,     // binary add
    OP_SUB,     // binary sub
    OP_MUL,     // binary mul
    OP_DIV,     // binary div
    OP_REM,     // binary rem
    OP_LSHIFT,  // left shift
    OP_RSHIFT,  // right shift
    // ICmp
    // FCmp
    OP_LTH,  // less than
    OP_LEQ,  // less or equal
    OP_GTH,  // greater
    OP_GEQ,  // greater or equal
    OP_EQU,  // equal
    OP_NEQ,  // not equal
};

bool IsCommutative(OpCode);
