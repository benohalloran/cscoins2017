#include <algorithm>
#include <cstdint>
#include <functional>

using namespace std;

void
fast_sort0(uint64_t *array, unsigned int len)
{
    sort(array, array + len);
}

void
fast_sort_r0(uint64_t *array, unsigned int len)
{
    sort(array, array + len, greater<uint64_t>());
}
