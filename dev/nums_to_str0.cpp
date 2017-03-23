#include <cstdint>
#include <cstring>
#include <string>

#define RESTRICT __restrict__
#define NOINLINE __attribute__((__noinline__))

using namespace std;

unsigned int
num_to_str0(uint64_t n, char *buf)
{
    string s = to_string(n);
    memcpy(buf, s.c_str(), s.size());
    return s.size();
}

unsigned int NOINLINE
nums_to_str0(const uint64_t * RESTRICT nums, unsigned int n,
    char * RESTRICT buf)
{
    char *p = buf;
    for (unsigned int i = 0; i < n; ++i) {
        p += num_to_str0(nums[i], p);
    }
    *p = '\0';
    return p - buf;
}

void
init_decimals0()
{ }
