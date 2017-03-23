#include "solver.hpp"
#include "sha256.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>
#include <vector>

using namespace std;

template <typename F, typename G>
void
test_nums_to_str(int id, F f, G g)
{
    printf("Testing nums_to_str %d\n", id);
    g();
    constexpr int max_seeds = 1024;
    constexpr int max_rands = 256;
    vector<uint64_t> r[max_seeds * 2 + 1];
    for (int i = 0; i < max_seeds; ++i) {
        r[i].resize(max_rands);
        mt19937_64 gen (rand());
        for (int j = 0; j < max_rands; ++j) {
            r[i][j] = gen();
        }
    }
    for (int i = max_seeds; i < 2 * max_seeds; ++i) {
        r[i].resize(max_rands);
        mt19937 gen (rand());
        r[i][0] = 0;
        for (int j = 1; j < max_rands; ++j) {
            r[i][j] = gen() % j;
        }
    }
    r[max_seeds * 2].resize(max_rands);
    memset(r[max_seeds * 2].data(), 0, max_rands * sizeof(r[max_seeds * 2][0]));
    for (unsigned int i = 0; i < 64; ++i) {
        r[max_seeds * 2][i] = 1lu << i;
    }

    char buf[max_rands * 20 + 21];
    for (int i = 0; i < max_seeds * 2 + 1; ++i) {
        string s;
        for (int j = 0; j < max_rands; ++j) {
            s += to_string(r[i][j]);
        }
        f(r[i].data(), max_rands, buf);
        assert(strlen(buf) <= max_rands * 20);
        if (strcmp(buf, s.c_str())) {
            for (int k = 0; k < 8; ++k) {
                printf("%lu\n", r[i][k]);
            }
            fprintf(stderr, "n = %lu\nbuf = %s\ns = %s\ni = %d\n", s.size(),
                buf, s.c_str(), i);
        }
        assert(strlen(buf) == s.size());
        assert(!strcmp(buf, s.c_str()));
    }
}

unsigned int nums_to_str0(const uint64_t *, unsigned int, char *);
unsigned int nums_to_str1(const uint64_t *, unsigned int, char *);
unsigned int nums_to_str2(const uint64_t *, unsigned int, char *);
unsigned int nums_to_str3(const uint64_t *, unsigned int, char *);
unsigned int nums_to_str4(const uint64_t *, unsigned int, char *);
unsigned int nums_to_str5(const uint64_t *, unsigned int, char *);
unsigned int nums_to_str6(const uint64_t *, unsigned int, char *);
unsigned int nums_to_str7(const uint64_t *, unsigned int, char *);
unsigned int nums_to_str8(const uint64_t *, unsigned int, char *);
void init_decimals0();
void init_decimals1();
void init_decimals2();
void init_decimals3();
void init_decimals4();
void init_decimals5();
void init_decimals6();
void init_decimals7();
void init_decimals8();

void
test_nums_to_str()
{
    test_nums_to_str(0, nums_to_str0, init_decimals0);
    test_nums_to_str(1, nums_to_str1, init_decimals1);
    test_nums_to_str(2, nums_to_str2, init_decimals2);
    test_nums_to_str(3, nums_to_str3, init_decimals3);
    test_nums_to_str(4, nums_to_str4, init_decimals4);
    // test_nums_to_str(5, nums_to_str5, init_decimals5);
    // test_nums_to_str(6, nums_to_str6, init_decimals6);
    // test_nums_to_str(7, nums_to_str7, init_decimals7);
    // test_nums_to_str(8, nums_to_str8, init_decimals8);
}

void
test_sha256()
{
    printf("Testing sha256\n");
    buffer<32> hash;
    buffer<65> prefix;
    sha256("", 0, hash);
    assert(hash.l[0] == 0x141cfc9842c4b0e3lu);
    strcpy((char *)prefix.c, "e3b");
    assert(check_prefix(hash, 0xb0e3, prefix, 3));
    strcpy((char *)prefix.c, "e3c");
    assert(!check_prefix(hash, 0xb0e3, prefix, 3));
    strcpy((char *)prefix.c, "e3b0");
    assert(check_prefix(hash, 0xb0e3, prefix, 4));
    strcpy((char *)prefix.c, "e3b0");
    assert(!check_prefix(hash, 0xe3b0, prefix, 4));
    strcpy((char *)prefix.c, "e3b0c");
    assert(check_prefix(hash, 0xb0e3, prefix, 5));
    strcpy((char *)prefix.c, "e3b0d");
    assert(!check_prefix(hash, 0xb0e3, prefix, 5));
    strcpy((char *)prefix.c, "e3b0c4");
    assert(check_prefix(hash, 0xb0e3, prefix, 6));
    strcpy((char *)prefix.c, "e3b0c5");
    assert(!check_prefix(hash, 0xb0e3, prefix, 6));
    memset(prefix.c, 0, 65);
    hash_str(hash, prefix);
    assert(prefix.c[64] == '\0');
    assert(!strcmp((const char *)prefix.c,
            "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));
}

template <typename F>
void
test_sorting(int id, F f)
{
    printf("Testing sorting %d\n", id);
    constexpr int max = 4096;
    vector<uint64_t> r[max];
    for (int i = 0; i < max; ++i) {
        r[i].reserve(max * 2);
        r[i].resize(max);
        mt19937_64 gen (rand());
        for (int j = 0; j < max; ++j) {
            r[i][j] = gen() & (-1lu >> (64 - BIN_SHIFT));
        }
    }

    for (int i = 0; i < max; ++i) {
        vector<uint64_t> cpy (r[i]);
        sort(cpy.begin(), cpy.begin() + i);
        f(r[i].data(), i);
        for (int j = 0; j < i; ++j) {
            if (cpy[j] != r[i][j]) {
                printf("i = %d, j = %d\n", i, j);
                for (int k = 0; k < i; ++k) {
                    printf("%lu ", r[i][k]);
                }
                fputc('\n', stdout);
                for (int k = 0; k < i; ++k) {
                    printf("%lu ", cpy[k]);
                }
                fputc('\n', stdout);
            }
            assert(cpy[j] == r[i][j]);
        }
    }
}

void fast_sort0(uint64_t *, unsigned int);
void fast_sort1(uint64_t *, unsigned int);
void fast_sort2(uint64_t *, unsigned int);
void fast_sort3(uint64_t *, unsigned int);
void fast_sort4(uint64_t *, unsigned int);

void
test_sorting()
{
    test_sorting(0, fast_sort0);
    test_sorting(1, fast_sort1);
    test_sorting(2, fast_sort2);
    test_sorting(3, fast_sort3);
    test_sorting(4, fast_sort4);
}

void
test_parser()
{
    printf("Testing parser\n");
    challenge c;
    parse("{ \
        \"challenge_id\": 0, \
        \"challenge_name\": \"sorted_list\", \
        \"last_solution_hash\": \
        \"0000000000000000000000000000000000000000000000000000000000000000\", \
        \"hash_prefix\": \"94f8\", \
        \"parameters\": { \
            \"nb_elements\": 20 \
        } \
    }", c);
    assert(c.id == 0);
    assert(c.type == TYPE_SORTED_LIST);
    assert(!strcmp((const char *)c.prev_hash.c,
            "0000000000000000000000000000000000000000000000000000000000000000"));
    assert(!strcmp((const char *)c.hash_prefix.c, "94f8"));
    assert(c.n == 20);
    assert(c.hash_prefix_len == 4);
    assert(c.prefix4 == 0xf894);
}

int
main()
{
    test_nums_to_str();
    test_sorting();
    test_sha256();
    test_parser();
    return 0;
}
