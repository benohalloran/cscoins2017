#include <cassert>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>

using namespace std;

template <typename F, typename G>
void
test_nums_to_str(int id, F f, G g)
{
    printf("Testing nums_to_str %d\n", id);
    g();
    constexpr int max_seeds = 1024;
    constexpr int max_rands = 256;
    uint64_t r[max_seeds * 2 + 1][max_rands];
    for (int i = 0; i < max_seeds; ++i) {
        mt19937_64 gen (rand());
        for (int j = 0; j < max_rands; ++j) {
            r[i][j] = gen();
        }
    }
    for (int i = max_seeds; i < 2 * max_seeds; ++i) {
        mt19937 gen (rand());
        r[i][0] = 0;
        for (int j = 1; j < max_rands; ++j) {
            r[i][j] = gen() % j;
        }
    }
    memset(r[max_seeds * 2], 0, sizeof(r[max_seeds * 2]));
    for (unsigned int i = 0; i < 64; ++i) {
        r[max_seeds * 2][i] = 1lu << i;
    }

    char buf[max_rands * 20 + 1];
    for (int i = 0; i < max_seeds * 2 + 1; ++i) {
        string s;
        for (int j = 0; j < max_rands; ++j) {
            s += to_string(r[i][j]);
        }
        f(r[i], max_rands, buf);
        assert(strlen(buf) <= max_rands * 20);
        if (strcmp(buf, s.c_str())) {
            for (int k = 0; k < 8; ++k) {
                printf("%lu\n", r[i][k]);
            }
            fprintf(stderr, "n = %lu\nbuf = %s\ns = %s\ni = %d\n", s.size(),
                buf, s.c_str(), i);
        }
        assert(strlen(buf) == s.size());
        if (strcmp(buf, s.c_str())) {
            for (int k = 0; k < 8; ++k) {
                printf("%lu\n", r[i][k]);
            }
            fprintf(stderr, "n = %lu\nbuf = %s\ns = %s\ni = %d\n", s.size(),
                buf, s.c_str(), i);
        }
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
void init_decimals0();
void init_decimals1();
void init_decimals2();
void init_decimals3();
void init_decimals4();
void init_decimals5();
void init_decimals6();
void init_decimals7();

void
test_nums_to_str()
{
    test_nums_to_str(0, nums_to_str0, init_decimals0);
    test_nums_to_str(1, nums_to_str1, init_decimals1);
    test_nums_to_str(2, nums_to_str2, init_decimals2);
    test_nums_to_str(3, nums_to_str3, init_decimals3);
    test_nums_to_str(4, nums_to_str4, init_decimals4);
    test_nums_to_str(5, nums_to_str5, init_decimals5);
    test_nums_to_str(6, nums_to_str6, init_decimals6);
    test_nums_to_str(7, nums_to_str7, init_decimals7);
}

int
main()
{
    test_nums_to_str();
    return 0;
}

// void
// test_solver()
// {
//     for (int align = 0; align <= 8; ++align) {
//         char mem1[1024 + 8];
//         char *buf = mem1 + 8 - (size_t)mem1 % 8 + align;
//
//         assert(num_to_str(7, buf) == 1);
//         assert(buf[0] == '7');
//         assert(buf[1] == '\0');
//         assert(num_to_str(36, buf) == 2);
//         assert(buf[0] == '3');
//         assert(buf[1] == '6');
//         assert(buf[2] == '\0');
//         assert(num_to_str(1024, buf) == 4);
//         assert(buf[0] == '1');
//         assert(buf[1] == '0');
//         assert(buf[2] == '2');
//         assert(buf[3] == '4');
//         assert(buf[4] == '\0');
//         assert(num_to_str(12345, buf) == 5);
//         assert(buf[0] == '1');
//         assert(buf[1] == '2');
//         assert(buf[2] == '3');
//         assert(buf[3] == '4');
//         assert(buf[4] == '5');
//         assert(buf[5] == '\0');
//         assert(num_to_str(18446744073709551615lu, buf) == 20);
//         assert(buf[0] == '1');
//         assert(buf[1] == '8');
//         assert(buf[2] == '4');
//         assert(buf[3] == '4');
//         assert(buf[4] == '6');
//         assert(buf[5] == '7');
//         assert(buf[6] == '4');
//         assert(buf[7] == '4');
//         assert(buf[8] == '0');
//         assert(buf[9] == '7');
//         assert(buf[10] == '3');
//         assert(buf[11] == '7');
//         assert(buf[12] == '0');
//         assert(buf[13] == '9');
//         assert(buf[14] == '5');
//         assert(buf[15] == '5');
//         assert(buf[16] == '1');
//         assert(buf[17] == '6');
//         assert(buf[18] == '1');
//         assert(buf[19] == '5');
//         assert(buf[20] == '\0');
//         assert(num_to_str(8446744073709551615lu, buf) == 19);
//         assert(buf[0] == '8');
//         assert(buf[1] == '4');
//         assert(buf[2] == '4');
//         assert(buf[3] == '6');
//         assert(buf[4] == '7');
//         assert(buf[5] == '4');
//         assert(buf[6] == '4');
//         assert(buf[7] == '0');
//         assert(buf[8] == '7');
//         assert(buf[9] == '3');
//         assert(buf[10] == '7');
//         assert(buf[11] == '0');
//         assert(buf[12] == '9');
//         assert(buf[13] == '5');
//         assert(buf[14] == '5');
//         assert(buf[15] == '1');
//         assert(buf[16] == '6');
//         assert(buf[17] == '1');
//         assert(buf[18] == '5');
//         assert(buf[19] == '\0');
//         assert(num_to_str(446744073709551615lu, buf) == 18);
//         assert(buf[0] == '4');
//         assert(buf[1] == '4');
//         assert(buf[2] == '6');
//         assert(buf[3] == '7');
//         assert(buf[4] == '4');
//         assert(buf[5] == '4');
//         assert(buf[6] == '0');
//         assert(buf[7] == '7');
//         assert(buf[8] == '3');
//         assert(buf[9] == '7');
//         assert(buf[10] == '0');
//         assert(buf[11] == '9');
//         assert(buf[12] == '5');
//         assert(buf[13] == '5');
//         assert(buf[14] == '1');
//         assert(buf[15] == '6');
//         assert(buf[16] == '1');
//         assert(buf[17] == '5');
//         assert(buf[18] == '\0');
//     }
//
//     vector<uint64_t> array = {2, 1};
//     fast_sort(array.data(), 2);
//     assert(array[0] == 1);
//     assert(array[1] == 2);
//     array = {5, 7, 3, 4, 2, 6, 1, 8};
//     fast_sort(array.data(), 8);
//     assert(array[0] == 1);
//     assert(array[1] == 2);
//     assert(array[2] == 3);
//     assert(array[3] == 4);
//     assert(array[4] == 5);
//     assert(array[5] == 6);
//     assert(array[6] == 7);
//     assert(array[7] == 8);
//
//     unsigned char mem2[32 + 8];
//     unsigned char *hash = mem2 + 8 - (size_t)mem2 % 8;
//     assert(IS_ALIGNED_TO(hash, 8));
//     sha256("", 0, hash);
//     assert(*(uint64_t *)hash == 0x141cfc9842c4b0e3);
//     assert(check_prefix(hash, 0xb0e3, strdup("e3b"), 3));
//     assert(!check_prefix(hash, 0xb0e3, strdup("e3c"), 3));
//     assert(check_prefix(hash, 0xb0e3, strdup("e3b0"), 4));
//     assert(!check_prefix(hash, 0xe3b0, strdup("e3b0"), 4));
//     assert(check_prefix(hash, 0xb0e3, strdup("e3b0c"), 5));
//     assert(!check_prefix(hash, 0xb0e3, strdup("e3b0d"), 5));
//     assert(check_prefix(hash, 0xb0e3, strdup("e3b0c4"), 6));
//     assert(!check_prefix(hash, 0xb0e3, strdup("e3b0c5"), 6));
//     char str[65];
//     hash_str(hash, str);
//     assert(str[64] == '\0');
//     assert(!strcmp(str,
//             "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));
//
//     challenge c;
//     parse("{ \
//         \"challenge_id\": 0, \
//         \"challenge_name\": \"sorted_list\", \
//         \"last_solution_hash\": \
//         \"0000000000000000000000000000000000000000000000000000000000000000\", \
//         \"hash_prefix\": \"94f8\", \
//         \"parameters\": { \
//             \"nb_elements\": 20 \
//         } \
//     }", c);
//     assert(c.id == 0);
//     assert(c.type == TYPE_SORTED_LIST);
//     assert(!strcmp(c.prev_hash,
//             "0000000000000000000000000000000000000000000000000000000000000000"));
//     assert(!strcmp(c.hash_prefix, "94f8"));
//     assert(c.n == 20);
//     assert(c.hash_prefix_len == 4);
//     assert(c.prefix4 == 0xf894);
// }
//
//
