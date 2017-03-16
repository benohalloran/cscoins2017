#ifndef SOLVER_CPP_
#error "cannot be included"
#endif

#define FILL1 b[0] = decimals[n % 10000]; n /= 10000
#define FILL2 b[1] = decimals[n % 10000]; n /= 10000; FILL1
#define FILL3 b[2] = decimals[n % 10000]; n /= 10000; FILL2
#define FILL4 b[3] = decimals[n % 10000]; n /= 10000; FILL3
#define FILL5 b[4] = decimals[n % 10000]; n /= 10000; FILL4

uint32_t decimals[10000];
uint16_t decimals100[100];

void num_to_str_20(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)buf;
    FILL5;
}

void num_to_str_19(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 3);
    FILL4;
    *(uint16_t *)(buf + 1) = decimals100[n % 100];
    buf[0] = n / 100 + '0';
}

void num_to_str_18(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 2);
    FILL4;
    *(uint16_t *)buf = decimals100[n];
}

void num_to_str_17(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 1);
    FILL4;
    buf[0] = n + '0';
}

void num_to_str_16(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)buf;
    FILL4;
}

void num_to_str_15(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 3);
    FILL3;
    *(uint16_t *)(buf + 1) = decimals100[n % 100];
    buf[0] = n / 100 + '0';
}

void num_to_str_14(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 2);
    FILL3;
    *(uint16_t *)buf = decimals100[n];
}

void num_to_str_13(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 1);
    FILL3;
    buf[0] = n + '0';
}

void num_to_str_12(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)buf;
    FILL3;
}

void num_to_str_11(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 3);
    FILL2;
    *(uint16_t *)(buf + 1) = decimals100[n % 100];
    buf[0] = n / 100 + '0';
}

void num_to_str_10(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 2);
    FILL2;
    *(uint16_t *)buf = decimals100[n];
}

void num_to_str_9(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 1);
    FILL2;
    buf[0] = n + '0';
}

void num_to_str_8(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)buf;
    FILL2;
}

void num_to_str_7(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 3);
    FILL1;
    *(uint16_t *)(buf + 1) = decimals100[n % 100];
    buf[0] = n / 100 + '0';
}

void num_to_str_6(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 2);
    FILL1;
    *(uint16_t *)buf = decimals100[n];
}

void num_to_str_5(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)(buf + 1);
    FILL1;
    buf[0] = n + '0';
}

void num_to_str_4(uint64_t n, char *buf)
{
    uint32_t *b = (uint32_t *)buf;
    b[0] = decimals[n];
}

void num_to_str_3(uint64_t n, char *buf)
{
    *(uint16_t *)(buf + 1) = decimals100[n % 100];
    buf[0] = n / 100 + '0';
}

void num_to_str_2(uint64_t n, char *buf)
{
    *(uint16_t *)buf = decimals100[n];
}

void num_to_str_1(uint64_t n, char *buf)
{
    buf[0] = n + '0';
}

inline void num_to_str_align(uint64_t n, char *buf, unsigned int len)
{
    static void *align_jmp_table[4] = {
        &&off_by_0, &&off_by_1, &&off_by_2, &&off_by_3
    };
    static void (* const num_to_str_jmp_table[21])(uint64_t n, char *buf) = {
        NULL, num_to_str_1, num_to_str_2, num_to_str_3, num_to_str_4,
        num_to_str_5, num_to_str_6, num_to_str_7, num_to_str_8, num_to_str_9,
        num_to_str_10, num_to_str_11, num_to_str_12, num_to_str_13,
        num_to_str_14, num_to_str_15, num_to_str_16, num_to_str_17,
        num_to_str_18, num_to_str_19, num_to_str_20
    };

    const unsigned int off_by = ((size_t)buf + len) % 4;
    goto *align_jmp_table[off_by];

off_by_0:
    num_to_str_jmp_table[len](n, buf);
    return;

off_by_1:
    buf[len - 1] = n % 10 + '0';
    n /= 10;
    num_to_str_jmp_table[len - 1](n, buf);
    return;

off_by_2:
    *(uint16_t *)&buf[len - 2] = decimals100[n % 100];
    n /= 100;
    num_to_str_jmp_table[len - 2](n, buf);
    return;

off_by_3:
    *(uint16_t *)&buf[len - 2] = decimals100[n % 100];
    n /= 100;
    buf[len - 3] = n % 10 + '0';
    n /= 10;
    num_to_str_jmp_table[len - 3](n, buf);
    return;
}

inline unsigned int num_to_str(uint64_t n, char *buf)
{
    if (n < 1000) {
        if (n < 100) {
            if (n < 10) {
                buf[0] = n + '0';
                buf[1] = '\0';
                return 1;
            }

            buf[0] = n / 10 + '0';
            buf[1] = n % 10 + '0';
            buf[2] = '\0';
            return 2;
        }

        buf[2] = n % 10 + '0';
        n /= 10;
        buf[1] = n % 10 + '0';
        buf[0] = n / 10 + '0';
        buf[3] = '\0';
        return 3;
    }

    if (n >= 10000000000000000000lu) {
        buf[20] = '\0';
        num_to_str_align(n, buf, 20);
        return 20;
    }

    if (n >= 1000000000000000000lu) {
        buf[19] = '\0';
        num_to_str_align(n, buf, 19);
        return 19;
    }

    if (n >= 100000000000000000lu) {
        buf[18] = '\0';
        num_to_str_align(n, buf, 18);
        return 18;
    }

    if (n >= 10000000000000000lu) {
        buf[17] = '\0';
        num_to_str_align(n, buf, 17);
        return 17;
    }

    if (n >= 1000000000000000lu) {
        buf[16] = '\0';
        num_to_str_align(n, buf, 16);
        return 16;
    }

    if (n >= 100000000000000lu) {
        buf[15] = '\0';
        num_to_str_align(n, buf, 15);
        return 15;
    }

    if (n >= 10000000000000lu) {
        buf[14] = '\0';
        num_to_str_align(n, buf, 14);
        return 14;
    }

    if (n >= 1000000000000lu) {
        buf[13] = '\0';
        num_to_str_align(n, buf, 13);
        return 13;
    }

    if (n >= 100000000000lu) {
        buf[12] = '\0';
        num_to_str_align(n, buf, 12);
        return 12;
    }

    if (n >= 10000000000lu) {
        buf[11] = '\0';
        num_to_str_align(n, buf, 11);
        return 11;
    }

    if (n >= 1000000000lu) {
        buf[10] = '\0';
        num_to_str_align(n, buf, 10);
        return 10;
    }

    if (n >= 100000000lu) {
        buf[9] = '\0';
        num_to_str_align(n, buf, 9);
        return 9;
    }

    if (n >= 10000000lu) {
        buf[8] = '\0';
        num_to_str_align(n, buf, 8);
        return 8;
    }

    if (n >= 1000000lu) {
        buf[7] = '\0';
        num_to_str_align(n, buf, 7);
        return 7;
    }

    if (n >= 100000lu) {
        buf[6] = '\0';
        num_to_str_align(n, buf, 6);
        return 6;
    }

    if (n >= 10000lu) {
        buf[5] = '\0';
        num_to_str_align(n, buf, 5);
        return 5;
    }

    buf[4] = '\0';
    num_to_str_align(n, buf, 4);
    return 4;
}

void init_decimals()
{
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                for (int l = 0; l < 10; ++l) {
                    uint32_t num32;
                    char *buf = (char *)&num32;
                    buf[0] = i + '0';
                    buf[1] = j + '0';
                    buf[2] = k + '0';
                    buf[3] = l + '0';

                    decimals[i * 1000 + j * 100 + k * 10 + l] = num32;

                    uint16_t num16;
                    buf = (char *)&num16;
                    buf[0] = k + '0';
                    buf[1] = l + '0';
                    decimals100[k * 10 + l] = num16;
                }
            }
        }
    }
}
