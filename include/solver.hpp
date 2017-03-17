#ifndef SOLVER_HPP_
#define SOLVER_HPP_ 1

#include <cstdint>
#include <string>

#define SET_AFFINITY 0

#define IS_ALIGNED_TO(p, s) (((uintptr_t)(p) & ((s) - 1)) == 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)

constexpr unsigned int MAX_NONCE = 100000000;
constexpr unsigned int N_BINS = 32;
constexpr unsigned int BIN_SHIFT = 59;

static_assert((-1ul >> BIN_SHIFT) + 1 == N_BINS, "invalid bins");

constexpr int TYPE_SORTED_LIST         = 1;
constexpr int TYPE_REVERSE_SORTED_LIST = 2;
constexpr int TYPE_SHORTEST_PATH       = 3;

struct challenge {
    int id, type;
    unsigned int n, m, hash_prefix_len;
    uint16_t prefix4;
    char hash_prefix[65];
    char prev_hash[65];
};

struct solution {
    unsigned int nonce;
    int id;
    char hash[65];
};

solution solve(const char *json);

void parse(const char *json, challenge &c);

void init_solver();

#endif
