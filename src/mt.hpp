#ifndef SOLVER_CPP_
#error "cannot be included"
#endif

#if VECTOR_LENGTH == 1
typedef union {
    uint64_t v;
    uint64_t a[1];
} vuint64_t;
#else
typedef uint64_t vnul __attribute__((vector_size(VECTOR_LENGTH *
            sizeof(uint64_t))));
typedef union {
    vnul v;
    uint64_t a[VECTOR_LENGTH];
} vuint64_t;
#endif

inline vuint64_t init_vector(const uint64_t *p)
{
    vuint64_t y;
    memcpy(&y.a, p, sizeof(y.a));
    return y;
}

inline void extract_vector(vuint64_t v, uint64_t *p)
{
    memcpy(p, &v.a, sizeof(v.a));
}

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
    void seed(const uint64_t *s);
    void gen(uint64_t *r);
    void twist();

    vuint64_t MT[n];
    unsigned int index;
};

mt::mt()
{
    uint64_t x[VECTOR_LENGTH] = {0};
    vuint64_t z = init_vector(x);
    for (unsigned int i = 0; i < n; ++i) {
        MT[i] = z;
    }
    index = 0;
}

void mt::seed(const uint64_t *s)
{
    vuint64_t v = init_vector(s);
    MT[0].v = v.v;
    for (unsigned int i = 1; i < n; ++i) {
        v.v = f * (v.v ^ (v.v >> (w - 2))) + i;
        MT[i].v = v.v;
    }
    twist();
}

void mt::gen(uint64_t *r)
{
    if (UNLIKELY(index == n)) {
        twist();
    }

    vuint64_t x = MT[index];
    x.v ^= (x.v >> u) & d;
    x.v ^= (x.v << s) & b;
    x.v ^= (x.v << t) & c;
    x.v ^= x.v >> l;

    ++index;
    extract_vector(x, r);
}

void mt::twist()
{
    for (unsigned int i = 0; i < n; ++i) {
        vuint64_t x;
        x.v = (MT[i].v & upper_mask) + (MT[(i + 1) % n].v & lower_mask);
        MT[i].v = MT[(i + m) % n].v ^ ((x.v >> 1) ^ (a * (x.v % 2)));
    }

    index = 0;
}
