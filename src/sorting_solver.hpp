#ifndef SOLVER_CPP_
#error "cannot be included"
#endif

struct bin {
    uint64_t *ptr;
    unsigned int len, cap;
};

struct sorting_solver {
    sorting_solver(sorting_solver &&);
    sorting_solver(unsigned int);
    void loop();

    thread t;
    atomic<bool> should_end;

private:
    void solve(const challenge &);
    void do_realloc(unsigned int);
    void fill_bins(uint64_t, unsigned int);
    unsigned int fill_solution_string(int);
    void submit_solution(int, unsigned long, const buffer<32> &);

    bin bins[N_BINS];
    char *solution_buf;
    unsigned int solution_buf_len;
    unsigned int t_id;
};

sorting_solver::sorting_solver(sorting_solver &&s)
{
    (void)s;
    assert(0);
}

sorting_solver::sorting_solver(unsigned int k)
{
    for (unsigned int i = 0; i < N_BINS; ++i) {
        bins[i].ptr = NULL;
        bins[i].cap = 0;
    }
    solution_buf = NULL;
    solution_buf_len = 0;
    do_realloc(1024);
    should_end = false;
    t_id = k;
}

void
sorting_solver::loop()
{
    set_affinity(t_id);
    ++ready;

    for (;;) {
        challenge c;
        unique_lock<mutex> lk (mtx);
        sorting_cv.wait(lk, [this] () {
            return new_sorting_challenge > 0;
        });
        should_end = false;
        --new_sorting_challenge;
        c = current_challenge;
        lk.unlock();
        assert(c.type == TYPE_SORTED_LIST
            || c.type == TYPE_REVERSE_SORTED_LIST);
        solve(c);
    }
}

void
sorting_solver::do_realloc(unsigned int n)
{
    if (solution_buf_len < n * 20 + 8) {
        solution_buf_len = n * 20 + 8;
        solution_buf = (char *)realloc(solution_buf,
            solution_buf_len * sizeof(*solution_buf));
        assert(solution_buf != NULL);
    }
    for (unsigned int i = 0; i < N_BINS; ++i) {
        if (bins[i].cap < 2 * n) {
            bins[i].ptr = (uint64_t *)realloc(bins[i].ptr, 2 * n *
                sizeof(*bins[i].ptr));
            assert(bins[i].ptr != NULL);
            bins[i].len = 0;
            bins[i].cap = 2 * n;
        }
    }
}

void
sorting_solver::fill_bins(uint64_t seed, unsigned int n)
{
    mt19937_64 gen (seed);
    for (unsigned int i = 0; i < n; ++i) {
        const uint64_t r = gen();
        const unsigned int index = r >> BIN_SHIFT;
        bins[index].ptr[bins[index].len++] = r;
    }
}

unsigned int
sorting_solver::fill_solution_string(int type)
{
    char *cur_solution_buf = solution_buf;

    if (type == TYPE_SORTED_LIST) {
        for (unsigned int i = 0; i < N_BINS; ++i) {
            const unsigned int len = bins[i].len;
            uint64_t * const ptr = bins[i].ptr;
            fast_sort(ptr, len);
            for (unsigned int j = 0; j < len; ++j) {
                cur_solution_buf += num_to_str(ptr[j], cur_solution_buf);
            }
            bins[i].len = 0;
        }
    } else {
        for (unsigned int i = N_BINS - 1; i < N_BINS; --i) {
            const unsigned int len = bins[i].len;
            uint64_t * const ptr = bins[i].ptr;
            fast_sort(ptr, len);
            for (unsigned int j = len - 1; j < len; --j) {
                cur_solution_buf += num_to_str(ptr[j], cur_solution_buf);
            }
            bins[i].len = 0;
        }
    }

    return cur_solution_buf - solution_buf;
}

void
sorting_solver::submit_solution(int id, unsigned long nonce,
    const buffer<32> &hash)
{
    lock_guard<mutex> lk (mtx);
    buffer<65> str;

    if (should_end) {
        should_end = false;
        return;
    }

    our_solution.id = id;
    our_solution.nonce = nonce;
    hash_str(hash, str);
    memcpy(our_solution.hash, str.c, 65);
    new_solution = true;
    solution_cv.notify_one();
}

void
sorting_solver::solve(const challenge &c)
{
    buffer<32> hash;
    char seed_str[64 + 20 + 8];
    unsigned long nonce;
    unsigned int seed_len;

    nonce = 1000 * (t_id + TEAM_ID * T + 1);
    strcpy(seed_str, (const char *)c.prev_hash.c);
    seed_len = num_to_str(nonce, seed_str + 64) + 64;
    do_realloc(c.n);

    for (;;) {
        unsigned int len;

        sha256(seed_str, seed_len, hash);
        fill_bins(hash.l[0], c.n);

        len = fill_solution_string(c.type);
        sha256(solution_buf, len, hash);
        if (check_prefix(hash, c.prefix4, c.hash_prefix, c.hash_prefix_len)) {
            submit_solution(c.id, nonce, hash);
            return;
        }

        if (UNLIKELY(should_end)) {
            should_end = false;
            return;
        }
        ++nonce;
        if (seed_str[seed_len - 1] != '9') {
            ++seed_str[seed_len - 1];
        } else if (seed_str[seed_len - 2] != '9') {
            seed_str[seed_len - 1] = '0';
            ++seed_str[seed_len - 2];
        } else if (seed_str[seed_len - 3] != '9') {
            seed_str[seed_len - 1] = '0';
            seed_str[seed_len - 2] = '0';
            ++seed_str[seed_len - 3];
        } else {
            nonce += 1000 * (T * TEAMS - 1);
            seed_len = num_to_str(nonce, seed_str + 64) + 64;
        }
    }
}
