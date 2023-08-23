#pragma once

#include <vector>
#include <cstddef>

#include "3tle3wa/backend/asm/AsmInstruction.hh"
#include "3tle3wa/riscv/spec.hh"

struct Transaction;

namespace riscv {

class Instruction : public AsmInstruction {
   protected:
    virtual void formatString(FILE *fp) final;

    char *inst_txt_{nullptr};
    size_t txt_len_{};

    uint64_t rd_{};
    uint64_t rs_{};
    uint64_t rt_{};
    int64_t imm_{};

   public:
    const std::vector<size_t> GetOperands() const;
    size_t GetResult() const;
    int64_t GetImm() const;
    size_t GetRs() const;
    size_t GetRt() const;
    size_t GetRd() const;

    Instruction(uint64_t rd = riscv::zero, uint64_t rs = riscv::zero, uint64_t rt = riscv::zero, int64_t imm = 0);
};

#define RVINST(name, ...)                \
    class name : public Instruction {    \
       public:                           \
        name(__VA_ARGS__);               \
        SCHED_TYPE GetSchedType() const; \
    }

//
// asm with label
//

RVINST(LA_LB, uint64_t rd, const char *sym);
RVINST(LLA_LB, uint64_t rd, const char *sym);
RVINST(LLA_LB_OFF, uint64_t rd, const char *sym, uint64_t off);

RVINST(LW_LB, uint64_t rd, const char *sym);
RVINST(LD_LB, uint64_t rd, const char *sym);
RVINST(SW_LB, uint64_t rs, const char *sym, uint64_t rt);
RVINST(SD_LB, uint64_t rs, const char *sym, uint64_t rt);
RVINST(FLW_LB, uint64_t frd, const char *sym, uint64_t rt);
RVINST(FSW_LB, uint64_t frs, const char *sym, uint64_t rt);

//
// misc int op
//

RVINST(MV, uint64_t rd, uint64_t rs);
RVINST(LI, uint64_t rd, int64_t imm);
RVINST(LUI, uint64_t rd, int64_t imm);
RVINST(SEXT_W, uint64_t rd, uint64_t rs);

//
// set flag
//

RVINST(SEQZ, uint64_t rd, uint64_t rs);
RVINST(SNEZ, uint64_t rd, uint64_t rs);
RVINST(SLTZ, uint64_t rd, uint64_t rs);
RVINST(SGTZ, uint64_t rd, uint64_t rs);
RVINST(SLT, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SLTI, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(SLTIU, uint64_t rd, uint64_t rs, uint64_t imm);
RVINST(SGT, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SLTU, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SGTU, uint64_t rd, uint64_t rs, uint64_t rt);

//
// misc flt op
//

RVINST(FMV_S, uint64_t frd, uint64_t frs);
RVINST(FABS_S, uint64_t frd, uint64_t frs);
RVINST(FNEG_S, uint64_t frd, uint64_t frs);

//
// branch with zero
//

RVINST(BEQZ, uint64_t rs, const char *off);
RVINST(BNEZ, uint64_t rs, const char *off);
RVINST(BLEZ, uint64_t rs, const char *off);
RVINST(BGEZ, uint64_t rs, const char *off);
RVINST(BLTZ, uint64_t rs, const char *off);
RVINST(BGTZ, uint64_t rs, const char *off);

//
// branch 2 op
//

RVINST(BEQ, uint64_t rs, uint64_t rt, const char *off);
RVINST(BNE, uint64_t rs, uint64_t rt, const char *off);
RVINST(BLT, uint64_t rs, uint64_t rt, const char *off);
RVINST(BGE, uint64_t rs, uint64_t rt, const char *off);
RVINST(BGT, uint64_t rs, uint64_t rt, const char *off);
RVINST(BLE, uint64_t rs, uint64_t rt, const char *off);
RVINST(BLTU, uint64_t rs, uint64_t rt, const char *off);
RVINST(BGEU, uint64_t rs, uint64_t rt, const char *off);
RVINST(BGTU, uint64_t rs, uint64_t rt, const char *off);
RVINST(BLEU, uint64_t rs, uint64_t rt, const char *off);

//
// j misc
//

RVINST(J, const char *off);
RVINST(JAL, const char *off);
RVINST(JR, uint64_t rs);
RVINST(JALR, uint64_t rs);
RVINST(RET);
RVINST(CALL, const char *off);
RVINST(TAIL, const char *off);

//
// RV64I
//

// ls

RVINST(LW, uint64_t rd, uint64_t rs, int64_t off);
RVINST(LD, uint64_t rd, uint64_t rs, int64_t off);
RVINST(SW, uint64_t rs, uint64_t rt, int64_t off);
RVINST(SD, uint64_t rs, uint64_t rt, int64_t off);

// imm 64

RVINST(ADDI, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(XORI, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(ORI, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(ANDI, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(SLLI, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(SRLI, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(SRAI, uint64_t rd, uint64_t rs, int64_t imm);

// op 64

RVINST(ADD, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SUB, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SLL, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(XOR, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SRL, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SRA, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(OR, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(AND, uint64_t rd, uint64_t rs, uint64_t rt);

// imm 32

RVINST(ADDIW, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(SLLIW, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(SRLIW, uint64_t rd, uint64_t rs, int64_t imm);
RVINST(SRAIW, uint64_t rd, uint64_t rs, int64_t imm);

// op 32

RVINST(ADDW, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SUBW, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SLLW, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SRLW, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(SRAW, uint64_t rd, uint64_t rs, uint64_t rt);

//
// RV64M
//

RVINST(MUL, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(DIV, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(DIVU, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(REM, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(REMU, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(MULW, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(DIVW, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(DIVWU, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(REMW, uint64_t rd, uint64_t rs, uint64_t rt);
RVINST(REMWU, uint64_t rd, uint64_t rs, uint64_t rt);

//
// RV64F
//

RVINST(FLW, uint64_t frd, uint64_t rs, int64_t off);
RVINST(FSW, uint64_t frs, uint64_t rt, int64_t off);
RVINST(FMADD_S, uint64_t frd, uint64_t frs, uint64_t frt, uint64_t fra);
RVINST(FMSUB_S, uint64_t frd, uint64_t frs, uint64_t frt, uint64_t fra);
RVINST(FNMADD_S, uint64_t frd, uint64_t frs, uint64_t frt, uint64_t fra);
RVINST(FNMSUB_S, uint64_t frd, uint64_t frs, uint64_t frt, uint64_t fra);
RVINST(FADD_S, uint64_t frd, uint64_t frs, uint64_t frt);
RVINST(FSUB_S, uint64_t frd, uint64_t frs, uint64_t frt);
RVINST(FMUL_S, uint64_t frd, uint64_t frs, uint64_t frt);
RVINST(FDIV_S, uint64_t frd, uint64_t frs, uint64_t frt);
RVINST(FCVT_W_S, uint64_t rd, uint64_t frs);
RVINST(FCVT_S_W, uint64_t frd, uint64_t rs);
RVINST(FEQ, uint64_t rd, uint64_t frs, uint64_t frt);
RVINST(FLT, uint64_t rd, uint64_t frs, uint64_t frt);
RVINST(FLE, uint64_t rd, uint64_t frs, uint64_t frt);
RVINST(FMV_W_X, uint64_t frd, uint64_t rs);
RVINST(FMV_X_W, uint64_t rd, uint64_t frs);
RVINST(FMAX_S, uint64_t rd, uint64_t frs, uint64_t frt);
RVINST(FMIN_S, uint64_t rd, uint64_t frs, uint64_t frt);

}  // namespace riscv
