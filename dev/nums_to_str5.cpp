#include <cstdint>
#include <cstring>

// NOT WORKING

#define RESTRICT __restrict__
#define NOINLINE __attribute__((noinline))

static char decimals[1000000][7];

#define FILL_NEXT(j) do { \
    const unsigned int k = x % 1000000; \
    x /= 1000000; \
    mods[m + j] = k; \
    __builtin_prefetch(&decimals[k]); \
} while (0)

#define FILL_LAST(j) do { \
    mods[m + j] = x; \
    __builtin_prefetch(&decimals[x]); \
} while (0)

unsigned int NOINLINE
nums_to_str5(const uint64_t * RESTRICT nums, unsigned int n,
    char * RESTRICT buf)
{
    unsigned int mods[n * 4];
    unsigned int m = 0;
    for (unsigned int i = 0; i < n; ++i) {
        uint64_t x = nums[i];
        if (x >= 1000000000000000000lu) {
            FILL_NEXT(3);
            FILL_NEXT(2);
            FILL_NEXT(1);
            FILL_LAST(0);
            m += 4;
        } else if (x >= 1000000000000lu) {
            FILL_NEXT(2);
            FILL_NEXT(1);
            FILL_LAST(0);
            m += 3;
        } else if (x >= 1000000lu) {
            FILL_NEXT(1);
            FILL_LAST(0);
            m += 2;
        } else {
            FILL_LAST(0);
            ++m;
        }
    }

    char *p = buf;
    for (unsigned int i = 0; i < m; ++i) {
        const char *d = decimals[mods[i]];
        memcpy(p, d, 6);
        p += d[6];
    }
    *p = '\0';
    return p - buf;
}

void
init_decimals5()
{
    for (int i = 0; i < 1000000; ++i) {
        if (i >= 100000) {
            decimals[i][6] = 6;
        } else if (i >= 10000) {
            decimals[i][6] = 5;
        } else if (i >= 1000) {
            decimals[i][6] = 4;
        } else if (i >= 100) {
            decimals[i][6] = 3;
        } else if (i >= 10) {
            decimals[i][6] = 2;
        } else {
            decimals[i][6] = 1;
        }

        int n = i;
        decimals[i][5] = n % 10 + '0';
        n /= 10;
        decimals[i][4] = n % 10 + '0';
        n /= 10;
        decimals[i][3] = n % 10 + '0';
        n /= 10;
        decimals[i][2] = n % 10 + '0';
        n /= 10;
        decimals[i][1] = n % 10 + '0';
        n /= 10;
        decimals[i][0] = n + '0';
    }
}
