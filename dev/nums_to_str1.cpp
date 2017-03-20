#include <cstddef>
#include <cstdint>

#define RESTRICT __restrict__
#define NO_STRICT_ALIASING __attribute__((optimize("-fno-strict-aliasing")))
#define NOINLINE __attribute__((noinline))

static constexpr uint16_t decimals[100] = {
    0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830,
    0x3930, 0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731,
    0x3831, 0x3931, 0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632,
    0x3732, 0x3832, 0x3932, 0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533,
    0x3633, 0x3733, 0x3833, 0x3933, 0x3034, 0x3134, 0x3234, 0x3334, 0x3434,
    0x3534, 0x3634, 0x3734, 0x3834, 0x3934, 0x3035, 0x3135, 0x3235, 0x3335,
    0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935, 0x3036, 0x3136, 0x3236,
    0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936, 0x3037, 0x3137,
    0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737, 0x3837, 0x3937, 0x3038,
    0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938,
    0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839,
    0x3939
};

template <unsigned int len>
inline void NO_STRICT_ALIASING
num_to_str_(uint64_t n, char *buf)
{
    uint16_t *b = (uint16_t *)(buf + (len % 2));

    for (unsigned int i = 0; i < len / 2; ++i) {
        b[len / 2 - 1 - i] = decimals[n % 100];
        n /= 100;
    }

    if (len % 2 == 1) {
        buf[0] = n + '0';
    }
}

template <unsigned int len>
inline void
num_to_str_align(uint64_t n, char *buf)
{
    const unsigned int off_by = ((size_t)buf + len) % 2;

    if (off_by == 0) {
        num_to_str_<len>(n, buf);
    } else {
        buf[len - 1] = n % 10 + '0';
        n /= 10;
        num_to_str_<len - 1>(n, buf);
    }
}

unsigned int
num_to_str1(uint64_t n, char *buf)
{
    if (n >= 10000000000000000000lu) {
        num_to_str_align<20>(n, buf);
        return 20;
    }

    if (n >= 1000000000000000000lu) {
        num_to_str_align<19>(n, buf);
        return 19;
    }

    if (n >= 100000000000000000lu) {
        num_to_str_align<18>(n, buf);
        return 18;
    }

    if (n >= 10000000000000000lu) {
        num_to_str_align<17>(n, buf);
        return 17;
    }

    if (n >= 1000000000000000lu) {
        num_to_str_align<16>(n, buf);
        return 16;
    }

    if (n >= 100000000000000lu) {
        num_to_str_align<15>(n, buf);
        return 15;
    }

    if (n >= 10000000000000lu) {
        num_to_str_align<14>(n, buf);
        return 14;
    }

    if (n >= 1000000000000lu) {
        num_to_str_align<13>(n, buf);
        return 13;
    }

    if (n >= 100000000000lu) {
        num_to_str_align<12>(n, buf);
        return 12;
    }

    if (n >= 10000000000lu) {
        num_to_str_align<11>(n, buf);
        return 11;
    }

    if (n >= 1000000000lu) {
        num_to_str_align<10>(n, buf);
        return 10;
    }

    if (n >= 100000000lu) {
        num_to_str_align<9>(n, buf);
        return 9;
    }

    if (n >= 10000000lu) {
        num_to_str_align<8>(n, buf);
        return 8;
    }

    if (n >= 1000000lu) {
        num_to_str_align<7>(n, buf);
        return 7;
    }

    if (n >= 100000lu) {
        num_to_str_align<6>(n, buf);
        return 6;
    }

    if (n >= 10000lu) {
        num_to_str_align<5>(n, buf);
        return 5;
    }

    if (n >= 1000lu) {
        num_to_str_align<4>(n, buf);
        return 4;
    }

    if (n >= 100lu) {
        num_to_str_align<3>(n, buf);
        return 3;
    }

    if (n >= 10lu) {
        buf[1] = n % 10 + '0';
        buf[0] = n / 10 + '0';
        return 2;
    }

    buf[0] = n + '0';
    return 1;
}

unsigned int NOINLINE
nums_to_str1(const uint64_t * RESTRICT nums, unsigned int n,
    char * RESTRICT buf)
{
    char *p = buf;
    for (unsigned int i = 0; i < n; ++i) {
        p += num_to_str1(nums[i], p);
    }
    *p = '\0';
    return p - buf;
}

void
init_decimals1()
{ }
