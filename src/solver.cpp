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

#include "rapidjson/document.h"

#include <sched.h>

#define TYPE_SORTED_LIST            1
#define TYPE_REVERSE_SORTED_LIST    2

#define N_BINS 256
#define BIN_SHIFT 56

#define MAX_NONCE 100000000

namespace {

using namespace std;
using namespace rapidjson;

struct challenge {
    int id, n, type;
    char hash_prefix[65];
    char prev_hash[65];
};

challenge parse(const string &json)
{
    Document d;
    challenge c;

    d.Parse(json.c_str());
    assert(d.IsObject());

    assert(d.HasMember("challenge_id"));
    const Value &id = d["challenge_id"];
    assert(id.IsInt());
    c.id = id.GetInt();

    assert(d.HasMember("challenge_name"));
    const Value &name = d["challenge_name"];
    assert(name.IsString());
    const char *name_str = name.GetString();
    if (!strcmp(name_str, "sorted_list")) {
        c.type = TYPE_SORTED_LIST;
    } else if (!strcmp(name_str, "reverse_sorted_list")) {
        c.type = TYPE_REVERSE_SORTED_LIST;
    } else {
        assert(0);
    }

    assert(d.HasMember("last_solution_hash"));
    const Value &hash = d["last_solution_hash"];
    assert(hash.IsString());
    const char *hash_str = hash.GetString();
    assert(strlen(hash_str) == 64);
    for (unsigned int i = 0; i < 64; ++i) {
        assert((hash_str[i] >= '0' && hash_str[i] <= '9')
            || (hash_str[i] >= 'a' && hash_str[i] <= 'f'));
    }
    strcpy(c.prev_hash, hash_str);

    assert(d.HasMember("hash_prefix"));
    const Value &hash_prefix = d["hash_prefix"];
    assert(hash_prefix.IsString());
    const char *hash_prefix_str = hash_prefix.GetString();
    assert(strlen(hash_prefix_str) <= 64);
    for (unsigned int i = 0; i < strlen(hash_prefix_str); ++i) {
        assert((hash_prefix_str[i] >= '0' && hash_prefix_str[i] <= '9')
            || (hash_prefix_str[i] >= 'a' && hash_prefix_str[i] <= 'f'));
    }
    strcpy(c.hash_prefix, hash_prefix_str);

    assert(d.HasMember("parameters"));
    const Value &parameters = d["parameters"];
    assert(parameters.IsObject());

    assert(parameters.HasMember("nb_elements"));
    const Value &nb_elements = parameters["nb_elements"];
    assert(nb_elements.IsInt());
    c.n = nb_elements.GetInt();
    assert(c.n >= 1);

    return c;
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
