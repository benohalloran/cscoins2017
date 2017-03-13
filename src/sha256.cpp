#include "sha256.hpp"

static const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
    '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void hash_str(const unsigned char *hash, char *str)
{
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        str[i * 2] = hex[(hash[i] & 0xf0) >> 4];
        str[i * 2 + 1] = hex[hash[i] & 0x0f];
    }
    str[SHA256_DIGEST_LENGTH * 2] = '\0';
}

bool check_prefix(const unsigned char *hash, const char *str,
    unsigned int len)
{
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        if (i * 2 + 1 >= len) {
            if (i * 2 < len && str[i * 2] != hex[(hash[i] & 0xf0) >> 4]) {
                return false;
            }

            return true;
        }

        if (str[i * 2] != hex[(hash[i] & 0xf0) >> 4]
            || str[i * 2 + 1] != hex[hash[i] & 0x0f]) {
            return false;
        }
    }

    return true;
}
