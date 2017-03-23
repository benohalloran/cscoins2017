#include "solver.hpp"

#include <algorithm>
#include <atomic>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>

#include <openssl/sha.h>

using namespace std;

const unsigned int T = thread::hardware_concurrency();

template <typename F>
unsigned long
get_time(F f)
{
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    f();
    clock_gettime(CLOCK_MONOTONIC, &end);
    return 1000000000lu * (end.tv_sec - start.tv_sec) + end.tv_nsec -
        start.tv_nsec;
}

template <typename F, typename G>
void
time_nums_to_str(int id, F f, G g)
{
    g();
    constexpr int max_iters = 256;
    constexpr int max_seeds = 1024;
    constexpr int max_rands = 256;
    vector<uint64_t> r[max_seeds];
    for (int i = 0; i < max_seeds; ++i) {
        r[i].resize(max_rands);
        mt19937_64 gen (rand());
        for (int j = 0; j < max_rands; ++j) {
            r[i][j] = gen();
        }
    }

    vector<thread> threads (T);
    vector<unsigned long> times (T);
    atomic<bool> start (false);

    for (unsigned int i = 0; i < T; ++i) {
        threads[i] = thread([&] (unsigned int i) {
            unsigned char str[1024];
            unsigned char hash[SHA256_DIGEST_LENGTH];
            memset(str, rand() % UCHAR_MAX, sizeof(str));
            char buf[20 * max_rands + 1];
            while (!start) { }
            times[i] = get_time([&] () {
                for (int i = 0; i < max_iters; ++i) {
                    for (int j = 0; j < max_seeds; ++j) {
                        SHA256(str, 1024, hash);
                        SHA256(str, 1024, hash);
                        volatile unsigned int len = f(r[j].data(), max_rands, buf);
                        (void)len;
                    }
                }
            });
            volatile char c = buf[0];
            (void)c;
        }, i);
    }

    usleep(1000);
    start = true;
    for (unsigned int i = 0; i < T; ++i) {
        threads[i].join();
    }

    sort(times.begin(), times.end());
    printf("%d: %f\n", id, (double)times[T / 2] / 1000000);
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
time_nums_to_str()
{
    printf("nums_to_str:\n");
    // time_nums_to_str(0, nums_to_str0, init_decimals0);
    // time_nums_to_str(1, nums_to_str1, init_decimals1);
    // time_nums_to_str(2, nums_to_str2, init_decimals2);
    time_nums_to_str(3, nums_to_str3, init_decimals3);
    time_nums_to_str(4, nums_to_str4, init_decimals4);
    // time_nums_to_str(5, nums_to_str5, init_decimals5);
    // time_nums_to_str(6, nums_to_str6, init_decimals6);
    // time_nums_to_str(7, nums_to_str7, init_decimals7);
    // time_nums_to_str(8, nums_to_str8, init_decimals8);
}

template <typename F>
void
time_sorting(int id, F f, unsigned int elem_lg, unsigned int bin_lg)
{
    const unsigned int elems = 1 << elem_lg;
    const unsigned int max_iters = (1024 * 512 * elem_lg * elem_lg) / elems;
    const unsigned int n_bins = 1 << bin_lg;
    vector<vector<uint64_t>> r (max_iters);
    for (unsigned int i = 0; i < max_iters; ++i) {
        r[i].reserve(2 * elems);
        r[i].resize(elems);
        mt19937_64 gen (rand());
        for (unsigned int j = 0; j < elems; ++j) {
            r[i][j] = gen();
        }
    }

    vector<thread> threads (T);
    vector<unsigned long> times (T);
    atomic<bool> start (false);

    for (unsigned int i = 0; i < T; ++i) {
        threads[i] = thread([&] (unsigned int i) {
            unsigned char str[1024];
            unsigned char hash[SHA256_DIGEST_LENGTH];
            memset(str, rand() % UCHAR_MAX, sizeof(str));
            vector<vector<uint64_t>> bins (n_bins);
            for (auto &bin : bins) {
                bin.reserve(elems);
            }
            while (!start) { }
            times[i] = get_time([&] () {
                for (unsigned int i = 0; i < max_iters; ++i) {
                    SHA256(str, 1024, hash);
                    SHA256(str, 1024, hash);
                    for (uint64_t k : r[i]) {
                        bins[k >> (64 - bin_lg)].push_back(k);
                    }
                    for (auto &bin : bins) {
                        f(bin.data(), bin.size());
                        bin.clear();
                    }
                }
            });
        }, i);
    }

    usleep(1000);
    start = true;
    for (unsigned int i = 0; i < T; ++i) {
        threads[i].join();
    }

    sort(times.begin(), times.end());
    printf("%d: %f\n", id, (double)times[T / 2] / 1000000);
}

void fast_sort0(uint64_t *, unsigned int);
void fast_sort1(uint64_t *, unsigned int);
void fast_sort2(uint64_t *, unsigned int);
void fast_sort3(uint64_t *, unsigned int);
void fast_sort4(uint64_t *, unsigned int);
void fast_sort5(uint64_t *, unsigned int);

void
time_sorting()
{
    printf("sorting:\n");
    for (unsigned int bin_lg = 4; bin_lg <= 4; ++bin_lg) {
        unsigned int bins = 1 << bin_lg;
        for (unsigned int elem_lg = 7; elem_lg <= 12; ++elem_lg) {
            unsigned int elems = 1 << elem_lg;
            printf("bins = %u, elems = %u\n", bins, elems);
            // time_sorting(0, fast_sort0, elem_lg, bin_lg);
            // time_sorting(1, fast_sort1, elem_lg, bin_lg);
            time_sorting(2, fast_sort2, elem_lg, bin_lg);
            // time_sorting(3, fast_sort3, elem_lg, bin_lg);
            time_sorting(4, fast_sort4, elem_lg, bin_lg);
            time_sorting(5, fast_sort5, elem_lg, bin_lg);
        }
    }
}

int
main()
{
    for (int i = 0; i < 3; ++i) {
        get_time([](){});
    }

    // time_nums_to_str();
    time_sorting();
    return 0;
}
