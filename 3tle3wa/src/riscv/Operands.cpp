#include "3tle3wa/riscv/asm.hh"

const std::vector<size_t> riscv::Instruction::GetOperands() const {
    std::vector<size_t> vec;
    if (rs_ != riscv::zero) {
        vec.push_back(rs_);
    }
    if (rt_ != riscv::zero) {
        vec.push_back(rt_);
    }
    return vec;
}

size_t riscv::Instruction::GetResult() const { return rd_; }

int64_t riscv::Instruction::GetImm() const { return imm_; }

size_t riscv::Instruction::GetRs() const { return rs_; }

size_t riscv::Instruction::GetRt() const { return rt_; }

size_t riscv::Instruction::GetRd() const { return rd_; }