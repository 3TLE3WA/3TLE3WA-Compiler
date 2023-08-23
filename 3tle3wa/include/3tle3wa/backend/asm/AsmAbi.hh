#pragma once

#include <set>
#include <vector>

struct AsmAbiInfo {
    struct {
        size_t nm_arg;
        size_t nm_save;
        size_t nm_tmp;

        size_t arg_bgn;
        size_t sp;
    } i;

    struct {
        size_t nm_arg;
        size_t nm_save;
        size_t nm_tmp;

        size_t arg_bgn;
    } f;
};

struct AsmRegInfo {
    struct {
        std::set<size_t> arg;
        std::set<size_t> save;
        std::set<size_t> tmp;

        std::set<size_t> caller_save;
        std::set<size_t> callee_save;
    } i;

    struct {
        std::set<size_t> arg;
        std::set<size_t> save;
        std::set<size_t> tmp;

        std::set<size_t> caller_save;
        std::set<size_t> callee_save;
    } f;
};

extern AsmAbiInfo abi_info;

extern AsmRegInfo abi_reg_info;

extern const char *gpr[];