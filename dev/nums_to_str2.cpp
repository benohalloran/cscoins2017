#include <cstdint>
#include <cstring>

#define RESTRICT __restrict__
#define NOINLINE __attribute__((noinline))

static char decimals[1000][3];

#define FILL_NEXT(n, buf, len) do { \
    const uint64_t m = n % 1000; \
    n /= 1000; \
    memcpy(&buf[len - 3], decimals[m], 3); \
} while (0)

#define FILL3(n, buf) do { \
    memcpy(buf, &decimals[n][0], 3); \
} while (0)

#define FILL2(n, buf) do { \
    memcpy(buf, &decimals[n][1], 2); \
} while (0)

#define FILL1(n, buf) buf[0] = n + '0'

unsigned int
num_to_str2(uint64_t n, char *buf)
{
    if (n >= 10000000000000000000lu) {
        FILL_NEXT(n, buf, 20);
        FILL_NEXT(n, buf, 17);
        FILL_NEXT(n, buf, 14);
        FILL_NEXT(n, buf, 11);
        FILL_NEXT(n, buf, 8);
        FILL_NEXT(n, buf, 5);
        FILL2(n, buf);
        return 20;
    }

    if (n >= 1000000000000000000lu) {
        FILL_NEXT(n, buf, 19);
        FILL_NEXT(n, buf, 16);
        FILL_NEXT(n, buf, 13);
        FILL_NEXT(n, buf, 10);
        FILL_NEXT(n, buf, 7);
        FILL_NEXT(n, buf, 4);
        FILL1(n, buf);
        return 19;
    }

    if (n >= 100000000000000000lu) {
        FILL_NEXT(n, buf, 18);
        FILL_NEXT(n, buf, 15);
        FILL_NEXT(n, buf, 12);
        FILL_NEXT(n, buf, 9);
        FILL_NEXT(n, buf, 6);
        FILL3(n, buf);
        return 18;
    }

    if (n >= 10000000000000000lu) {
        FILL_NEXT(n, buf, 17);
        FILL_NEXT(n, buf, 14);
        FILL_NEXT(n, buf, 11);
        FILL_NEXT(n, buf, 8);
        FILL_NEXT(n, buf, 5);
        FILL2(n, buf);
        return 17;
    }

    if (n >= 1000000000000000lu) {
        FILL_NEXT(n, buf, 16);
        FILL_NEXT(n, buf, 13);
        FILL_NEXT(n, buf, 10);
        FILL_NEXT(n, buf, 7);
        FILL_NEXT(n, buf, 4);
        FILL1(n, buf);
        return 16;
    }

    if (n >= 100000000000000lu) {
        FILL_NEXT(n, buf, 15);
        FILL_NEXT(n, buf, 12);
        FILL_NEXT(n, buf, 9);
        FILL_NEXT(n, buf, 6);
        FILL3(n, buf);
        return 15;
    }

    if (n >= 10000000000000lu) {
        FILL_NEXT(n, buf, 14);
        FILL_NEXT(n, buf, 11);
        FILL_NEXT(n, buf, 8);
        FILL_NEXT(n, buf, 5);
        FILL2(n, buf);
        return 14;
    }

    if (n >= 1000000000000lu) {
        FILL_NEXT(n, buf, 13);
        FILL_NEXT(n, buf, 10);
        FILL_NEXT(n, buf, 7);
        FILL_NEXT(n, buf, 4);
        FILL1(n, buf);
        return 13;
    }

    if (n >= 100000000000lu) {
        FILL_NEXT(n, buf, 12);
        FILL_NEXT(n, buf, 9);
        FILL_NEXT(n, buf, 6);
        FILL3(n, buf);
        return 12;
    }

    if (n >= 10000000000lu) {
        FILL_NEXT(n, buf, 11);
        FILL_NEXT(n, buf, 8);
        FILL_NEXT(n, buf, 5);
        FILL2(n, buf);
        return 11;
    }

    if (n >= 1000000000lu) {
        FILL_NEXT(n, buf, 10);
        FILL_NEXT(n, buf, 7);
        FILL_NEXT(n, buf, 4);
        FILL1(n, buf);
        return 10;
    }

    if (n >= 100000000lu) {
        FILL_NEXT(n, buf, 9);
        FILL_NEXT(n, buf, 6);
        FILL3(n, buf);
        return 9;
    }

    if (n >= 10000000lu) {
        FILL_NEXT(n, buf, 8);
        FILL_NEXT(n, buf, 5);
        FILL2(n, buf);
        return 8;
    }

    if (n >= 1000000lu) {
        FILL_NEXT(n, buf, 7);
        FILL_NEXT(n, buf, 4);
        FILL1(n, buf);
        return 7;
    }

    if (n >= 100000lu) {
        FILL_NEXT(n, buf, 6);
        FILL3(n, buf);
        return 6;
    }

    if (n >= 10000lu) {
        FILL_NEXT(n, buf, 5);
        FILL2(n, buf);
        return 5;
    }

    if (n >= 1000lu) {
        FILL_NEXT(n, buf, 4);
        FILL1(n, buf);
        return 4;
    }

    if (n >= 100lu) {
        FILL3(n, buf);
        return 3;
    }

    if (n >= 10lu) {
        FILL2(n, buf);
        return 2;
    }

    FILL1(n, buf);
    return 1;
}

unsigned int NOINLINE
nums_to_str2(const uint64_t * RESTRICT nums, unsigned int n,
    char * RESTRICT buf)
{
    char *p = buf;
    for (unsigned int i = 0; i < n; ++i) {
        p += num_to_str2(nums[i], p);
    }
    *p = '\0';
    return p - buf;
}

void
init_decimals2()
{
    for (int i = 0; i < 1000; ++i) {
        int n = i;
        decimals[i][2] = n % 10 + '0';
        n /= 10;
        decimals[i][1] = n % 10 + '0';
        n /= 10;
        decimals[i][0] = n + '0';
    }
}
