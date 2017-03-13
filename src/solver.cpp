#include "solver.hpp"
#include "sha256.hpp"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include <sched.h>

#define TYPE_SORTED_LIST            1
#define TYPE_REVERSE_SORTED_LIST    2

#define N_BINS 256
#define BIN_SHIFT 56

#define MAX_NONCE 100000000

namespace {

using namespace std;

struct challenge {
    int id, n, type;
    char hash_prefix[65];
    char prev_hash[65];
};

challenge parse(const string &json)
{
    size_t m = 0;
    bool copy = false;
    challenge ch;

    memset(ch.hash_prefix, 0, sizeof(ch.hash_prefix));

    string str;
    str.reserve(json.size());

    for (size_t i = 0; i < json.size(); ++i) {
        const char c = json[i];

        switch (c) {
        case ':':
            copy = true;
            break;

        case'{':
        case '}':
        case ',':
            switch (m++) {
            case 1:
                ch.id = atoi(str.c_str());
                break;

            case 2:
                if (str == "sortedlist") {
                    ch.type = TYPE_SORTED_LIST;
                } else if (str == "reversesortedlist") {
                    ch.type = TYPE_REVERSE_SORTED_LIST;
                }
                break;

            case 3:
                strcpy(ch.prev_hash, str.c_str());
                break;

            case 4:
                strcpy(ch.hash_prefix, str.c_str());
                break;

            case 6:
                ch.n = atoi(str.c_str());
                break;
            }

            copy = false;
            str = "";
            break;

        default:
            if (copy && ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))) {
                str += c;
            }
        }
    }

    return ch;
}

unsigned int
num_to_str(uint64_t n, char *buf)
{
    if (n < 10) {
        buf[0] = n + '0';
        buf[1] = '\0';
        return 1;
    }

    buf[20] = '\0';

    int i;
    for (i = 19; n && i >= 0; --i) {
        const uint64_t m = n % 10;
        n /= 10;
        buf[i] = m + '0';
    }

    for (int j = 0; j < 20 - i; ++j) {
        buf[j] = buf[j + i + 1];
    }

    return 19 - i;
}

void solve_sorted_list(const challenge &c, unsigned int start_nonce,
    solution *s, atomic<bool> *done)
{
    unsigned char mem[32 + 8];
    unsigned char *hash = mem + 8 - (size_t)mem % 8;
    vector<uint64_t> bins[N_BINS];
    const size_t prefix_len = strlen(c.hash_prefix);
    char seed_str[64 + 20 + 1];
    vector<char> solution_mem (c.n * 20 + 1);
    char *solution_buf = solution_mem.data(),
         *cur_solution_buf = solution_buf;

    strcpy(seed_str, c.prev_hash);

    for (auto &v : bins) {
        v.reserve(c.n);
    }

    mt19937_64 gen;
    unsigned int nonce = start_nonce;
    unsigned int seed_len = num_to_str(nonce, seed_str + 64) + 64;

    for (;;) {
        sha256(seed_str, seed_len, hash);
        uint64_t seed = *(uint64_t *)hash;
        gen.seed(seed);
        for (int i = 0; i < c.n; ++i) {
            uint64_t r = gen();
            bins[r >> BIN_SHIFT].push_back(r);
        }

        if (c.type == TYPE_SORTED_LIST) {
            for (int i = 0; i < N_BINS; ++i) {
                if (bins[i].empty()) {
                    continue;
                }

                sort(bins[i].begin(), bins[i].end());

                for (uint64_t d : bins[i]) {
                    cur_solution_buf += num_to_str(d, cur_solution_buf);
                }
            }
        } else {
            for (int i = N_BINS - 1; i >= 0; --i) {
                if (bins[i].empty()) {
                    continue;
                }

                sort(bins[i].begin(), bins[i].end(), greater<uint64_t>());

                for (uint64_t d : bins[i]) {
                    cur_solution_buf += num_to_str(d, cur_solution_buf);
                }
            }
        }

        sha256(solution_buf, cur_solution_buf - solution_buf, hash);
        if (check_prefix(hash, c.hash_prefix, prefix_len)) {
            if (*done) {
                return;
            }
            s->id = c.id;
            s->nonce = nonce;
            hash_str(hash, s->hash);
            *done = true;
            return;
        }

        if (*done) {
            break;
        }
        cur_solution_buf = solution_buf;
        for (int i = 0; i < N_BINS; ++i) {
            bins[i].clear();
        }
        ++nonce;
        if (seed_str[seed_len - 1] == '9') {
            seed_len = num_to_str(nonce, seed_str + 64) + 64;
        } else {
            ++seed_str[seed_len - 1];
        }
    }
}

void solve(const challenge &c, unsigned int start_nonce,
    solution *s, atomic<bool> *done)
{
    switch (c.type) {
    case TYPE_SORTED_LIST:
    case TYPE_REVERSE_SORTED_LIST:
        solve_sorted_list(c, start_nonce, s, done);
        break;

    default:
        assert(0);
    }
}

void set_affinity(int cpu)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    int ret = sched_setaffinity(0, sizeof(cpu_set_t), &set);
    assert(ret == 0);
}

solution solve(const challenge &c)
{
    const unsigned int T = thread::hardware_concurrency() - 1;

    if (T == 0 || strlen(c.hash_prefix) <= 3) {
        solution s;
        atomic<bool> done {false};
        solve(c, 0, &s, &done);
        return s;
    }

    vector<thread> threads (T);
    vector<atomic<bool>> dones (T);
    vector<solution> solutions (T);

    for (unsigned int i = 0; i < T; ++i) {
        dones[i] = false;
        threads[i] = thread([=] (const challenge &c, solution *s, atomic<bool> *done) {
            set_affinity(i);
            solve(c, MAX_NONCE / T * i, s, done);
        }, c, &solutions[i], &dones[i]);
    }
    set_affinity(T);

    unsigned int index;
    for (;;) {
        for (unsigned int i = 0; i < T; ++i) {
            if (dones[i]) {
                index = i;
                goto done;
            }
        }
    }

done:
    for (unsigned int i = 0; i < T; ++i) {
        dones[i] = true;
        threads[i].detach();
    }

    return solutions[index];
}

}

solution solve(const std::string &json)
{
    return solve(parse(json));
}
