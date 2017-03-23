#ifndef SOLVER_HPP_
#define SOLVER_HPP_ 1

#include <cstdint>
#include <string>

#define SET_AFFINITY 0

#define RESTRICT __restrict__
#define ALIGN_TO(s) __attribute__((aligned(s)))
#define IS_ALIGNED_TO(p, s) (((uintptr_t)(p) & ((s) - 1)) == 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#ifndef TEAMS
#define TEAMS 1
#define TEAM_ID 0
#endif

static_assert(sizeof(long) == 8, "long too small");

constexpr unsigned int N_BINS = 16;
constexpr unsigned int BIN_SHIFT = 60;

static_assert((-1ul >> BIN_SHIFT) + 1 == N_BINS, "invalid bins");

constexpr int TYPE_SORTED_LIST = 1;
constexpr int TYPE_REVERSE_SORTED_LIST = 2;
constexpr int TYPE_SHORTEST_PATH = 3;

template <size_t N>
union buffer {
    uint8_t c[N];
    uint16_t s[(N + 1) / 2];
    uint32_t i[(N + 3) / 4];
    uint64_t l[(N + 7) / 8];
};

template <>
union buffer<2> {
    uint8_t c[2];
    uint16_t s;
};

template <>
union buffer<4> {
    uint8_t c[4];
    uint16_t s[2];
    uint32_t i;
};

template <>
union buffer<8> {
    uint8_t c[8];
    uint16_t s[4];
    uint32_t i[2];
    uint64_t l;
};

struct challenge {
    buffer<65> hash_prefix, prev_hash;
    int id, type;
    unsigned int n, m, hash_prefix_len;
    uint16_t prefix4;
};

struct solution {
    unsigned long nonce;
    int id;
    char hash[65];
};

solution solve(const char *json);

void parse(const char *json, challenge &c);

void init_solver();

#endif
