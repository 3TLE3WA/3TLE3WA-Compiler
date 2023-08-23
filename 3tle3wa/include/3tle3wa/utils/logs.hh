#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iomanip>

#define CRVC_UNUSE [[maybe_unused]]

extern uint64_t compile_start_time__;

// #define NDEBUG

#ifndef NDEBUG

#define Log(fmt, ...)                       \
    fprintf(stdout,                         \
            "\033[1;34m"                    \
            "%s:%d: " fmt " [%f]\033[0m\n", \
            __FILE__, __LINE__, ##__VA_ARGS__, ((clock() - compile_start_time__) / (float)CLOCKS_PER_SEC))

#define Warn(fmt, ...)                 \
    fprintf(stderr,                    \
            "\033[1;31m"               \
            "%s:%d: " fmt "\033[0m\n", \
            __FILE__, __LINE__, ##__VA_ARGS__)

#define panic(fmt, ...)           \
    do {                          \
        Warn(fmt, ##__VA_ARGS__); \
        abort();                  \
    } while (0)

#define Assert(expr, fmt, ...)                                   \
    do {                                                         \
        if (!(expr)) {                                           \
            panic(#expr " Assertion Fail: " fmt, ##__VA_ARGS__); \
        }                                                        \
    } while (0)

#else

#define Log(fmt, ...)
#define Warn(fmt, ...)
#define panic(fmt, ...)
#define Assert(expr, fmt, ...)

#endif

template <typename T>
void CRVC_UNUSED_PARAM(T front) {
    (void)front;
}

template <typename T, typename... Args>
void CRVC_UNUSED_PARAM(T front, Args&&... args) {
    (void)front;
    CRVC_UNUSED_PARAM(args...);
}
