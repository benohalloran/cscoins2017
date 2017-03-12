#include "solver.hpp"
#include "sha256.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#define TYPE_SORTED_LIST            1
#define TYPE_REVERSE_SORTED_LIST    2

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

string solve_sorted_list(vector<uint64_t> &nums)
{
    sort(nums.begin(), nums.end());
    string str;
    for (size_t i = 0; i < nums.size(); ++i) {
        str += to_string(nums[i]);
    }
    return str;
}

string solve_reverse_sorted_list(vector<uint64_t> &nums)
{
    sort(nums.begin(), nums.end(), std::greater<uint64_t>());
    string str;
    for (size_t i = 0; i < nums.size(); ++i) {
        str += to_string(nums[i]);
    }
    return str;
}

solution solve(const challenge &c)
{
    char mem[65 + 8];
    char *hash = mem + 8 - (size_t)mem % 8;
    std::vector<uint64_t> nums (c.n);
    const size_t prefix_len = strlen(c.hash_prefix);

    for (unsigned int nonce = 0; ; ++nonce) {
        char seed_str[64 + 20 + 1];
        size_t len = sprintf(seed_str, "%s%u", c.prev_hash, nonce);
        sha256(seed_str, len, hash);
        uint64_t seed = *(uint64_t *)hash;
        mt19937_64 gen (seed);
        for (int i = 0; i < c.n; ++i) {
            nums[i] = gen();
        }

        string sol;
        switch (c.type) {
        case TYPE_SORTED_LIST:
            sol = solve_sorted_list(nums);
            break;

        case TYPE_REVERSE_SORTED_LIST:
            sol = solve_reverse_sorted_list(nums);
            break;
        }

        sha256(sol.c_str(), sol.size(), hash);
        if (!strncmp(hash, c.hash_prefix, prefix_len)) {
            solution s;
            s.id = c.id;
            s.nonce = nonce;
            strcpy(s.hash, hash);
            return s;
        }
    }
}

}

solution solve(const std::string &json)
{
    return solve(parse(json));
}
