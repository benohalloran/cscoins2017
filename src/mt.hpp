#ifndef SOLVER_CPP_
#error "cannot be included"
#endif

constexpr unsigned int mt_vec_len = 4;

typedef uint64_t vuint64_t __attribute__((vector_size(mt_vec_len * 8)));

struct mt {

    static constexpr uint64_t w = 64lu;
    static constexpr uint64_t n = 312lu;
    static constexpr uint64_t m = 156lu;
    static constexpr uint64_t r = 31lu;
    static constexpr uint64_t a = 0xB5026F5AA96619E9lu;
    static constexpr uint64_t u = 29lu;
    static constexpr uint64_t d = 0x5555555555555555lu;
    static constexpr uint64_t s = 17lu;
    static constexpr uint64_t b = 0x71D67FFFEDA60000lu;
    static constexpr uint64_t t = 37lu;
    static constexpr uint64_t c = 0xFFF7EEE000000000lu;
    static constexpr uint64_t l = 43lu;
    static constexpr uint64_t f = 6364136223846793005lu;

    static constexpr uint64_t lower_mask = 0x7FFFFFFFlu;
    static constexpr uint64_t upper_mask = 0xffffffff80000000lu;

    mt();
    void seed(vuint64_t s);
    vuint64_t gen();
    void twist();

    vuint64_t MT[n];
    unsigned int index;
};

mt::mt()
{
    index = 0;
    for (unsigned int i = 0; i < n; ++i) {
        for (unsigned int j = 0; j < mt_vec_len; ++j) {
            MT[i][j] = 0;
        }
    }
}

void mt::seed(vuint64_t s)
{
    index = n;
    MT[0] = s;
    for (unsigned int i = 1; i < n; ++i) {
        MT[i] = f * (MT[i - 1] ^ (MT[i - 1] >> (w - 2))) + i;
    }
}

vuint64_t mt::gen()
{
    if (index == n) {
        twist();
    }

    vuint64_t x = MT[index];
    x ^= (x >> u) & d;
    x ^= (x << s) & b;
    x ^= (x << t) & c;
    x ^= x >> l;

    ++index;

    return x;
}

void mt::twist()
{
    for (unsigned int i = 0; i < n; ++i) {
        vuint64_t x = (MT[i] & upper_mask) + (MT[(i + 1) % n] & lower_mask);
        vuint64_t y = x >> 1;
        y ^= a * (x % 2);
        MT[i] = MT[(i + m) % n] ^ y;
    }

    index = 0;
}
