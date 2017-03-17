#ifndef SOLVER_CPP_
#error "cannot be included"
#endif

#include <cassert>
#include <cstdint>
#include <cstring>

#include <openssl/sha.h>

// TODO - use asm if available
inline void sha256(const char *str, size_t len, unsigned char *hash)
{
    SHA256((const unsigned char *)str, len, hash);
}

#define H(s) (s[0] | (s[1] << 8))
constexpr uint64_t hex_vals[] = {
    H("00"), H("01"), H("02"), H("03"), H("04"), H("05"), H("06"), H("07"),
    H("08"), H("09"), H("0a"), H("0b"), H("0c"), H("0d"), H("0e"), H("0f"),
    H("10"), H("11"), H("12"), H("13"), H("14"), H("15"), H("16"), H("17"),
    H("18"), H("19"), H("1a"), H("1b"), H("1c"), H("1d"), H("1e"), H("1f"),
    H("20"), H("21"), H("22"), H("23"), H("24"), H("25"), H("26"), H("27"),
    H("28"), H("29"), H("2a"), H("2b"), H("2c"), H("2d"), H("2e"), H("2f"),
    H("30"), H("31"), H("32"), H("33"), H("34"), H("35"), H("36"), H("37"),
    H("38"), H("39"), H("3a"), H("3b"), H("3c"), H("3d"), H("3e"), H("3f"),
    H("40"), H("41"), H("42"), H("43"), H("44"), H("45"), H("46"), H("47"),
    H("48"), H("49"), H("4a"), H("4b"), H("4c"), H("4d"), H("4e"), H("4f"),
    H("50"), H("51"), H("52"), H("53"), H("54"), H("55"), H("56"), H("57"),
    H("58"), H("59"), H("5a"), H("5b"), H("5c"), H("5d"), H("5e"), H("5f"),
    H("60"), H("61"), H("62"), H("63"), H("64"), H("65"), H("66"), H("67"),
    H("68"), H("69"), H("6a"), H("6b"), H("6c"), H("6d"), H("6e"), H("6f"),
    H("70"), H("71"), H("72"), H("73"), H("74"), H("75"), H("76"), H("77"),
    H("78"), H("79"), H("7a"), H("7b"), H("7c"), H("7d"), H("7e"), H("7f"),
    H("80"), H("81"), H("82"), H("83"), H("84"), H("85"), H("86"), H("87"),
    H("88"), H("89"), H("8a"), H("8b"), H("8c"), H("8d"), H("8e"), H("8f"),
    H("90"), H("91"), H("92"), H("93"), H("94"), H("95"), H("96"), H("97"),
    H("98"), H("99"), H("9a"), H("9b"), H("9c"), H("9d"), H("9e"), H("9f"),
    H("a0"), H("a1"), H("a2"), H("a3"), H("a4"), H("a5"), H("a6"), H("a7"),
    H("a8"), H("a9"), H("aa"), H("ab"), H("ac"), H("ad"), H("ae"), H("af"),
    H("b0"), H("b1"), H("b2"), H("b3"), H("b4"), H("b5"), H("b6"), H("b7"),
    H("b8"), H("b9"), H("ba"), H("bb"), H("bc"), H("bd"), H("be"), H("bf"),
    H("c0"), H("c1"), H("c2"), H("c3"), H("c4"), H("c5"), H("c6"), H("c7"),
    H("c8"), H("c9"), H("ca"), H("cb"), H("cc"), H("cd"), H("ce"), H("cf"),
    H("d0"), H("d1"), H("d2"), H("d3"), H("d4"), H("d5"), H("d6"), H("d7"),
    H("d8"), H("d9"), H("da"), H("db"), H("dc"), H("dd"), H("de"), H("df"),
    H("e0"), H("e1"), H("e2"), H("e3"), H("e4"), H("e5"), H("e6"), H("e7"),
    H("e8"), H("e9"), H("ea"), H("eb"), H("ec"), H("ed"), H("ee"), H("ef"),
    H("f0"), H("f1"), H("f2"), H("f3"), H("f4"), H("f5"), H("f6"), H("f7"),
    H("f8"), H("f9"), H("fa"), H("fb"), H("fc"), H("fd"), H("fe"), H("ff")
};
#undef H

void hash_str(const unsigned char *hash, char *str)
{
    assert(IS_ALIGNED_TO(str, 2));
    uint16_t *str2 = (uint16_t *)str;
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        str2[i] = hex_vals[hash[i]];
    }
    str[SHA256_DIGEST_LENGTH * 2] = '\0';
}

inline bool check_prefix(const unsigned char *hash, uint16_t prefix4,
    const char *prefix, unsigned int len)
{
    assert(IS_ALIGNED_TO(hash, 2));
    assert(IS_ALIGNED_TO(prefix, 2));

    if (LIKELY(len >= 4)) {
        if (*(uint16_t *)hash != prefix4) {
            return false;
        }
    } else {
        uint16_t str2[2];
        char *str = (char *)str2;
        str2[0] = hex_vals[hash[0]];
        str2[1] = hex_vals[hash[1]];
        for (unsigned int i = 0; i < len; ++i) {
            if (str[i] != prefix[i]) {
                return false;
            }
        }
        return true;
    }

    for (unsigned int i = 2; i < SHA256_DIGEST_LENGTH; ++i) {
        unsigned int index = i * 2;
        if (len == index) {
            return true;
        }

        uint16_t str2;
        char *str = (char *)&str2;
        str2 = hex_vals[hash[i]];
        if (str[0] != prefix[index]) {
            return false;
        }

        ++index;
        if (len == index) {
            return true;
        }

        if (str[1] != prefix[index]) {
            return false;
        }
    }

    assert(0);
}
