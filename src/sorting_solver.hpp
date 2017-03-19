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
    void solve(const challenge &);

    thread t;
    atomic<bool> should_end;

private:
    bin bins[N_BINS];
    uint64_t *num_buf;
    char *solution_buf;
    unsigned int num_buf_len;
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
        bins[i].ptr = (uint64_t *)malloc(1024 * sizeof(*bins[i].ptr));
        assert(bins[i].ptr != NULL);
        bins[i].len = 0;
        bins[i].cap = 1024;
    }
    should_end = false;
    solution_buf = (char *)malloc((1024 * 20 + 1) * sizeof(*solution_buf));
    solution_buf_len = 1024 * 20 + 1;
    assert(solution_buf != NULL);
    num_buf = (uint64_t *)malloc(1024 * sizeof(*num_buf));
    num_buf_len = 1024;
    assert(num_buf != NULL);
    t_id = k;
}

void sorting_solver::loop()
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

void sorting_solver::solve(const challenge &c)
{
    mt19937_64 gen;
    buffer<32> hash;
    if (solution_buf_len < c.n * 20 + 1) {
        solution_buf_len = c.n * 20 + 1;
        solution_buf = (char *)realloc(solution_buf,
            solution_buf_len * sizeof(*solution_buf));
        assert(solution_buf != NULL);
    }
    if (num_buf_len < c.n) {
        num_buf_len = c.n;
        num_buf = (uint64_t *)realloc(num_buf, num_buf_len * sizeof(*num_buf));
        assert(num_buf != NULL);
    }
    char seed_str[64 + 20 + 1];
    strcpy(seed_str, (const char *)c.prev_hash.c);
    for (unsigned int i = 0; i < N_BINS; ++i) {
        if (bins[i].cap < c.n) {
            bins[i].ptr = (uint64_t *)realloc(bins[i].ptr, c.n * sizeof(*bins[i].ptr));
            assert(bins[i].ptr != NULL);
            bins[i].len = 0;
            bins[i].cap = c.n;
        }
    }
    unsigned int nonce = t_id * 10;
    unsigned int seed_len = num_to_str(nonce, seed_str + 64) + 64;

    for (;;) {
        sha256(seed_str, seed_len, hash);
        gen.seed(hash.l[0]);
        for (unsigned int i = 0; i < c.n; ++i) {
            uint64_t r = gen();
            unsigned int index = r >> BIN_SHIFT;
            bins[index].ptr[bins[index].len++] = r;
        }

        if (c.type == TYPE_SORTED_LIST) {
            uint64_t *cur_num_buf = num_buf;
            for (unsigned int i = 0; i < N_BINS; ++i) {
                fast_sort(bins[i].ptr, bins[i].len);
                memcpy(cur_num_buf, bins[i].ptr, bins[i].len *
                    sizeof(*bins[i].ptr));
                cur_num_buf += bins[i].len;
                bins[i].len = 0;
            }
        } else {
            uint64_t *cur_num_buf = num_buf;
            for (unsigned int i = N_BINS - 1; i < N_BINS; --i) {
                fast_sort_r(bins[i].ptr, bins[i].len);
                memcpy(cur_num_buf, bins[i].ptr, bins[i].len *
                    sizeof(*bins[i].ptr));
                cur_num_buf += bins[i].len;
                bins[i].len = 0;
            }
        }

        unsigned int len = nums_to_str(num_buf, c.n, solution_buf);

        sha256(solution_buf, len, hash);
        if (check_prefix(hash, c.prefix4, c.hash_prefix, c.hash_prefix_len)) {
            lock_guard<mutex> lk (mtx);
            if (should_end) {
                should_end = false;
                return;
            }

            our_solution.id = c.id;
            our_solution.nonce = nonce;
            buffer<65> str;
            hash_str(hash, str);
            memcpy(our_solution.hash, str.c, 65);
            new_solution = true;
            solution_cv.notify_one();
            return;
        }

        if (UNLIKELY(should_end)) {
            should_end = false;
            return;
        }
        ++nonce;
        assert(nonce > 0);
        if (seed_str[seed_len - 1] == '9') {
            nonce += 10 * (T - 1);
            seed_len = num_to_str(nonce, seed_str + 64) + 64;
        } else {
            ++seed_str[seed_len - 1];
        }
    }
}
