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
// struct path_solver;

vector<sorting_solver> sorting_solvers;
// vector<path_solver> path_solvers;
unsigned int new_sorting_challenge, new_path_challenge;
bool new_solution;
challenge current_challenge;
solution our_solution;
mutex mtx;
condition_variable sorting_cv, path_cv, solution_cv;
atomic<unsigned int> ready;

#if SET_AFFINITY
void
set_affinity(int cpu)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    int ret = sched_setaffinity(0, sizeof(cpu_set_t), &set);
    assert(ret == 0);
}
#else
inline void
set_affinity(int cpu)
{
    (void)cpu;
}
#endif

#include "num_to_str.hpp"
#include "sorting.hpp"
#include "sha256.hpp"
#include "sorting_solver.hpp"

void
end_all_solvers()
{
    for (auto &solver : sorting_solvers) {
        solver.should_end = true;
    }
    // for (auto &solver : path_solvers) {
    //     solver.should_end = true;
    // }
}

}

solution
solve(const char *json)
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
        assert(0);
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
        assert(0);
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

void
init_solver()
{
    init_decimals();
    ready = 0;
    sorting_solvers.reserve(T);
    // path_solvers.reserve(T);
    for (unsigned int i = 0; i < T; ++i) {
        sorting_solvers.emplace_back(i);
        // path_solvers.emplace_back(i);
    }
    for (unsigned int i = 0; i < T; ++i) {
        sorting_solvers[i].t = thread([=] () {
            sorting_solvers[i].loop();
        });
        sorting_solvers[i].t.detach();
        // path_solvers[i].t = thread([=] () {
        //     path_solvers[i].loop();
        // });
        // path_solvers[i].t.detach();
    }
    while (ready != T) { }
}
