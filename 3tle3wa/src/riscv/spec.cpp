#include "3tle3wa/riscv/spec.hh"

const char *gpr[] = {
    // integer registers
    "zero",  // x0
    "ra",    // x1
    "sp",    // x2
    "gp",    // x3
    "tp",    // x4
    "t0",    // x5
    "t1",    // x6
    "t2",    // x7
    "s0",    // x8 fp
    "s1",    // x9
    "a0",    // x10
    "a1",    // x11
    "a2",    // x12
    "a3",    // x13
    "a4",    // x14
    "a5",    // x15
    "a6",    // x16
    "a7",    // x17
    "s2",    // x18
    "s3",    // x19
    "s4",    // x20
    "s5",    // x21
    "s6",    // x22
    "s7",    // x23
    "s8",    // x24
    "s9",    // x25
    "s10",   // x26
    "s11",   // x27
    "t3",    // x28
    "t4",    // x29
    "t5",    // x30
    "t6",    // x31

    // float registers freg n idx = 32 + n
    "ft0",   // f0
    "ft1",   // f1
    "ft2",   // f2
    "ft3",   // f3
    "ft4",   // f4
    "ft5",   // f5
    "ft6",   // f6
    "ft7",   // f7
    "fs0",   // f8
    "fs1",   // f9
    "fa0",   // f10
    "fa1",   // f11
    "fa2",   // f12
    "fa3",   // f13
    "fa4",   // f14
    "fa5",   // f15
    "fa6",   // f16
    "fa7",   // f17
    "fs2",   // f18
    "fs3",   // f19
    "fs4",   // f20
    "fs5",   // f21
    "fs6",   // f22
    "fs7",   // f23
    "fs8",   // f24
    "fs9",   // f25
    "fs10",  // f26
    "fs11",  // f27
    "ft8",   // f28
    "ft9",   // f29
    "ft10",  // f30
    "ft11",  // f31
};