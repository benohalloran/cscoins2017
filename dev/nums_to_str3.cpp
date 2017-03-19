#include <cstdint>
#include <cstring>

#define RESTRICT __restrict__
#define NOINLINE __attribute__((noinline))

static char decimals[10000][5];

#define FILL_NEXT(j) do { \
    const unsigned int k = x % 10000; \
    x /= 10000; \
    memcpy(p, decimals[k], 4); \
    p += decimals[k][4]; \
} while (0)

unsigned int NOINLINE
nums_to_str3(const uint64_t * RESTRICT nums, unsigned int n,
    char * RESTRICT buf)
{
    char *p = buf;
    for (unsigned int i = 0; i < n; ++i) {
        uint64_t x = nums[i];
        if (x >= 10000000000000000lu) {
            FILL_NEXT(0);
            FILL_NEXT(1);
            FILL_NEXT(2);
            FILL_NEXT(3);
            FILL_NEXT(4);
        } else if (x >= 1000000000000lu) {
            FILL_NEXT(0);
            FILL_NEXT(1);
            FILL_NEXT(2);
            FILL_NEXT(3);
        } else if (x >= 100000000lu) {
            FILL_NEXT(0);
            FILL_NEXT(1);
            FILL_NEXT(2);
        } else if (x >= 10000lu) {
            FILL_NEXT(0);
            FILL_NEXT(1);
        } else {
            FILL_NEXT(0);
        }
    }

    return p - buf;
}

void
init_decimals3()
{
    for (int i = 0; i < 10000; ++i) {
        if (i >= 1000) {
            decimals[i][4] = 4;
        } else if (i >= 100) {
            decimals[i][4] = 3;
        } else if (i >= 10) {
            decimals[i][4] = 2;
        } else {
            decimals[i][4] = 1;
        }

        int n = i;
        decimals[i][3] = n % 10 + '0';
        n /= 10;
        decimals[i][2] = n % 10 + '0';
        n /= 10;
        decimals[i][1] = n % 10 + '0';
        n /= 10;
        decimals[i][0] = n + '0';
    }
}
