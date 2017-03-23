#include <cstdint>
#include <cstring>

#define RESTRICT __restrict__
#define NOINLINE __attribute__((noinline))

static char decimals[10001][5];

#define FILL1 do { \
    len = decimals[x][4]; \
    if (len > 4) { __builtin_unreachable(); } \
    memcpy(buf, &decimals[x][4 - len], 4); \
} while (0)

#define FILL2 do { \
    const unsigned int k = x % 10000; \
    x /= 10000; \
    FILL1; \
    memcpy(buf + len, decimals[k], 4); \
} while (0)

#define FILL3 do { \
    const unsigned int k = x % 10000; \
    x /= 10000; \
    FILL2; \
    memcpy(buf + len + 4, decimals[k], 4); \
} while (0)

#define FILL4 do { \
    const unsigned int k = x % 10000; \
    x /= 10000; \
    FILL3; \
    memcpy(buf + len + 8, decimals[k], 4); \
} while (0)

#define FILL5 do { \
    const unsigned int k = x % 10000; \
    x /= 10000; \
    FILL4; \
    memcpy(buf + len + 12, decimals[k], 4); \
} while (0)

unsigned int
num_to_str3(uint64_t x, char *buf)
{
    unsigned int len;
    if (x >= 10000000000000000lu) {
        FILL5;
        len += 16;
    } else if (x >= 1000000000000lu) {
        FILL4;
        len += 12;
    } else if (x >= 100000000lu) {
        FILL3;
        len += 8;
    } else if (x >= 10000lu) {
        FILL2;
        len += 4;
    } else {
        FILL1;
    }
    return len;
}

unsigned int NOINLINE
nums_to_str3(const uint64_t * RESTRICT nums, unsigned int n,
    char * RESTRICT buf)
{
    char *p = buf;
    for (unsigned int i = 0; i < n; ++i) {
        p += num_to_str3(nums[i], p);
    }
    *p = '\0';
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
    memset(decimals[10000], 0, 5);
}
