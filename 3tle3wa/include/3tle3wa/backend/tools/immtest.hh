#pragma once

#include <cstddef>
#include <cstdint>

/**
 * @brief 判断立即数 imm 是否能够用宽度为 width bit 的补码表示
 *
 * @param width 指定宽度
 * @param imm 立即数
 * @return true
 * @return false
 */
static inline constexpr bool ImmWithin(size_t width, int64_t imm) {
    uint64_t max_ = (1ul << (width - 1)) - 1;
    uint64_t min_ = ~(1ul << (width - 1)) + 1;
    return (int64_t)min_ <= imm and imm <= (int64_t)max_;
}

static_assert(ImmWithin(12, 0b011111111111));
static_assert(ImmWithin(12, 0b000000000000));
static_assert(ImmWithin(12, -0b100000000000));
static_assert(ImmWithin(12, 0b100000000000) == false);
static_assert(ImmWithin(12, 2048) == false);
static_assert(ImmWithin(12, -2048));

/**
 * @brief 不小于 num 的 align 的最小倍数
 *
 * @param align
 * @param num
 * @return constexpr uint64_t
 */
static inline constexpr uint64_t RoundUp(uint64_t align, uint64_t num) {
    uint64_t integer = num / align;
    uint64_t rest = num % align;
    return (integer + !!(rest)) * align;
}

static_assert(RoundUp(16, 0) == 0);
static_assert(RoundUp(16, 16) == 16);
static_assert(RoundUp(16, 23) == 32);
static_assert(RoundUp(16, 48) == 48);

/**
 * @brief 符号拓展
 * 
 * @param width 
 * @param imm 
 * @return constexpr int32_t 
 */
static inline constexpr int32_t Sext32(uint32_t width, uint32_t imm) {
    size_t shift = 32 - width;
    int32_t x = imm << shift;
    return x >> shift;
}

static_assert(Sext32(12, 0b011111111111) == 0b011111111111);
static_assert(Sext32(12, 0b100000000000) == -2048);
