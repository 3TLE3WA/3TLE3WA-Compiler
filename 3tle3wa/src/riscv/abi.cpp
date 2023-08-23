#include "3tle3wa/backend/asm/AsmAbi.hh"
#include "3tle3wa/riscv/spec.hh"

AsmAbiInfo abi_info = {
    .i =
        {
            .nm_arg = 8,
            .nm_save = 12,
            .nm_tmp = 7,

            .arg_bgn = riscv::a0,
            .sp = riscv::sp,
        },

    .f =
        {
            .nm_arg = 8,
            .nm_save = 12,
            .nm_tmp = 12,

            .arg_bgn = riscv::fa0,
        },
};

AsmRegInfo abi_reg_info = {
    .i =
        {
            .arg =
                {
                    riscv::a0,
                    riscv::a1,
                    riscv::a2,
                    riscv::a3,
                    riscv::a4,
                    riscv::a5,
                    riscv::a6,
                    riscv::a7,
                },
            .save =
                {
                    riscv::s0,
                    riscv::s1,
                    riscv::s2,
                    riscv::s3,
                    riscv::s4,
                    riscv::s5,
                    riscv::s6,
                    riscv::s7,
                    riscv::s8,
                    riscv::s9,
                    riscv::s10,
                    riscv::s11,
                },
            .tmp =
                {
                    riscv::t0,
                    riscv::t1,
                    riscv::t2,
                    riscv::t3,
                    riscv::t4,
                    riscv::t5,
                    riscv::t6,
                },

            .caller_save{
                riscv::a0,
                riscv::a1,
                riscv::a2,
                riscv::a3,
                riscv::a4,
                riscv::a5,
                riscv::a6,
                riscv::a7,
                riscv::t0,
                riscv::t1,
                riscv::t2,
                riscv::t3,
                riscv::t4,
                riscv::t5,
                riscv::t6,
            },

            .callee_save{
                riscv::s0,
                riscv::s1,
                riscv::s2,
                riscv::s3,
                riscv::s4,
                riscv::s5,
                riscv::s6,
                riscv::s7,
                riscv::s8,
                riscv::s9,
                riscv::s10,
                riscv::s11,
            },
        },

    .f =
        {
            .arg =
                {
                    riscv::fa0,
                    riscv::fa1,
                    riscv::fa2,
                    riscv::fa3,
                    riscv::fa4,
                    riscv::fa5,
                    riscv::fa6,
                    riscv::fa7,
                },
            .save =
                {
                    riscv::fs0,
                    riscv::fs1,
                    riscv::fs2,
                    riscv::fs3,
                    riscv::fs4,
                    riscv::fs5,
                    riscv::fs6,
                    riscv::fs7,
                    riscv::fs8,
                    riscv::fs9,
                    riscv::fs10,
                    riscv::fs11,
                },
            .tmp =
                {
                    riscv::ft0,
                    riscv::ft1,
                    riscv::ft2,
                    riscv::ft3,
                    riscv::ft4,
                    riscv::ft5,
                    riscv::ft6,
                    riscv::ft7,
                    riscv::ft8,
                    riscv::ft9,
                    riscv::ft10,
                    riscv::ft11,
                },

            .caller_save{
                riscv::fa0, riscv::fa1, riscv::fa2, riscv::fa3, riscv::fa4,  riscv::fa5,  riscv::fa6,
                riscv::fa7, riscv::ft0, riscv::ft1, riscv::ft2, riscv::ft3,  riscv::ft4,  riscv::ft5,
                riscv::ft6, riscv::ft7, riscv::ft8, riscv::ft9, riscv::ft10, riscv::ft11,
            },

            .callee_save{
                riscv::fs0,
                riscv::fs1,
                riscv::fs2,
                riscv::fs3,
                riscv::fs4,
                riscv::fs5,
                riscv::fs6,
                riscv::fs7,
                riscv::fs8,
                riscv::fs9,
                riscv::fs10,
                riscv::fs11,
            },
        },
};