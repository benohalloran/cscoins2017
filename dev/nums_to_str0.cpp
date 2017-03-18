#include <cstdint>
#include <cstring>
#include <string>

#define RESTRICT __restrict__
#define NOINLINE __attribute__((__noinline__))

using namespace std;

unsigned int NOINLINE
nums_to_str0(const uint64_t * RESTRICT nums, unsigned int n,
    char * RESTRICT buf)
{
    string s;
    for (unsigned int i = 0; i < n; ++i) {
        s += to_string(nums[i]);
    }
    memcpy(buf, s.c_str(), s.size());
    return s.size();
}

void
init_decimals0()
{ }
