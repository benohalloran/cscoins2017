#include "sha256.hpp"

#include <openssl/sha.h>

void sha256(const char *str, size_t len, char *hash)
{
    static const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    unsigned char buf[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)str, len, buf);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hash[i * 2] = hex[(buf[i] & 0xf0) >> 4];
        hash[i * 2 + 1] = hex[buf[i] & 0x0f];
    }
    hash[SHA256_DIGEST_LENGTH * 2] = '\0';
}
