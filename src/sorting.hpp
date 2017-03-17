#ifndef SOLVER_CPP_
#error "cannot be included"
#endif

// GCC doesn't use cmov with these for me, so they're slower
// #define SWAP(a,b) {
//     const uint64_t x = array[a];
//     const uint64_t y = array[b];
//     const int lt = x < y;
//     const uint64_t min = lt ? x : y;
//     const uint64_t max = lt ? y : x;
//     array[a] = min;
//     array[b] = max;
// }
// #define SWAP(a,b) {
//     if (array[b] < array[a]) {
//         std::swap(array[a], array[b]);
//     }
// }

#define SWAP(a,b) { \
    uint64_t tmp; \
    asm("mov %0, %2" \
        "\n\tcmp %1, %0" \
        "\n\tcmovg %1, %0" \
        "\n\tcmovg %2, %1" \
        : "+r" (array[a]), "+r" (array[b]), "=r" (tmp) \
        : : "cc"); \
}

void sort16(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(12, 13); SWAP(14, 15); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(12, 14); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(13, 15); SWAP(0, 4); SWAP(8, 12); SWAP(1, 5); SWAP(9, 13); SWAP(2, 6); SWAP(10, 14); SWAP(3, 7); SWAP(11, 15); SWAP(0, 8); SWAP(1, 9); SWAP(2, 10); SWAP(3, 11); SWAP(4, 12); SWAP(5, 13); SWAP(6, 14); SWAP(7, 15); SWAP(5, 10); SWAP(6, 9); SWAP(3, 12); SWAP(13, 14); SWAP(7, 11); SWAP(1, 2); SWAP(4, 8); SWAP(1, 4); SWAP(7, 13); SWAP(2, 8); SWAP(11, 14); SWAP(2, 4); SWAP(5, 6); SWAP(9, 10); SWAP(11, 13); SWAP(3, 8); SWAP(7, 12); SWAP(6, 8); SWAP(10, 12); SWAP(3, 5); SWAP(7, 9); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8); SWAP(9, 10); SWAP(11, 12); SWAP(6, 7); SWAP(8, 9);
}

void sort15(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(12, 13); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(12, 14); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(0, 4); SWAP(8, 12); SWAP(1, 5); SWAP(9, 13); SWAP(2, 6); SWAP(10, 14); SWAP(3, 7); SWAP(0, 8); SWAP(1, 9); SWAP(2, 10); SWAP(3, 11); SWAP(4, 12); SWAP(5, 13); SWAP(6, 14); SWAP(5, 10); SWAP(6, 9); SWAP(3, 12); SWAP(13, 14); SWAP(7, 11); SWAP(1, 2); SWAP(4, 8); SWAP(1, 4); SWAP(7, 13); SWAP(2, 8); SWAP(11, 14); SWAP(2, 4); SWAP(5, 6); SWAP(9, 10); SWAP(11, 13); SWAP(3, 8); SWAP(7, 12); SWAP(6, 8); SWAP(10, 12); SWAP(3, 5); SWAP(7, 9); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8); SWAP(9, 10); SWAP(11, 12); SWAP(6, 7); SWAP(8, 9);
}

void sort14(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(12, 13); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(0, 4); SWAP(8, 12); SWAP(1, 5); SWAP(9, 13); SWAP(2, 6); SWAP(3, 7); SWAP(0, 8); SWAP(1, 9); SWAP(2, 10); SWAP(3, 11); SWAP(4, 12); SWAP(5, 13); SWAP(5, 10); SWAP(6, 9); SWAP(3, 12); SWAP(7, 11); SWAP(1, 2); SWAP(4, 8); SWAP(1, 4); SWAP(7, 13); SWAP(2, 8); SWAP(2, 4); SWAP(5, 6); SWAP(9, 10); SWAP(11, 13); SWAP(3, 8); SWAP(7, 12); SWAP(6, 8); SWAP(10, 12); SWAP(3, 5); SWAP(7, 9); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8); SWAP(9, 10); SWAP(11, 12); SWAP(6, 7); SWAP(8, 9);
}

void sort13(uint64_t *array)
{
    SWAP(1, 7); SWAP(9, 11); SWAP(3, 4); SWAP(5, 8); SWAP(0, 12); SWAP(2, 6); SWAP(0, 1); SWAP(2, 3); SWAP(4, 6); SWAP(8, 11); SWAP(7, 12); SWAP(5, 9); SWAP(0, 2); SWAP(3, 7); SWAP(10, 11); SWAP(1, 4); SWAP(6, 12); SWAP(7, 8); SWAP(11, 12); SWAP(4, 9); SWAP(6, 10); SWAP(3, 4); SWAP(5, 6); SWAP(8, 9); SWAP(10, 11); SWAP(1, 7); SWAP(2, 6); SWAP(9, 11); SWAP(1, 3); SWAP(4, 7); SWAP(8, 10); SWAP(0, 5); SWAP(2, 5); SWAP(6, 8); SWAP(9, 10); SWAP(1, 2); SWAP(3, 5); SWAP(7, 8); SWAP(4, 6); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(3, 4); SWAP(5, 6);
}

void sort12(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(10, 11); SWAP(1, 3); SWAP(5, 7); SWAP(9, 11); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(1, 2); SWAP(5, 6); SWAP(9, 10); SWAP(1, 5); SWAP(6, 10); SWAP(5, 9); SWAP(2, 6); SWAP(1, 5); SWAP(6, 10); SWAP(0, 4); SWAP(7, 11); SWAP(3, 7); SWAP(4, 8); SWAP(0, 4); SWAP(7, 11); SWAP(1, 4); SWAP(7, 10); SWAP(3, 8); SWAP(2, 3); SWAP(8, 9); SWAP(2, 4); SWAP(7, 9); SWAP(3, 5); SWAP(6, 8); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8);
}

void sort11(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7); SWAP(8, 9); SWAP(1, 3); SWAP(5, 7); SWAP(0, 2); SWAP(4, 6); SWAP(8, 10); SWAP(1, 2); SWAP(5, 6); SWAP(9, 10); SWAP(1, 5); SWAP(6, 10); SWAP(5, 9); SWAP(2, 6); SWAP(1, 5); SWAP(6, 10); SWAP(0, 4); SWAP(3, 7); SWAP(4, 8); SWAP(0, 4); SWAP(1, 4); SWAP(7, 10); SWAP(3, 8); SWAP(2, 3); SWAP(8, 9); SWAP(2, 4); SWAP(7, 9); SWAP(3, 5); SWAP(6, 8); SWAP(3, 4); SWAP(5, 6); SWAP(7, 8);
}

void sort10(uint64_t *array)
{
    SWAP(4, 9); SWAP(3, 8); SWAP(2, 7); SWAP(1, 6); SWAP(0, 5); SWAP(1, 4); SWAP(6, 9); SWAP(0, 3); SWAP(5, 8); SWAP(0, 2); SWAP(3, 6); SWAP(7, 9); SWAP(0, 1); SWAP(2, 4); SWAP(5, 7); SWAP(8, 9); SWAP(1, 2); SWAP(4, 6); SWAP(7, 8); SWAP(3, 5); SWAP(2, 5); SWAP(6, 8); SWAP(1, 3); SWAP(4, 7); SWAP(2, 3); SWAP(6, 7); SWAP(3, 4); SWAP(5, 6); SWAP(4, 5);
}

void sort9(uint64_t *array)
{
    SWAP(0, 1); SWAP(3, 4); SWAP(6, 7); SWAP(1, 2); SWAP(4, 5); SWAP(7, 8); SWAP(0, 1); SWAP(3, 4); SWAP(6, 7); SWAP(0, 3); SWAP(3, 6); SWAP(0, 3); SWAP(1, 4); SWAP(4, 7); SWAP(1, 4); SWAP(2, 5); SWAP(5, 8); SWAP(2, 5); SWAP(1, 3); SWAP(5, 7); SWAP(2, 6); SWAP(4, 6); SWAP(2, 4); SWAP(2, 3); SWAP(5, 6);
}

void sort8(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(0, 2); SWAP(1, 3); SWAP(1, 2); SWAP(4, 5); SWAP(6, 7); SWAP(4, 6); SWAP(5, 7); SWAP(5, 6); SWAP(0, 4); SWAP(1, 5); SWAP(1, 4); SWAP(2, 6); SWAP(3, 7); SWAP(3, 6); SWAP(2, 4); SWAP(3, 5); SWAP(3, 4);
}

void sort7(uint64_t *array)
{
    SWAP(1, 2); SWAP(0, 2); SWAP(0, 1); SWAP(3, 4); SWAP(5, 6); SWAP(3, 5); SWAP(4, 6); SWAP(4, 5); SWAP(0, 4); SWAP(0, 3); SWAP(1, 5); SWAP(2, 6); SWAP(2, 5); SWAP(1, 3); SWAP(2, 4); SWAP(2, 3);
}

void sort6(uint64_t *array)
{
    SWAP(1, 2); SWAP(0, 2); SWAP(0, 1); SWAP(4, 5); SWAP(3, 5); SWAP(3, 4); SWAP(0, 3); SWAP(1, 4); SWAP(2, 5); SWAP(2, 4); SWAP(1, 3); SWAP(2, 3);
}

void sort5(uint64_t *array)
{
    SWAP(0, 1); SWAP(3, 4); SWAP(2, 4); SWAP(2, 3); SWAP(0, 3); SWAP(0, 2); SWAP(1, 4); SWAP(1, 3); SWAP(1, 2);
}

void sort4(uint64_t *array)
{
    SWAP(0, 1); SWAP(2, 3); SWAP(0, 2); SWAP(1, 3); SWAP(1, 2);
}

void sort3(uint64_t *array)
{
    SWAP(1, 2); SWAP(0, 2); SWAP(0, 1);
}

void sort2(uint64_t *array)
{
    SWAP(0, 1);
}

void sort_empty(uint64_t *array)
{
    (void)array;
}

void (*jmp_table[17])(uint64_t *) = {
    sort_empty, sort_empty, sort2, sort3, sort4, sort5, sort6, sort7, sort8,
    sort9, sort10, sort11, sort12, sort13, sort14, sort15, sort16
};

inline void fast_sort(uint64_t *array, unsigned int len)
{
    if (len <= 1) {
        return;
    }

    if (len <= 16) {
        jmp_table[len & 0x1f](array);
    } else {
        sort(array, array + len);
    }
}
