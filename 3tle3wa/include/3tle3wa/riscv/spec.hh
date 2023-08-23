#pragma once

#include <cstddef>
#include <cstdint>

extern const char *gpr[];

namespace riscv {

constexpr uint64_t zero = 0;
constexpr uint64_t ra = 1;
constexpr uint64_t sp = 2;
constexpr uint64_t gp = 3;
constexpr uint64_t tp = 4;
constexpr uint64_t t0 = 5;
constexpr uint64_t t1 = 6;
constexpr uint64_t t2 = 7;

constexpr uint64_t fp = 8;
constexpr uint64_t s0 = 8;

constexpr uint64_t s1 = 9;

constexpr uint64_t a0 = 10 + 0;
constexpr uint64_t a1 = 10 + 1;
constexpr uint64_t a2 = 10 + 2;
constexpr uint64_t a3 = 10 + 3;
constexpr uint64_t a4 = 10 + 4;
constexpr uint64_t a5 = 10 + 5;
constexpr uint64_t a6 = 10 + 6;
constexpr uint64_t a7 = 10 + 7;

constexpr uint64_t s2 = 18;
constexpr uint64_t s3 = 19;
constexpr uint64_t s4 = 20;
constexpr uint64_t s5 = 21;
constexpr uint64_t s6 = 22;
constexpr uint64_t s7 = 23;
constexpr uint64_t s8 = 24;
constexpr uint64_t s9 = 25;
constexpr uint64_t s10 = 26;
constexpr uint64_t s11 = 27;
constexpr uint64_t t3 = 28;
constexpr uint64_t t4 = 29;
constexpr uint64_t t5 = 30;
constexpr uint64_t t6 = 31;

constexpr uint64_t ft0 = 32;
constexpr uint64_t ft1 = 33;
constexpr uint64_t ft2 = 34;
constexpr uint64_t ft3 = 35;
constexpr uint64_t ft4 = 36;
constexpr uint64_t ft5 = 37;
constexpr uint64_t ft6 = 38;
constexpr uint64_t ft7 = 39;
constexpr uint64_t fs0 = 40;
constexpr uint64_t fs1 = 41;

constexpr uint64_t fa0 = 42 + 0;
constexpr uint64_t fa1 = 42 + 1;
constexpr uint64_t fa2 = 42 + 2;
constexpr uint64_t fa3 = 42 + 3;
constexpr uint64_t fa4 = 42 + 4;
constexpr uint64_t fa5 = 42 + 5;
constexpr uint64_t fa6 = 42 + 6;
constexpr uint64_t fa7 = 42 + 7;

constexpr uint64_t fs2 = 50;
constexpr uint64_t fs3 = 51;
constexpr uint64_t fs4 = 52;
constexpr uint64_t fs5 = 53;
constexpr uint64_t fs6 = 54;
constexpr uint64_t fs7 = 55;
constexpr uint64_t fs8 = 56;
constexpr uint64_t fs9 = 57;
constexpr uint64_t fs10 = 58;
constexpr uint64_t fs11 = 59;
constexpr uint64_t ft8 = 60;
constexpr uint64_t ft9 = 61;
constexpr uint64_t ft10 = 62;
constexpr uint64_t ft11 = 63;

}  // namespace riscv