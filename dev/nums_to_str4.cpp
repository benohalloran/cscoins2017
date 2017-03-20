#include <cstdint>
#include <cstring>

#define RESTRICT __restrict__
#define NOINLINE __attribute__((noinline))

static char decimals[100001][6];

#define FILL1 do { \
    len = decimals[x][5]; \
    if (len > 5) { __builtin_unreachable(); } \
    memcpy(buf, &decimals[x][5 - len], 5); \
} while (0)

#define FILL2 do { \
    const unsigned int k = x % 100000; \
    x /= 100000; \
    FILL1; \
    memcpy(buf + len, decimals[k], 5); \
} while (0)

#define FILL3 do { \
    const unsigned int k = x % 100000; \
    x /= 100000; \
    FILL2; \
    memcpy(buf + len + 5, decimals[k], 5); \
} while (0)

#define FILL4 do { \
    const unsigned int k = x % 100000; \
    x /= 100000; \
    FILL3; \
    memcpy(buf + len + 10, decimals[k], 5); \
} while (0)

unsigned int
num_to_str4(uint64_t x, char *buf)
{
    unsigned int len;
    if (x >= 1000000000000000lu) {
        FILL4;
        len += 15;
    } else if (x >= 10000000000lu) {
        FILL3;
        len += 10;
    } else if (x >= 100000lu) {
        FILL2;
        len += 5;
    } else {
        FILL1;
    }
    return len;
}

unsigned int NOINLINE
nums_to_str4(const uint64_t * RESTRICT nums, unsigned int n,
    char * RESTRICT buf)
{
    char *p = buf;
    for (unsigned int i = 0; i < n; ++i) {
        p += num_to_str4(nums[i], p);
    }
    *p = '\0';
    return p - buf;
}

void
init_decimals4()
{
    for (int i = 0; i < 100000; ++i) {
        if (i >= 10000) {
            decimals[i][5] = 5;
        } else if (i >= 1000) {
            decimals[i][5] = 4;
        } else if (i >= 100) {
            decimals[i][5] = 3;
        } else if (i >= 10) {
            decimals[i][5] = 2;
        } else {
            decimals[i][5] = 1;
        }

        int n = i;
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
    memset(decimals[100000], 0, 6);
}
