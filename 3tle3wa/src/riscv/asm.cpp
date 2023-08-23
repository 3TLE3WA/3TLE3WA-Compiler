#include "3tle3wa/riscv/asm.hh"

#include <cinttypes>

#include "3tle3wa/riscv/spec.hh"

#define GENINST(fmt, ...)                                  \
    do {                                                   \
        FILE *out = open_memstream(&inst_txt_, &txt_len_); \
        fprintf(out, fmt, ##__VA_ARGS__);                  \
        fflush(out);                                       \
        fclose(out);                                       \
    } while (0)

#define TAB "\t"
#define COMMA ", "
#define NEWLINE "\n"

namespace riscv {

Instruction::Instruction(uint64_t rd, uint64_t rs, uint64_t rt, int64_t imm) : rd_(rd), rs_(rs), rt_(rt), imm_(imm) {}

void Instruction::formatString(FILE *fp) {
    // avoid format
    fprintf(fp, "\t%s\n", inst_txt_);
}

//
// asm with label
//

LA_LB::LA_LB(uint64_t rd, const char *sym) : Instruction(rd) {
    // avoid format
    GENINST("la" TAB "%s" COMMA "%s", gpr[rd], sym);
}

LLA_LB::LLA_LB(uint64_t rd, const char *sym) : Instruction(rd) {
    // avoid format
    GENINST("lla" TAB "%s" COMMA "%s", gpr[rd], sym);
}

LLA_LB_OFF::LLA_LB_OFF(uint64_t rd, const char *sym, uint64_t off) : Instruction(rd, riscv::zero, riscv::zero, off) {
    // avoid format
    GENINST("lla" TAB "%s" COMMA "%s" "+" "%" PRIu64, gpr[rd], sym, off);
}

LW_LB::LW_LB(uint64_t rd, const char *sym) : Instruction(rd) {
    // avoid format
    GENINST("lw" TAB "%s" COMMA "%s", gpr[rd], sym);
}

LD_LB::LD_LB(uint64_t rd, const char *sym) : Instruction(rd) {
    // avoid format
    GENINST("ld" TAB "%s" COMMA "%s", gpr[rd], sym);
}

SW_LB::SW_LB(uint64_t rs, const char *sym, uint64_t rt) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("sw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], sym, gpr[rt]);
}

SD_LB::SD_LB(uint64_t rs, const char *sym, uint64_t rt) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("sd" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], sym, gpr[rt]);
}

FLW_LB::FLW_LB(uint64_t frd, const char *sym, uint64_t rt) : Instruction(frd, rt) {
    // avoid format
    GENINST("flw" TAB "%s" COMMA "%s" COMMA "%s", gpr[frd], sym, gpr[rt]);
}

FSW_LB::FSW_LB(uint64_t frs, const char *sym, uint64_t rt) : Instruction(riscv::zero, frs, rt) {
    // avoid format
    GENINST("fsw" TAB "%s" COMMA "%s" COMMA "%s", gpr[frs], sym, gpr[rt]);
}

//
// misc int op
//

MV::MV(uint64_t rd, uint64_t rs) : Instruction(rd, rs) {
    // avoid format
    GENINST("mv" TAB "%s" COMMA "%s", gpr[rd], gpr[rs]);
}

LI::LI(uint64_t rd, int64_t imm) : Instruction(rd, riscv::zero, riscv::zero, imm) {
    // avoid format
    GENINST("li" TAB "%s" COMMA "%" PRId64, gpr[rd], imm);
}

LUI::LUI(uint64_t rd, int64_t imm) : Instruction(rd, riscv::zero, riscv::zero, imm) {
    // avoid format
    GENINST("lui" TAB "%s" COMMA "%" PRId64, gpr[rd], imm);
}

SEXT_W::SEXT_W(uint64_t rd, uint64_t rs) : Instruction(rd, rs) {
    // avoid format
    GENINST("sext.w" TAB "%s" COMMA "%s", gpr[rd], gpr[rs]);
}

//
// set flag
//

SEQZ::SEQZ(uint64_t rd, uint64_t rs) : Instruction(rd, rs) {
    // avoid format
    GENINST("seqz" TAB "%s" COMMA "%s", gpr[rd], gpr[rs]);
}

SNEZ::SNEZ(uint64_t rd, uint64_t rs) : Instruction(rd, rs) {
    // avoid format
    GENINST("snez" TAB "%s" COMMA "%s", gpr[rd], gpr[rs]);
}

SLTZ::SLTZ(uint64_t rd, uint64_t rs) : Instruction(rd, rs) {
    // avoid format
    GENINST("sltz" TAB "%s" COMMA "%s", gpr[rd], gpr[rs]);
}

SGTZ::SGTZ(uint64_t rd, uint64_t rs) : Instruction(rd, rs) {
    // avoid format
    GENINST("sgtz" TAB "%s" COMMA "%s", gpr[rd], gpr[rs]);
}

SLT::SLT(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("slt" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SLTI::SLTI(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("slti" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

SLTIU::SLTIU(uint64_t rd, uint64_t rs, uint64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("sltiu" TAB "%s" COMMA "%s" COMMA "%" PRIu64, gpr[rd], gpr[rs], imm);
}

SGT::SGT(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("slt" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rt], gpr[rs]);
}

SLTU::SLTU(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("sltu" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SGTU::SGTU(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("sltu" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rt], gpr[rs]);
}

//
// misc flt op
//

FMV_S::FMV_S(uint64_t frd, uint64_t frs) : Instruction(frd, frs) {
    // avoid format
    GENINST("fmv.s" TAB "%s" COMMA "%s", gpr[frd], gpr[frs]);
}

FABS_S::FABS_S(uint64_t frd, uint64_t frs) : Instruction(frd, frs) {
    // avoid format
    GENINST("fabs.s" TAB "%s" COMMA "%s", gpr[frd], gpr[frs]);
}

FNEG_S::FNEG_S(uint64_t frd, uint64_t frs) : Instruction(frd, frs) {
    // avoid format
    GENINST("fneg.s" TAB "%s" COMMA "%s", gpr[frd], gpr[frs]);
}

//
// branch with zero
//

BEQZ::BEQZ(uint64_t rs, const char *off) : Instruction(riscv::zero, rs) {
    // avoid format
    GENINST("beqz" TAB "%s" COMMA "%s", gpr[rs], off);
}

BNEZ::BNEZ(uint64_t rs, const char *off) : Instruction(riscv::zero, rs) {
    // avoid format
    GENINST("bnez" TAB "%s" COMMA "%s", gpr[rs], off);
}

BLEZ::BLEZ(uint64_t rs, const char *off) : Instruction(riscv::zero, rs) {
    // avoid format
    GENINST("blez" TAB "%s" COMMA "%s", gpr[rs], off);
}

BGEZ::BGEZ(uint64_t rs, const char *off) : Instruction(riscv::zero, rs) {
    // avoid format
    GENINST("bgez" TAB "%s" COMMA "%s", gpr[rs], off);
}

BLTZ::BLTZ(uint64_t rs, const char *off) : Instruction(riscv::zero, rs) {
    // avoid format
    GENINST("bltz" TAB "%s" COMMA "%s", gpr[rs], off);
}

BGTZ::BGTZ(uint64_t rs, const char *off) : Instruction(riscv::zero, rs) {
    // avoid format
    GENINST("bgtz" TAB "%s" COMMA "%s", gpr[rs], off);
}

//
// branch 2 op
//

BEQ::BEQ(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("beq" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

BNE::BNE(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("bne" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

BLT::BLT(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("blt" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

BGE::BGE(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("bge" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

BGT::BGT(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("bgt" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

BLE::BLE(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("ble" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

BLTU::BLTU(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("bltu" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

BGEU::BGEU(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("bgeu" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

BGTU::BGTU(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("bgtu" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

BLEU::BLEU(uint64_t rs, uint64_t rt, const char *off) : Instruction(riscv::zero, rs, rt) {
    // avoid format
    GENINST("bleu" TAB "%s" COMMA "%s" COMMA "%s", gpr[rs], gpr[rt], off);
}

//
// j misc
//

J::J(const char *off) {
    // avoid format
    GENINST("j" TAB "%s", off);
}

JAL::JAL(const char *off) {
    // avoid format
    GENINST("jal" TAB "%s", off);
}

JR::JR(uint64_t rs) : Instruction(riscv::zero, rs) {
    // avoid format
    GENINST("jr" TAB "%s", gpr[rs]);
}

JALR::JALR(uint64_t rs) : Instruction(riscv::zero, rs) {
    // avoid format
    GENINST("jalr" TAB "%s", gpr[rs]);
}

RET::RET() {
    // avoid format
    GENINST("ret");
}

CALL::CALL(const char *off) {
    // avoid format
    GENINST("call" TAB "%s", off);
}

TAIL::TAIL(const char *off) {
    // avoid format
    GENINST("tail" TAB "%s", off);
}

//
// RV64I
//

// ls

LW::LW(uint64_t rd, uint64_t rs, int64_t off) : Instruction(rd, rs, riscv::zero, off) {
    // avoid format
    GENINST("lw" TAB "%s" COMMA "%" PRId64 "(%s)", gpr[rd], off, gpr[rs]);
}

LD::LD(uint64_t rd, uint64_t rs, int64_t off) : Instruction(rd, rs, riscv::zero, off) {
    // avoid format
    GENINST("ld" TAB "%s" COMMA "%" PRId64 "(%s)", gpr[rd], off, gpr[rs]);
}

SW::SW(uint64_t rs, uint64_t rt, int64_t off) : Instruction(riscv::zero, rs, rt, off) {
    // avoid format
    GENINST("sw" TAB "%s" COMMA "%" PRId64 "(%s)", gpr[rs], off, gpr[rt]);
}

SD::SD(uint64_t rs, uint64_t rt, int64_t off) : Instruction(riscv::zero, rs, rt, off) {
    // avoid format
    GENINST("sd" TAB "%s" COMMA "%" PRId64 "(%s)", gpr[rs], off, gpr[rt]);
}

// imm 64

ADDI::ADDI(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("addi" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

XORI::XORI(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("xori" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

ORI::ORI(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("ori" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

ANDI::ANDI(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("andi" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

SLLI::SLLI(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("slli" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

SRLI::SRLI(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("srli" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

SRAI::SRAI(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("srai" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

// op 64

ADD::ADD(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("add" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SUB::SUB(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("sub" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SLL::SLL(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("sll" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

XOR::XOR(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("xor" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SRL::SRL(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("srl" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SRA::SRA(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("sra" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

OR::OR(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("or" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

AND::AND(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("and" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

// imm 32

ADDIW::ADDIW(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("addiw" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

SLLIW::SLLIW(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("slliw" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

SRLIW::SRLIW(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("srliw" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

SRAIW::SRAIW(uint64_t rd, uint64_t rs, int64_t imm) : Instruction(rd, rs, riscv::zero, imm) {
    // avoid format
    GENINST("sraiw" TAB "%s" COMMA "%s" COMMA "%" PRId64, gpr[rd], gpr[rs], imm);
}

// op 32

ADDW::ADDW(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("addw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SUBW::SUBW(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("subw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SLLW::SLLW(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("sllw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SRLW::SRLW(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("srlw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

SRAW::SRAW(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("sraw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

//
// RV64M
//

MUL::MUL(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("mul" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

DIV::DIV(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("div" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

DIVU::DIVU(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("divu" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

REM::REM(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("rem" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

REMU::REMU(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("remu" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

MULW::MULW(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("mulw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

DIVW::DIVW(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("divw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

DIVWU::DIVWU(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("divuw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

REMW::REMW(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("remw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

REMWU::REMWU(uint64_t rd, uint64_t rs, uint64_t rt) : Instruction(rd, rs, rt) {
    // avoid format
    GENINST("remuw" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[rs], gpr[rt]);
}

//
// RV64F
//

FLW::FLW(uint64_t frd, uint64_t rs, int64_t off) : Instruction(frd, rs, riscv::zero, off) {
    // avoid format
    GENINST("flw" TAB "%s" COMMA "%" PRId64 "(%s)", gpr[frd], off, gpr[rs]);
}

FSW::FSW(uint64_t frs, uint64_t rt, int64_t off) : Instruction(riscv::zero, frs, rt, off) {
    // avoid format
    GENINST("fsw" TAB TAB "%s" COMMA "%" PRId64 "(%s)", gpr[frs], off, gpr[rt]);
}

FMADD_S::FMADD_S(uint64_t frd, uint64_t frs, uint64_t frt, uint64_t fra) {
    // avoid format
    GENINST("fmadd.s" TAB "%s" COMMA "%s" COMMA "%s" COMMA "%s", gpr[frd], gpr[frs], gpr[frt], gpr[fra]);
}

FMSUB_S::FMSUB_S(uint64_t frd, uint64_t frs, uint64_t frt, uint64_t fra) {
    // avoid format
    GENINST("fmsub.s" TAB "%s" COMMA "%s" COMMA "%s" COMMA "%s", gpr[frd], gpr[frs], gpr[frt], gpr[fra]);
}

FNMADD_S::FNMADD_S(uint64_t frd, uint64_t frs, uint64_t frt, uint64_t fra) {
    // avoid format
    GENINST("fnmadd.s" TAB "%s" COMMA "%s" COMMA "%s" COMMA "%s", gpr[frd], gpr[frs], gpr[frt], gpr[fra]);
}

FNMSUB_S::FNMSUB_S(uint64_t frd, uint64_t frs, uint64_t frt, uint64_t fra) {
    // avoid format
    GENINST("fnmsub.s" TAB "%s" COMMA "%s" COMMA "%s" COMMA "%s", gpr[frd], gpr[frs], gpr[frt], gpr[fra]);
}

FADD_S::FADD_S(uint64_t frd, uint64_t frs, uint64_t frt) : Instruction(frd, frs, frt) {
    // avoid format
    GENINST("fadd.s" TAB "%s" COMMA "%s" COMMA "%s", gpr[frd], gpr[frs], gpr[frt]);
}

FSUB_S::FSUB_S(uint64_t frd, uint64_t frs, uint64_t frt) : Instruction(frd, frs, frt) {
    // avoid format
    GENINST("fsub.s" TAB "%s" COMMA "%s" COMMA "%s", gpr[frd], gpr[frs], gpr[frt]);
}

FMUL_S::FMUL_S(uint64_t frd, uint64_t frs, uint64_t frt) : Instruction(frd, frs, frt) {
    // avoid format
    GENINST("fmul.s" TAB "%s" COMMA "%s" COMMA "%s", gpr[frd], gpr[frs], gpr[frt]);
}

FDIV_S::FDIV_S(uint64_t frd, uint64_t frs, uint64_t frt) : Instruction(frd, frs, frt) {
    // avoid format
    GENINST("fdiv.s" TAB "%s" COMMA "%s" COMMA "%s", gpr[frd], gpr[frs], gpr[frt]);
}

// 目前固定舍入方式为 rtz

FCVT_W_S::FCVT_W_S(uint64_t rd, uint64_t frs) : Instruction(rd, frs) {
    // avoid format
    GENINST("fcvt.w.s" TAB "%s" COMMA "%s" COMMA "rtz", gpr[rd], gpr[frs]);
}

FCVT_S_W::FCVT_S_W(uint64_t frd, uint64_t rs) : Instruction(frd, rs) {
    // avoid format
    GENINST("fcvt.s.w" TAB "%s" COMMA "%s" COMMA "rtz", gpr[frd], gpr[rs]);
}

FEQ::FEQ(uint64_t rd, uint64_t frs, uint64_t frt) : Instruction(rd, frs, frt) {
    // avoid format
    GENINST("feq.s" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[frs], gpr[frt]);
}

FLT::FLT(uint64_t rd, uint64_t frs, uint64_t frt) : Instruction(rd, frs, frt) {
    // avoid format
    GENINST("flt.s" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[frs], gpr[frt]);
}

FLE::FLE(uint64_t rd, uint64_t frs, uint64_t frt) : Instruction(rd, frs, frt) {
    // avoid format
    GENINST("fle.s" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[frs], gpr[frt]);
}

FMV_W_X::FMV_W_X(uint64_t frd, uint64_t rs) : Instruction(frd, rs) {
    // avoid format
    GENINST("fmv.w.x" TAB "%s" COMMA "%s", gpr[frd], gpr[rs]);
}

FMV_X_W::FMV_X_W(uint64_t rd, uint64_t frs) : Instruction(rd, frs) {
    // avoid format
    GENINST("fmv.x.w" TAB "%s" COMMA "%s", gpr[rd], gpr[frs]);
}

FMAX_S::FMAX_S(uint64_t rd, uint64_t frs, uint64_t frt) : Instruction(rd, frs, frt) {
    // avoid format
    GENINST("fmax.s" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[frs], gpr[frt]);
}

FMIN_S::FMIN_S(uint64_t rd, uint64_t frs, uint64_t frt) : Instruction(rd, frs, frt) {
    // avoid format
    GENINST("fmin.s" TAB "%s" COMMA "%s" COMMA "%s", gpr[rd], gpr[frs], gpr[frt]);
}

}  // namespace riscv
