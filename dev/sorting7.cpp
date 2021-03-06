#include "solver.hpp"

#include <algorithm>
#include <cstring>

#define NOINLINE __attribute__((noinline))

using namespace std;

#define SWAP(a,b) { \
    const uint64_t tmp = x##a; \
    x##a = x##a < x##b ? x##a : x##b; \
    x##b ^= tmp ^ x##a; \
}

constexpr unsigned int AFS_N_BINS = 64;
constexpr unsigned int AFS_BIN_SHIFT = 6;

static void
sort16(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6], x7 = array[7], x8 = array[8], x9 = array[9], x10 = array[10], x11 = array[11], x12 = array[12], x13 = array[13], x14 = array[14], x15 = array[15];
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(12, 13); SWAP(14, 15); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(12, 14); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(13, 15); SWAP(0, 4); SWAP(8, 12); SWAP(1, 5); SWAP(9, 13); SWAP(2, 6); SWAP(10, 14); SWAP(3, 7); SWAP(11, 15); SWAP(0, 8); SWAP(1, 9); SWAP(2, 10); SWAP(3, 11); SWAP(4, 12); SWAP(5, 13); SWAP(6, 14); SWAP(7, 15); SWAP(5, 10); SWAP(6, 9); SWAP(3, 12); SWAP(13, 14); SWAP(7, 11); SWAP(1, 2); SWAP(4, 8); SWAP(1, 4); SWAP(7, 13); SWAP(2, 8); SWAP(11, 14); SWAP(2, 4); SWAP(5, 6); SWAP(9, 10); SWAP(11, 13); SWAP(3, 8); SWAP(7, 12); SWAP(6, 8); SWAP(10, 12); SWAP(3, 5); SWAP(7, 9); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8); SWAP(9, 10); SWAP(11, 12); SWAP(6, 7); SWAP(8, 9);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6; array[7] = x7; array[8] = x8; array[9] = x9; array[10] = x10; array[11] = x11; array[12] = x12; array[13] = x13; array[14] = x14; array[15] = x15;
}

static void
sort15(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6], x7 = array[7], x8 = array[8], x9 = array[9], x10 = array[10], x11 = array[11], x12 = array[12], x13 = array[13], x14 = array[14];
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(12, 13); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(12, 14); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(0, 4); SWAP(8, 12); SWAP(1, 5); SWAP(9, 13); SWAP(2, 6); SWAP(10, 14); SWAP(3, 7); SWAP(0, 8); SWAP(1, 9); SWAP(2, 10); SWAP(3, 11); SWAP(4, 12); SWAP(5, 13); SWAP(6, 14); SWAP(5, 10); SWAP(6, 9); SWAP(3, 12); SWAP(13, 14); SWAP(7, 11); SWAP(1, 2); SWAP(4, 8); SWAP(1, 4); SWAP(7, 13); SWAP(2, 8); SWAP(11, 14); SWAP(2, 4); SWAP(5, 6); SWAP(9, 10); SWAP(11, 13); SWAP(3, 8); SWAP(7, 12); SWAP(6, 8); SWAP(10, 12); SWAP(3, 5); SWAP(7, 9); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8); SWAP(9, 10); SWAP(11, 12); SWAP(6, 7); SWAP(8, 9);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6; array[7] = x7; array[8] = x8; array[9] = x9; array[10] = x10; array[11] = x11; array[12] = x12; array[13] = x13; array[14] = x14;
}

static void
sort14(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6], x7 = array[7], x8 = array[8], x9 = array[9], x10 = array[10], x11 = array[11], x12 = array[12], x13 = array[13];
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(12, 13); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(0, 4); SWAP(8, 12); SWAP(1, 5); SWAP(9, 13); SWAP(2, 6); SWAP(3, 7); SWAP(0, 8); SWAP(1, 9); SWAP(2, 10); SWAP(3, 11); SWAP(4, 12); SWAP(5, 13); SWAP(5, 10); SWAP(6, 9); SWAP(3, 12); SWAP(7, 11); SWAP(1, 2); SWAP(4, 8); SWAP(1, 4); SWAP(7, 13); SWAP(2, 8); SWAP(2, 4); SWAP(5, 6); SWAP(9, 10); SWAP(11, 13); SWAP(3, 8); SWAP(7, 12); SWAP(6, 8); SWAP(10, 12); SWAP(3, 5); SWAP(7, 9); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8); SWAP(9, 10); SWAP(11, 12); SWAP(6, 7); SWAP(8, 9);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6; array[7] = x7; array[8] = x8; array[9] = x9; array[10] = x10; array[11] = x11; array[12] = x12; array[13] = x13;
}

static void
sort13(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6], x7 = array[7], x8 = array[8], x9 = array[9], x10 = array[10], x11 = array[11], x12 = array[12];
    SWAP(1, 7); SWAP(9, 11); SWAP(3, 4); SWAP(5, 8); SWAP(0, 12); SWAP(2, 6); SWAP(0, 1); SWAP(2, 3); SWAP(4, 6); SWAP(8, 11); SWAP(7, 12); SWAP(5, 9); SWAP(0, 2); SWAP(3, 7); SWAP(10, 11); SWAP(1, 4); SWAP(6, 12); SWAP(7, 8); SWAP(11, 12); SWAP(4, 9); SWAP(6, 10); SWAP(3, 4); SWAP(5, 6); SWAP(8, 9); SWAP(10, 11); SWAP(1, 7); SWAP(2, 6); SWAP(9, 11); SWAP(1, 3); SWAP(4, 7); SWAP(8, 10); SWAP(0, 5); SWAP(2, 5); SWAP(6, 8); SWAP(9, 10); SWAP(1, 2); SWAP(3, 5); SWAP(7, 8); SWAP(4, 6); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(3, 4); SWAP(5, 6);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6; array[7] = x7; array[8] = x8; array[9] = x9; array[10] = x10; array[11] = x11; array[12] = x12;
}

static void
sort12(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6], x7 = array[7], x8 = array[8], x9 = array[9], x10 = array[10], x11 = array[11];
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(1, 2); SWAP(5, 6); SWAP(9, 10); SWAP(1, 5); SWAP(6, 10); SWAP(5, 9); SWAP(2, 6); SWAP(1, 5); SWAP(6, 10); SWAP(0, 4); SWAP(7, 11); SWAP(3, 7); SWAP(4, 8); SWAP(0, 4); SWAP(7, 11); SWAP(1, 4); SWAP(7, 10); SWAP(3, 8); SWAP(2, 3); SWAP(8, 9); SWAP(2, 4); SWAP(7, 9); SWAP(3, 5); SWAP(6, 8); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6; array[7] = x7; array[8] = x8; array[9] = x9; array[10] = x10; array[11] = x11;
}

static void
sort11(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6], x7 = array[7], x8 = array[8], x9 = array[9], x10 = array[10];
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(1, 3); SWAP(5, 7); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(1, 2); SWAP(5, 6); SWAP(9, 10); SWAP(1, 5); SWAP(6, 10); SWAP(5, 9); SWAP(2, 6); SWAP(1, 5); SWAP(6, 10); SWAP(0, 4); SWAP(3, 7); SWAP(4, 8); SWAP(0, 4); SWAP(1, 4); SWAP(7, 10); SWAP(3, 8); SWAP(2, 3); SWAP(8, 9); SWAP(2, 4); SWAP(7, 9); SWAP(3, 5); SWAP(6, 8); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6; array[7] = x7; array[8] = x8; array[9] = x9; array[10] = x10;
}

static void
sort10(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6], x7 = array[7], x8 = array[8], x9 = array[9];
    SWAP(4, 9); SWAP(3, 8); SWAP(2, 7); SWAP(1, 6); SWAP(0, 5); SWAP(1, 4); SWAP(6, 9); SWAP(0, 3); SWAP(5, 8); SWAP(0, 2); SWAP(3, 6); SWAP(7, 9); SWAP(0, 1); SWAP(2, 4); SWAP(5, 7); SWAP(8, 9); SWAP(1, 2); SWAP(4, 6); SWAP(7, 8); SWAP(3, 5); SWAP(2, 5); SWAP(6, 8); SWAP(1, 3); SWAP(4, 7); SWAP(2, 3); SWAP(6, 7); SWAP(3, 4); SWAP(5, 6); SWAP(4, 5);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6; array[7] = x7; array[8] = x8; array[9] = x9;
}

static void
sort9(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6], x7 = array[7], x8 = array[8];
    SWAP(0, 1); SWAP(3, 4); SWAP(6, 7); SWAP(1, 2); SWAP(4, 5); SWAP(7, 8); SWAP(0, 1); SWAP(3, 4); SWAP(6, 7); SWAP(0, 3); SWAP(3, 6); SWAP(0, 3); SWAP(1, 4); SWAP(4, 7); SWAP(1, 4); SWAP(2, 5); SWAP(5, 8); SWAP(2, 5); SWAP(1, 3); SWAP(5, 7); SWAP(2, 6); SWAP(4, 6); SWAP(2, 4); SWAP(2, 3); SWAP(5, 6);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6; array[7] = x7; array[8] = x8;
}

static void
sort8(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6], x7 = array[7];
    SWAP(0, 1); SWAP(2, 3); SWAP(0, 2); SWAP(1, 3); SWAP(1, 2); SWAP(4, 5); SWAP(6, 7); SWAP(4, 6); SWAP(5, 7); SWAP(5, 6); SWAP(0, 4); SWAP(1, 5); SWAP(1, 4); SWAP(2, 6); SWAP(3, 7); SWAP(3, 6); SWAP(2, 4); SWAP(3, 5); SWAP(3, 4);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6; array[7] = x7;
}

static void
sort7(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5], x6 = array[6];
    SWAP(1, 2); SWAP(0, 2); SWAP(0, 1); SWAP(3, 4); SWAP(5, 6); SWAP(3, 5); SWAP(4, 6); SWAP(4, 5); SWAP(0, 4); SWAP(0, 3); SWAP(1, 5); SWAP(2, 6); SWAP(2, 5); SWAP(1, 3); SWAP(2, 4); SWAP(2, 3);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5; array[6] = x6;
}

static void
sort6(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4], x5 = array[5];
    SWAP(1, 2); SWAP(0, 2); SWAP(0, 1); SWAP(4, 5); SWAP(3, 5); SWAP(3, 4); SWAP(0, 3); SWAP(1, 4); SWAP(2, 5); SWAP(2, 4); SWAP(1, 3); SWAP(2, 3);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4; array[5] = x5;
}

static void
sort5(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3], x4 = array[4];
    SWAP(0, 1); SWAP(3, 4); SWAP(2, 4); SWAP(2, 3); SWAP(0, 3); SWAP(0, 2); SWAP(1, 4); SWAP(1, 3); SWAP(1, 2);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3; array[4] = x4;
}

static void
sort4(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2], x3 = array[3];
    SWAP(0, 1); SWAP(2, 3); SWAP(0, 2); SWAP(1, 3); SWAP(1, 2);
    array[0] = x0; array[1] = x1; array[2] = x2; array[3] = x3;
}

static void
sort3(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1], x2 = array[2];
    SWAP(1, 2); SWAP(0, 2); SWAP(0, 1);
    array[0] = x0; array[1] = x1; array[2] = x2;
}

static void
sort2(uint64_t *array)
{
    uint64_t x0 = array[0], x1 = array[1];
    SWAP(0, 1);
    array[0] = x0; array[1] = x1;
}

static void
sort_empty(uint64_t *array)
{
    (void)array;
}

static void (*jmp_table[17])(uint64_t *) = {
    sort_empty, sort_empty, sort2, sort3, sort4, sort5, sort6, sort7, sort8,
    sort9, sort10, sort11, sort12, sort13, sort14, sort15, sort16
};

template <unsigned int S>
static void
american_flag_sort_1(uint64_t *array, unsigned int len)
{
    constexpr unsigned int B = 1 << S;
    constexpr unsigned int shift = BIN_SHIFT - S;
    unsigned int counts[B] = {0};
    unsigned int offsets[B + 1];
    unsigned int next_free[B];

    for (unsigned int i = 0; i < len; ++i) {
        ++counts[(array[i] >> shift) & (B - 1)];
    }
    offsets[0] = next_free[0] = 0;
    for (unsigned int i = 0; i < B - 1; ++i) {
        offsets[i + 1] = next_free[i + 1] = offsets[i] + counts[i];
    }
    offsets[B] = len;

    for (unsigned int i = 0; i < len; ++i) {
        const unsigned int bin = (array[i] >> shift) & (B - 1);
        array[len + next_free[bin]] = array[i];
        ++next_free[bin];
    }

    memcpy(array, array + len, len * sizeof(*array));

    for (unsigned int b = 0; b < B; ++b) {
        const unsigned int count = counts[b];
        if (count <= 16) {
            jmp_table[count](array + offsets[b]);
        } else {
            uint64_t *next_array = array + offsets[b];
            sort(next_array, next_array + count);
        }
    }
}

template <unsigned int S1, unsigned int S2>
static void
american_flag_sort_2b(uint64_t * RESTRICT array, unsigned int len,
    uint64_t * RESTRICT spare)
{
    constexpr unsigned int B = 1 << S2;
    constexpr unsigned int shift = BIN_SHIFT - S1 - S2;
    unsigned int counts[B] = {0};
    unsigned int offsets[B + 1];
    unsigned int next_free[B];

    for (unsigned int i = 0; i < len; ++i) {
        ++counts[(array[i] >> shift) & (B - 1)];
    }

    offsets[0] = next_free[0] = 0;
    for (unsigned int i = 0; i < B - 1; ++i) {
        offsets[i + 1] = next_free[i + 1] = offsets[i] + counts[i];
    }
    offsets[B] = len;

    for (unsigned int i = 0; i < len; ++i) {
        const unsigned int bin = (array[i] >> shift) & (B - 1);
        spare[next_free[bin]] = array[i];
        ++next_free[bin];
    }

    memcpy(array, spare, len * sizeof(*array));

    for (unsigned int b = 0; b < B; ++b) {
        const unsigned int bin = b,
            count = counts[bin],
            offset = offsets[bin];
        if (count <= 16) {
            jmp_table[count](array + offset);
        } else {
            sort(array + offset, array + offset + count);
        }
    }
}

template <unsigned int S1, unsigned int S2>
static void
american_flag_sort_2a(uint64_t *array, unsigned int len)
{
    constexpr unsigned int B = 1 << S1;
    constexpr unsigned int shift = BIN_SHIFT - S1;
    unsigned int counts[B] = {0};
    unsigned int offsets[B + 1];
    unsigned int next_free[B];

    for (unsigned int i = 0; i < len; ++i) {
        ++counts[(array[i] >> shift) & (B - 1)];
    }
    offsets[0] = next_free[0] = 0;
    for (unsigned int i = 0; i < B - 1; ++i) {
        offsets[i + 1] = next_free[i + 1] = offsets[i] + counts[i];
    }
    offsets[B] = len;

    for (unsigned int i = 0; i < len; ++i) {
        const unsigned int bin = (array[i] >> shift) & (B - 1);
        array[len + next_free[bin]] = array[i];
        ++next_free[bin];
    }

    memcpy(array, array + len, len * sizeof(*array));

    for (unsigned int b = 0; b < B; ++b) {
        const unsigned int count = counts[b];
        if (count <= 16) {
            jmp_table[count](array + offsets[b]);
        } else {
            uint64_t *next_array = array + offsets[b];
            american_flag_sort_2b<S1, S2>(next_array, count, next_array + len);
        }
    }
}


void NOINLINE
fast_sort7(uint64_t *array, unsigned int len)
{
    unsigned int lg;

    static void (*afs_jmp_table[32])(uint64_t *, unsigned int) = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        american_flag_sort_1<4>,
        american_flag_sort_1<4>,
        american_flag_sort_1<4>,
        american_flag_sort_1<5>,
        american_flag_sort_1<6>,
        american_flag_sort_1<7>,
        american_flag_sort_1<8>,
        american_flag_sort_1<9>,
        american_flag_sort_1<10>,
        american_flag_sort_1<11>,
        american_flag_sort_2a<8, 4>,
        american_flag_sort_2a<9, 4>,
        american_flag_sort_2a<10, 4>,
        american_flag_sort_2a<11, 4>,
        american_flag_sort_2a<12, 4>,
        american_flag_sort_2a<12, 5>,
        american_flag_sort_2a<12, 6>,
        american_flag_sort_2a<12, 7>,
        american_flag_sort_2a<12, 8>,
        american_flag_sort_2a<12, 9>,
        american_flag_sort_2a<12, 10>,
        american_flag_sort_2a<12, 11>,
        american_flag_sort_2a<12, 12>,
        american_flag_sort_2a<12, 12>,
        american_flag_sort_2a<12, 12>,
        american_flag_sort_2a<12, 12>,
        american_flag_sort_2a<12, 12>
    };

    if (len <= 16) {
        jmp_table[len](array);
        return;
    }

    lg = 32 - __builtin_clz(len - 1);

    afs_jmp_table[lg](array, len);
}
