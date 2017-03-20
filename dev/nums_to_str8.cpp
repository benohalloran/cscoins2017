#include <cstdint>
#include <cstring>

#define RESTRICT __restrict__
#define NOINLINE __attribute__((noinline))

static char decimals[1000][4];

#define FILL_NEXT do { \
    const unsigned int k = x % 1000; \
    x /= 1000; \
    memcpy(p, decimals[k], 3); \
    p += decimals[k][3]; \
} while (0)

#define FILL_LAST do { \
    memcpy(p, decimals[x], 3); \
    p += decimals[x][3]; \
} while (0)

unsigned int
num_to_str8(uint64_t x, char *buf)
{
    char *p = buf;
    if (x >= 1000000000000000000lu) {
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_LAST;
    } else if (x >= 1000000000000000lu) {
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_LAST;
    } else if (x >= 1000000000000lu) {
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_LAST;
    } else if (x >= 1000000000lu) {
        FILL_NEXT;
        FILL_NEXT;
        FILL_NEXT;
        FILL_LAST;
    } else if (x >= 1000000lu) {
        FILL_NEXT;
        FILL_NEXT;
        FILL_LAST;
    } else if (x >= 1000lu) {
        FILL_NEXT;
        FILL_LAST;
    } else {
        FILL_LAST;
    }
    return p - buf;
}

unsigned int NOINLINE
nums_to_str8(const uint64_t * RESTRICT nums, unsigned int n,
    char * RESTRICT buf)
{
    char *p = buf;
    for (unsigned int i = 0; i < n; ++i) {
        p += num_to_str8(nums[i], p);
    }
    *p = '\0';
    return p - buf;
}

void
init_decimals8()
{
    for (int i = 0; i < 1000; ++i) {
        if (i >= 100) {
            decimals[i][3] = 3;
        } else if (i >= 10) {
            decimals[i][3] = 2;
        } else {
            decimals[i][3] = 1;
        }

        int n = i;
        decimals[i][2] = n % 10 + '0';
        n /= 10;
        decimals[i][1] = n % 10 + '0';
        n /= 10;
        decimals[i][0] = n + '0';
    }
}
