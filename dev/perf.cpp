#include <algorithm>
#include <atomic>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <openssl/sha.h>
#include <random>
#include <string>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>

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
    uint64_t r[max_seeds][max_rands];
    for (int i = 0; i < max_seeds; ++i) {
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
            while (!start) { }
            times[i] = get_time([&] () {
                unsigned char str[4096];
                unsigned char hash[SHA256_DIGEST_LENGTH];
                memset(str, rand() % UCHAR_MAX, sizeof(str));
                char buf[20 * max_rands + 1];
                for (int i = 0; i < max_iters; ++i) {
                    for (int j = 0; j < max_seeds; ++j) {
                        SHA256(str, 1024, hash);
                        SHA256(str, 1024, hash);
                        volatile unsigned int len = f(r[j], max_rands, buf);
                        (void)len;
                    }
                }
                volatile char c = buf[0];
                (void)c;
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
time_nums_to_str()
{
    printf("nums_to_str:\n");
    // time_nums_to_str(0, nums_to_str0, init_decimals0);
    time_nums_to_str(1, nums_to_str1, init_decimals1);
    time_nums_to_str(2, nums_to_str2, init_decimals2);
    time_nums_to_str(3, nums_to_str3, init_decimals3);
    time_nums_to_str(4, nums_to_str4, init_decimals4);
    time_nums_to_str(5, nums_to_str5, init_decimals5);
    time_nums_to_str(6, nums_to_str6, init_decimals6);
    time_nums_to_str(7, nums_to_str7, init_decimals7);
}

int
main()
{
    for (int i = 0; i < 3; ++i) {
        get_time([](){});
    }

    time_nums_to_str();

    return 0;
}
