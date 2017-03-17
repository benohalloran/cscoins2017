#ifndef UTILS_HPP_
#define UTILS_HPP_ 1

#include <cstdint>

#define IS_ALIGNED_TO(p, s) (((uintptr_t)(p) & ((s) - 1)) == 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)

#endif
