#define SOLVER_CPP_ 1
#include "solver.hpp"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#if SET_AFFINITY
#include <sched.h>
#endif

namespace {

using namespace std;

const unsigned int T = thread::hardware_concurrency();

struct sorting_solver;
struct path_solver;

vector<sorting_solver> sorting_solvers;
vector<path_solver> path_solvers;
unsigned int new_sorting_challenge, new_path_challenge;
bool new_solution;
challenge current_challenge;
solution our_solution;
mutex mtx;
condition_variable sorting_cv, path_cv, solution_cv;

#if SET_AFFINITY
void set_affinity(int cpu)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    int ret = sched_setaffinity(0, sizeof(cpu_set_t), &set);
    assert(ret == 0);
}
#else
inline void set_affinity(int cpu)
{
    (void)cpu;
}
#endif

#include "num_to_str.hpp"
#include "sorting.hpp"
#include "sha256.hpp"
#include "sorting_solver.hpp"
#include "path_solver.hpp"

inline void end_all_solvers()
{
    for (auto &solver : sorting_solvers) {
        solver.should_end = true;
    }
    for (auto &solver : path_solvers) {
        solver.should_end = true;
    }
}

}

solution solve(const char *json)
{
    challenge c;
    parse(json, c);

    mtx.lock();
    current_challenge = c;
    switch (c.type) {
    case TYPE_SORTED_LIST:
    case TYPE_REVERSE_SORTED_LIST:
        new_sorting_challenge = T;
        break;

    case TYPE_SHORTEST_PATH:
        new_path_challenge = T;
        break;
    }
    mtx.unlock();

    end_all_solvers();

    switch (c.type) {
    case TYPE_SORTED_LIST:
    case TYPE_REVERSE_SORTED_LIST:
        sorting_cv.notify_all();
        break;

    case TYPE_SHORTEST_PATH:
        path_cv.notify_all();
        break;
    }

    unique_lock<mutex> lk (mtx);
    solution_cv.wait(lk, [] () {
        return new_solution;
    });
    end_all_solvers();
    new_solution = false;
    return our_solution;
}

void init_solver()
{
    init_decimals();
    sorting_solvers.reserve(T);
    path_solvers.reserve(T);
    for (unsigned int i = 0; i < T; ++i) {
        sorting_solvers.emplace_back(i);
        path_solvers.emplace_back(i);
    }
    for (unsigned int i = 0; i < T; ++i) {
        sorting_solvers[i].t = thread([=] () {
            sorting_solvers[i].loop();
        });
        sorting_solvers[i].t.detach();
        // path_solvers[i].t = thread([=] () {
        //     path_solvers[i].loop();
        // });
    }
}

#ifdef TESTING
void test_solver()
{
    for (int align = 0; align <= 4; ++align) {
        char mem1[1024 + 8];
        char *buf = mem1 + 8 - (size_t)mem1 % 8 + align;

        assert(num_to_str(7, buf) == 1);
        assert(buf[0] == '7');
        assert(buf[1] == '\0');
        assert(num_to_str(36, buf) == 2);
        assert(buf[0] == '3');
        assert(buf[1] == '6');
        assert(buf[2] == '\0');
        assert(num_to_str(1024, buf) == 4);
        assert(buf[0] == '1');
        assert(buf[1] == '0');
        assert(buf[2] == '2');
        assert(buf[3] == '4');
        assert(buf[4] == '\0');
        assert(num_to_str(12345, buf) == 5);
        assert(buf[0] == '1');
        assert(buf[1] == '2');
        assert(buf[2] == '3');
        assert(buf[3] == '4');
        assert(buf[4] == '5');
        assert(buf[5] == '\0');
        assert(num_to_str(18446744073709551615lu, buf) == 20);
        assert(buf[0] == '1');
        assert(buf[1] == '8');
        assert(buf[2] == '4');
        assert(buf[3] == '4');
        assert(buf[4] == '6');
        assert(buf[5] == '7');
        assert(buf[6] == '4');
        assert(buf[7] == '4');
        assert(buf[8] == '0');
        assert(buf[9] == '7');
        assert(buf[10] == '3');
        assert(buf[11] == '7');
        assert(buf[12] == '0');
        assert(buf[13] == '9');
        assert(buf[14] == '5');
        assert(buf[15] == '5');
        assert(buf[16] == '1');
        assert(buf[17] == '6');
        assert(buf[18] == '1');
        assert(buf[19] == '5');
        assert(buf[20] == '\0');
        assert(num_to_str(8446744073709551615lu, buf) == 19);
        assert(buf[0] == '8');
        assert(buf[1] == '4');
        assert(buf[2] == '4');
        assert(buf[3] == '6');
        assert(buf[4] == '7');
        assert(buf[5] == '4');
        assert(buf[6] == '4');
        assert(buf[7] == '0');
        assert(buf[8] == '7');
        assert(buf[9] == '3');
        assert(buf[10] == '7');
        assert(buf[11] == '0');
        assert(buf[12] == '9');
        assert(buf[13] == '5');
        assert(buf[14] == '5');
        assert(buf[15] == '1');
        assert(buf[16] == '6');
        assert(buf[17] == '1');
        assert(buf[18] == '5');
        assert(buf[19] == '\0');
        assert(num_to_str(446744073709551615lu, buf) == 18);
        assert(buf[0] == '4');
        assert(buf[1] == '4');
        assert(buf[2] == '6');
        assert(buf[3] == '7');
        assert(buf[4] == '4');
        assert(buf[5] == '4');
        assert(buf[6] == '0');
        assert(buf[7] == '7');
        assert(buf[8] == '3');
        assert(buf[9] == '7');
        assert(buf[10] == '0');
        assert(buf[11] == '9');
        assert(buf[12] == '5');
        assert(buf[13] == '5');
        assert(buf[14] == '1');
        assert(buf[15] == '6');
        assert(buf[16] == '1');
        assert(buf[17] == '5');
        assert(buf[18] == '\0');
    }

    vector<uint64_t> array = {2, 1};
    fast_sort(array.data(), 2);
    assert(array[0] == 1);
    assert(array[1] == 2);
    array = {5, 7, 3, 4, 2, 6, 1, 8};
    fast_sort(array.data(), 8);
    assert(array[0] == 1);
    assert(array[1] == 2);
    assert(array[2] == 3);
    assert(array[3] == 4);
    assert(array[4] == 5);
    assert(array[5] == 6);
    assert(array[6] == 7);
    assert(array[7] == 8);

    unsigned char mem2[32 + 8];
    unsigned char *hash = mem2 + 8 - (size_t)mem2 % 8;
    assert(IS_ALIGNED_TO(hash, 8));
    sha256("", 0, hash);
    assert(*(uint64_t *)hash == 0x141cfc9842c4b0e3);
    assert(check_prefix(hash, 0xb0e3, strdup("e3b"), 3));
    assert(!check_prefix(hash, 0xb0e3, strdup("e3c"), 3));
    assert(check_prefix(hash, 0xb0e3, strdup("e3b0"), 4));
    assert(!check_prefix(hash, 0xe3b0, strdup("e3b0"), 4));
    assert(check_prefix(hash, 0xb0e3, strdup("e3b0c"), 5));
    assert(!check_prefix(hash, 0xb0e3, strdup("e3b0d"), 5));
    assert(check_prefix(hash, 0xb0e3, strdup("e3b0c4"), 6));
    assert(!check_prefix(hash, 0xb0e3, strdup("e3b0c5"), 6));
    char str[65];
    hash_str(hash, str);
    assert(str[64] == '\0');
    assert(!strcmp(str,
            "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));

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
    assert(!strcmp(c.prev_hash,
            "0000000000000000000000000000000000000000000000000000000000000000"));
    assert(!strcmp(c.hash_prefix, "94f8"));
    assert(c.n == 20);
    assert(c.hash_prefix_len == 4);
    assert(c.prefix4 == 0xf894);
}
#endif
