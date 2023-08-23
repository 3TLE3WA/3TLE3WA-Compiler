#pragma once

#include <cstddef>
#include <cstdint>
#include <variant>

/**
 * @brief C 形式的 reinterpretation
 */
union Reinterp32Bit {
    float flt_;
    uint32_t u32_;
    int32_t i32_;
};

/**
 * @brief C 形式的 reinterpretation
 */
union Reinterp64Bit {
    double dlb_;
    uint64_t u64_;
    int64_t i64_;
};

/**
 * @brief 常数展开信息
 *
 */
struct ConstValueInfo {
    size_t width_{0};
    bool isflt_{false};
    bool illegel_{false};

    Reinterp32Bit v32_{0};
    Reinterp64Bit v64_{0};
};

using ConstPackage = std::variant<bool, char, int32_t, float, int64_t>;

/**
 * @brief 展开 llvm ir 常量指针
 *
 * @param cstptr 常量指针
 * @return cval_info 常量展开信息
 */
static inline ConstValueInfo XConstValue(const ConstPackage &cpk) {
    ConstValueInfo cvalinfo;

    std::visit(
        [&cvalinfo](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, float>) {
                cvalinfo.v32_.flt_ = arg;
                cvalinfo.isflt_ = true;
                cvalinfo.width_ = 32;
            } else if constexpr (std::is_same_v<T, double>) {
                cvalinfo.v32_.flt_ = arg;
                cvalinfo.v64_.dlb_ = arg;
                cvalinfo.isflt_ = true;
                cvalinfo.width_ = 64;
            } else if constexpr (std::is_same_v<T, int32_t>) {
                cvalinfo.v32_.i32_ = arg;
                cvalinfo.isflt_ = false;
                cvalinfo.width_ = 32;
            } else if constexpr (std::is_same_v<T, bool>) {
                cvalinfo.v32_.i32_ = arg;
                cvalinfo.isflt_ = false;
                cvalinfo.width_ = 32;
            } else if constexpr (std::is_same_v<T, char>) {
                cvalinfo.v32_.i32_ = arg;
                cvalinfo.isflt_ = false;
                cvalinfo.width_ = 32;
            } else if constexpr (std::is_same_v<T, int64_t>) {
                cvalinfo.v64_.i64_ = arg;
                cvalinfo.isflt_ = false;
                cvalinfo.width_ = 64;
            } else {
                // mean illegel
                cvalinfo.illegel_ = true;
            }
        },
        cpk);

    return cvalinfo;
}
