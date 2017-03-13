#ifndef SHA256_HPP_
#define SHA256_HPP_ 1

#include <cstddef>

#include <openssl/sha.h>

static inline void sha256(const char *str, size_t len, unsigned char *hash)
{
    SHA256((const unsigned char *)str, len, hash);
}

void hash_str(const unsigned char *hash, char *str);

bool check_prefix(const unsigned char *hash, const char *str,
    unsigned int len);

#endif
