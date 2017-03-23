#include <algorithm>
#include <cstdint>
#include <functional>

#define NOINLINE __attribute__((noinline))

using namespace std;

#define SWAP(a,b) { \
    uint64_t tmp; \
    asm("mov %0, %2" \
        "\n\tcmp %1, %0" \
        "\n\tcmova %1, %0" \
        "\n\tcmova %2, %1" \
        : "+r" (array[a]), "+r" (array[b]), "=r" (tmp) \
        : : "cc"); \
}

#define SWAP_R(a,b) { \
    uint64_t tmp; \
    asm("mov %0, %2" \
        "\n\tcmp %1, %0" \
        "\n\tcmovb %1, %0" \
        "\n\tcmovb %2, %1" \
        : "+r" (array[a]), "+r" (array[b]), "=r" (tmp) \
        : : "cc"); \
}

static void
sort16(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(12, 13); SWAP(14, 15); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(12, 14); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(13, 15); SWAP(0, 4); SWAP(8, 12); SWAP(1, 5); SWAP(9, 13); SWAP(2, 6); SWAP(10, 14); SWAP(3, 7); SWAP(11, 15); SWAP(0, 8); SWAP(1, 9); SWAP(2, 10); SWAP(3, 11); SWAP(4, 12); SWAP(5, 13); SWAP(6, 14); SWAP(7, 15); SWAP(5, 10); SWAP(6, 9); SWAP(3, 12); SWAP(13, 14); SWAP(7, 11); SWAP(1, 2); SWAP(4, 8); SWAP(1, 4); SWAP(7, 13); SWAP(2, 8); SWAP(11, 14); SWAP(2, 4); SWAP(5, 6); SWAP(9, 10); SWAP(11, 13); SWAP(3, 8); SWAP(7, 12); SWAP(6, 8); SWAP(10, 12); SWAP(3, 5); SWAP(7, 9); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8); SWAP(9, 10); SWAP(11, 12); SWAP(6, 7); SWAP(8, 9);
}

static void
sort15(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(12, 13); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(12, 14); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(0, 4); SWAP(8, 12); SWAP(1, 5); SWAP(9, 13); SWAP(2, 6); SWAP(10, 14); SWAP(3, 7); SWAP(0, 8); SWAP(1, 9); SWAP(2, 10); SWAP(3, 11); SWAP(4, 12); SWAP(5, 13); SWAP(6, 14); SWAP(5, 10); SWAP(6, 9); SWAP(3, 12); SWAP(13, 14); SWAP(7, 11); SWAP(1, 2); SWAP(4, 8); SWAP(1, 4); SWAP(7, 13); SWAP(2, 8); SWAP(11, 14); SWAP(2, 4); SWAP(5, 6); SWAP(9, 10); SWAP(11, 13); SWAP(3, 8); SWAP(7, 12); SWAP(6, 8); SWAP(10, 12); SWAP(3, 5); SWAP(7, 9); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8); SWAP(9, 10); SWAP(11, 12); SWAP(6, 7); SWAP(8, 9);
}

static void
sort14(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(12, 13); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(0, 4); SWAP(8, 12); SWAP(1, 5); SWAP(9, 13); SWAP(2, 6); SWAP(3, 7); SWAP(0, 8); SWAP(1, 9); SWAP(2, 10); SWAP(3, 11); SWAP(4, 12); SWAP(5, 13); SWAP(5, 10); SWAP(6, 9); SWAP(3, 12); SWAP(7, 11); SWAP(1, 2); SWAP(4, 8); SWAP(1, 4); SWAP(7, 13); SWAP(2, 8); SWAP(2, 4); SWAP(5, 6); SWAP(9, 10); SWAP(11, 13); SWAP(3, 8); SWAP(7, 12); SWAP(6, 8); SWAP(10, 12); SWAP(3, 5); SWAP(7, 9); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8); SWAP(9, 10); SWAP(11, 12); SWAP(6, 7); SWAP(8, 9);
}

static void
sort13(uint64_t *array)
{
    SWAP(1, 7); SWAP(9, 11); SWAP(3, 4); SWAP(5, 8); SWAP(0, 12); SWAP(2, 6); SWAP(0, 1); SWAP(2, 3); SWAP(4, 6); SWAP(8, 11); SWAP(7, 12); SWAP(5, 9); SWAP(0, 2); SWAP(3, 7); SWAP(10, 11); SWAP(1, 4); SWAP(6, 12); SWAP(7, 8); SWAP(11, 12); SWAP(4, 9); SWAP(6, 10); SWAP(3, 4); SWAP(5, 6); SWAP(8, 9); SWAP(10, 11); SWAP(1, 7); SWAP(2, 6); SWAP(9, 11); SWAP(1, 3); SWAP(4, 7); SWAP(8, 10); SWAP(0, 5); SWAP(2, 5); SWAP(6, 8); SWAP(9, 10); SWAP(1, 2); SWAP(3, 5); SWAP(7, 8); SWAP(4, 6); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(3, 4); SWAP(5, 6);
}

static void
sort12(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(1, 2); SWAP(5, 6); SWAP(9, 10); SWAP(1, 5); SWAP(6, 10); SWAP(5, 9); SWAP(2, 6); SWAP(1, 5); SWAP(6, 10); SWAP(0, 4); SWAP(7, 11); SWAP(3, 7); SWAP(4, 8); SWAP(0, 4); SWAP(7, 11); SWAP(1, 4); SWAP(7, 10); SWAP(3, 8); SWAP(2, 3); SWAP(8, 9); SWAP(2, 4); SWAP(7, 9); SWAP(3, 5); SWAP(6, 8); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8);
}

static void
sort11(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(1, 3); SWAP(5, 7); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(1, 2); SWAP(5, 6); SWAP(9, 10); SWAP(1, 5); SWAP(6, 10); SWAP(5, 9); SWAP(2, 6); SWAP(1, 5); SWAP(6, 10); SWAP(0, 4); SWAP(3, 7); SWAP(4, 8); SWAP(0, 4); SWAP(1, 4); SWAP(7, 10); SWAP(3, 8); SWAP(2, 3); SWAP(8, 9); SWAP(2, 4); SWAP(7, 9); SWAP(3, 5); SWAP(6, 8); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8);
}

static void
sort10(uint64_t *array)
{
    SWAP(4, 9); SWAP(3, 8); SWAP(2, 7); SWAP(1, 6); SWAP(0, 5); SWAP(1, 4); SWAP(6, 9); SWAP(0, 3); SWAP(5, 8); SWAP(0, 2); SWAP(3, 6); SWAP(7, 9); SWAP(0, 1); SWAP(2, 4); SWAP(5, 7); SWAP(8, 9); SWAP(1, 2); SWAP(4, 6); SWAP(7, 8); SWAP(3, 5); SWAP(2, 5); SWAP(6, 8); SWAP(1, 3); SWAP(4, 7); SWAP(2, 3); SWAP(6, 7); SWAP(3, 4); SWAP(5, 6); SWAP(4, 5);
}

static void
sort9(uint64_t *array)
{
    SWAP(0, 1); SWAP(3, 4); SWAP(6, 7); SWAP(1, 2); SWAP(4, 5); SWAP(7, 8); SWAP(0, 1); SWAP(3, 4); SWAP(6, 7); SWAP(0, 3); SWAP(3, 6); SWAP(0, 3); SWAP(1, 4); SWAP(4, 7); SWAP(1, 4); SWAP(2, 5); SWAP(5, 8); SWAP(2, 5); SWAP(1, 3); SWAP(5, 7); SWAP(2, 6); SWAP(4, 6); SWAP(2, 4); SWAP(2, 3); SWAP(5, 6);
}

static void
sort8(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(0, 2); SWAP(1, 3); SWAP(1, 2); SWAP(4, 5); SWAP(6, 7); SWAP(4, 6); SWAP(5, 7); SWAP(5, 6); SWAP(0, 4); SWAP(1, 5); SWAP(1, 4); SWAP(2, 6); SWAP(3, 7); SWAP(3, 6); SWAP(2, 4); SWAP(3, 5); SWAP(3, 4);
}

static void
sort7(uint64_t *array)
{
    SWAP(1, 2); SWAP(0, 2); SWAP(0, 1); SWAP(3, 4); SWAP(5, 6); SWAP(3, 5); SWAP(4, 6); SWAP(4, 5); SWAP(0, 4); SWAP(0, 3); SWAP(1, 5); SWAP(2, 6); SWAP(2, 5); SWAP(1, 3); SWAP(2, 4); SWAP(2, 3);
}

static void
sort6(uint64_t *array)
{
    SWAP(1, 2); SWAP(0, 2); SWAP(0, 1); SWAP(4, 5); SWAP(3, 5); SWAP(3, 4); SWAP(0, 3); SWAP(1, 4); SWAP(2, 5); SWAP(2, 4); SWAP(1, 3); SWAP(2, 3);
}

static void
sort5(uint64_t *array)
{
    SWAP(0, 1); SWAP(3, 4); SWAP(2, 4); SWAP(2, 3); SWAP(0, 3); SWAP(0, 2); SWAP(1, 4); SWAP(1, 3); SWAP(1, 2);
}

static void
sort4(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(0, 2); SWAP(1, 3); SWAP(1, 2);
}

static void
sort3(uint64_t *array)
{
    SWAP(1, 2); SWAP(0, 2); SWAP(0, 1);
}

static void
sort2(uint64_t *array)
{
    SWAP(0, 1);
}

static void
sort16_r(uint64_t *array)
{
    SWAP_R(0, 1); SWAP_R(2, 3); SWAP_R(4, 5); SWAP_R(6, 7); SWAP_R(8, 9); SWAP_R(10, 11); SWAP_R(12, 13); SWAP_R(14, 15); SWAP_R(0, 2); SWAP_R(4, 6); SWAP_R(8, 10); SWAP_R(12, 14); SWAP_R(1, 3); SWAP_R(5, 7); SWAP_R(9, 11); SWAP_R(13, 15); SWAP_R(0, 4); SWAP_R(8, 12); SWAP_R(1, 5); SWAP_R(9, 13); SWAP_R(2, 6); SWAP_R(10, 14); SWAP_R(3, 7); SWAP_R(11, 15); SWAP_R(0, 8); SWAP_R(1, 9); SWAP_R(2, 10); SWAP_R(3, 11); SWAP_R(4, 12); SWAP_R(5, 13); SWAP_R(6, 14); SWAP_R(7, 15); SWAP_R(5, 10); SWAP_R(6, 9); SWAP_R(3, 12); SWAP_R(13, 14); SWAP_R(7, 11); SWAP_R(1, 2); SWAP_R(4, 8); SWAP_R(1, 4); SWAP_R(7, 13); SWAP_R(2, 8); SWAP_R(11, 14); SWAP_R(2, 4); SWAP_R(5, 6); SWAP_R(9, 10); SWAP_R(11, 13); SWAP_R(3, 8); SWAP_R(7, 12); SWAP_R(6, 8); SWAP_R(10, 12); SWAP_R(3, 5); SWAP_R(7, 9); SWAP_R(3, 4); SWAP_R(5, 6); SWAP_R(7, 8); SWAP_R(9, 10); SWAP_R(11, 12); SWAP_R(6, 7); SWAP_R(8, 9);
}

static void
sort15_r(uint64_t *array)
{
    SWAP_R(0, 1); SWAP_R(2, 3); SWAP_R(4, 5); SWAP_R(6, 7); SWAP_R(8, 9); SWAP_R(10, 11); SWAP_R(12, 13); SWAP_R(0, 2); SWAP_R(4, 6); SWAP_R(8, 10); SWAP_R(12, 14); SWAP_R(1, 3); SWAP_R(5, 7); SWAP_R(9, 11); SWAP_R(0, 4); SWAP_R(8, 12); SWAP_R(1, 5); SWAP_R(9, 13); SWAP_R(2, 6); SWAP_R(10, 14); SWAP_R(3, 7); SWAP_R(0, 8); SWAP_R(1, 9); SWAP_R(2, 10); SWAP_R(3, 11); SWAP_R(4, 12); SWAP_R(5, 13); SWAP_R(6, 14); SWAP_R(5, 10); SWAP_R(6, 9); SWAP_R(3, 12); SWAP_R(13, 14); SWAP_R(7, 11); SWAP_R(1, 2); SWAP_R(4, 8); SWAP_R(1, 4); SWAP_R(7, 13); SWAP_R(2, 8); SWAP_R(11, 14); SWAP_R(2, 4); SWAP_R(5, 6); SWAP_R(9, 10); SWAP_R(11, 13); SWAP_R(3, 8); SWAP_R(7, 12); SWAP_R(6, 8); SWAP_R(10, 12); SWAP_R(3, 5); SWAP_R(7, 9); SWAP_R(3, 4); SWAP_R(5, 6); SWAP_R(7, 8); SWAP_R(9, 10); SWAP_R(11, 12); SWAP_R(6, 7); SWAP_R(8, 9);
}

static void
sort14_r(uint64_t *array)
{
    SWAP_R(0, 1); SWAP_R(2, 3); SWAP_R(4, 5); SWAP_R(6, 7); SWAP_R(8, 9); SWAP_R(10, 11); SWAP_R(12, 13); SWAP_R(0, 2); SWAP_R(4, 6); SWAP_R(8, 10); SWAP_R(1, 3); SWAP_R(5, 7); SWAP_R(9, 11); SWAP_R(0, 4); SWAP_R(8, 12); SWAP_R(1, 5); SWAP_R(9, 13); SWAP_R(2, 6); SWAP_R(3, 7); SWAP_R(0, 8); SWAP_R(1, 9); SWAP_R(2, 10); SWAP_R(3, 11); SWAP_R(4, 12); SWAP_R(5, 13); SWAP_R(5, 10); SWAP_R(6, 9); SWAP_R(3, 12); SWAP_R(7, 11); SWAP_R(1, 2); SWAP_R(4, 8); SWAP_R(1, 4); SWAP_R(7, 13); SWAP_R(2, 8); SWAP_R(2, 4); SWAP_R(5, 6); SWAP_R(9, 10); SWAP_R(11, 13); SWAP_R(3, 8); SWAP_R(7, 12); SWAP_R(6, 8); SWAP_R(10, 12); SWAP_R(3, 5); SWAP_R(7, 9); SWAP_R(3, 4); SWAP_R(5, 6); SWAP_R(7, 8); SWAP_R(9, 10); SWAP_R(11, 12); SWAP_R(6, 7); SWAP_R(8, 9);
}

static void
sort13_r(uint64_t *array)
{
    SWAP_R(1, 7); SWAP_R(9, 11); SWAP_R(3, 4); SWAP_R(5, 8); SWAP_R(0, 12); SWAP_R(2, 6); SWAP_R(0, 1); SWAP_R(2, 3); SWAP_R(4, 6); SWAP_R(8, 11); SWAP_R(7, 12); SWAP_R(5, 9); SWAP_R(0, 2); SWAP_R(3, 7); SWAP_R(10, 11); SWAP_R(1, 4); SWAP_R(6, 12); SWAP_R(7, 8); SWAP_R(11, 12); SWAP_R(4, 9); SWAP_R(6, 10); SWAP_R(3, 4); SWAP_R(5, 6); SWAP_R(8, 9); SWAP_R(10, 11); SWAP_R(1, 7); SWAP_R(2, 6); SWAP_R(9, 11); SWAP_R(1, 3); SWAP_R(4, 7); SWAP_R(8, 10); SWAP_R(0, 5); SWAP_R(2, 5); SWAP_R(6, 8); SWAP_R(9, 10); SWAP_R(1, 2); SWAP_R(3, 5); SWAP_R(7, 8); SWAP_R(4, 6); SWAP_R(2, 3); SWAP_R(4, 5); SWAP_R(6, 7); SWAP_R(8, 9); SWAP_R(3, 4); SWAP_R(5, 6);
}

static void
sort12_r(uint64_t *array)
{
    SWAP_R(0, 1); SWAP_R(2, 3); SWAP_R(4, 5); SWAP_R(6, 7); SWAP_R(8, 9); SWAP_R(10, 11); SWAP_R(1, 3); SWAP_R(5, 7); SWAP_R(9, 11); SWAP_R(0, 2); SWAP_R(4, 6); SWAP_R(8, 10); SWAP_R(1, 2); SWAP_R(5, 6); SWAP_R(9, 10); SWAP_R(1, 5); SWAP_R(6, 10); SWAP_R(5, 9); SWAP_R(2, 6); SWAP_R(1, 5); SWAP_R(6, 10); SWAP_R(0, 4); SWAP_R(7, 11); SWAP_R(3, 7); SWAP_R(4, 8); SWAP_R(0, 4); SWAP_R(7, 11); SWAP_R(1, 4); SWAP_R(7, 10); SWAP_R(3, 8); SWAP_R(2, 3); SWAP_R(8, 9); SWAP_R(2, 4); SWAP_R(7, 9); SWAP_R(3, 5); SWAP_R(6, 8); SWAP_R(3, 4); SWAP_R(5, 6); SWAP_R(7, 8);
}

static void
sort11_r(uint64_t *array)
{
    SWAP_R(0, 1); SWAP_R(2, 3); SWAP_R(4, 5); SWAP_R(6, 7); SWAP_R(8, 9); SWAP_R(1, 3); SWAP_R(5, 7); SWAP_R(0, 2); SWAP_R(4, 6); SWAP_R(8, 10); SWAP_R(1, 2); SWAP_R(5, 6); SWAP_R(9, 10); SWAP_R(1, 5); SWAP_R(6, 10); SWAP_R(5, 9); SWAP_R(2, 6); SWAP_R(1, 5); SWAP_R(6, 10); SWAP_R(0, 4); SWAP_R(3, 7); SWAP_R(4, 8); SWAP_R(0, 4); SWAP_R(1, 4); SWAP_R(7, 10); SWAP_R(3, 8); SWAP_R(2, 3); SWAP_R(8, 9); SWAP_R(2, 4); SWAP_R(7, 9); SWAP_R(3, 5); SWAP_R(6, 8); SWAP_R(3, 4); SWAP_R(5, 6); SWAP_R(7, 8);
}

static void
sort10_r(uint64_t *array)
{
    SWAP_R(4, 9); SWAP_R(3, 8); SWAP_R(2, 7); SWAP_R(1, 6); SWAP_R(0, 5); SWAP_R(1, 4); SWAP_R(6, 9); SWAP_R(0, 3); SWAP_R(5, 8); SWAP_R(0, 2); SWAP_R(3, 6); SWAP_R(7, 9); SWAP_R(0, 1); SWAP_R(2, 4); SWAP_R(5, 7); SWAP_R(8, 9); SWAP_R(1, 2); SWAP_R(4, 6); SWAP_R(7, 8); SWAP_R(3, 5); SWAP_R(2, 5); SWAP_R(6, 8); SWAP_R(1, 3); SWAP_R(4, 7); SWAP_R(2, 3); SWAP_R(6, 7); SWAP_R(3, 4); SWAP_R(5, 6); SWAP_R(4, 5);
}

static void
sort9_r(uint64_t *array)
{
    SWAP_R(0, 1); SWAP_R(3, 4); SWAP_R(6, 7); SWAP_R(1, 2); SWAP_R(4, 5); SWAP_R(7, 8); SWAP_R(0, 1); SWAP_R(3, 4); SWAP_R(6, 7); SWAP_R(0, 3); SWAP_R(3, 6); SWAP_R(0, 3); SWAP_R(1, 4); SWAP_R(4, 7); SWAP_R(1, 4); SWAP_R(2, 5); SWAP_R(5, 8); SWAP_R(2, 5); SWAP_R(1, 3); SWAP_R(5, 7); SWAP_R(2, 6); SWAP_R(4, 6); SWAP_R(2, 4); SWAP_R(2, 3); SWAP_R(5, 6);
}

static void
sort8_r(uint64_t *array)
{
    SWAP_R(0, 1); SWAP_R(2, 3); SWAP_R(0, 2); SWAP_R(1, 3); SWAP_R(1, 2); SWAP_R(4, 5); SWAP_R(6, 7); SWAP_R(4, 6); SWAP_R(5, 7); SWAP_R(5, 6); SWAP_R(0, 4); SWAP_R(1, 5); SWAP_R(1, 4); SWAP_R(2, 6); SWAP_R(3, 7); SWAP_R(3, 6); SWAP_R(2, 4); SWAP_R(3, 5); SWAP_R(3, 4);
}

static void
sort7_r(uint64_t *array)
{
    SWAP_R(1, 2); SWAP_R(0, 2); SWAP_R(0, 1); SWAP_R(3, 4); SWAP_R(5, 6); SWAP_R(3, 5); SWAP_R(4, 6); SWAP_R(4, 5); SWAP_R(0, 4); SWAP_R(0, 3); SWAP_R(1, 5); SWAP_R(2, 6); SWAP_R(2, 5); SWAP_R(1, 3); SWAP_R(2, 4); SWAP_R(2, 3);
}

static void
sort6_r(uint64_t *array)
{
    SWAP_R(1, 2); SWAP_R(0, 2); SWAP_R(0, 1); SWAP_R(4, 5); SWAP_R(3, 5); SWAP_R(3, 4); SWAP_R(0, 3); SWAP_R(1, 4); SWAP_R(2, 5); SWAP_R(2, 4); SWAP_R(1, 3); SWAP_R(2, 3);
}

static void
sort5_r(uint64_t *array)
{
    SWAP_R(0, 1); SWAP_R(3, 4); SWAP_R(2, 4); SWAP_R(2, 3); SWAP_R(0, 3); SWAP_R(0, 2); SWAP_R(1, 4); SWAP_R(1, 3); SWAP_R(1, 2);
}

static void
sort4_r(uint64_t *array)
{
    SWAP_R(0, 1); SWAP_R(2, 3); SWAP_R(0, 2); SWAP_R(1, 3); SWAP_R(1, 2);
}

static void
sort3_r(uint64_t *array)
{
    SWAP_R(1, 2); SWAP_R(0, 2); SWAP_R(0, 1);
}

static void
sort2_r(uint64_t *array)
{
    SWAP_R(0, 1);
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

static void (*jmp_table_r[17])(uint64_t *) = {
    sort_empty, sort_empty, sort2_r, sort3_r, sort4_r, sort5_r, sort6_r,
    sort7_r, sort8_r, sort9_r, sort10_r, sort11_r, sort12_r, sort13_r,
    sort14_r, sort15_r, sort16_r
};

void NOINLINE
fast_sort1(uint64_t *array, unsigned int len)
{
    if (len <= 16) {
        jmp_table[len & 0x1f](array);
    } else {
        sort(array, array + len);
    }
}

void NOINLINE
fast_sort_r1(uint64_t *array, unsigned int len)
{
    if (len <= 16) {
        jmp_table_r[len & 0x1f](array);
    } else {
        sort(array, array + len, greater<uint64_t>());
    }
}
