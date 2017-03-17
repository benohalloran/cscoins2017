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
    mt19937_64 gen;
    bin bins[N_BINS];
    atomic<bool> should_end;
    char *solution_buf;
    unsigned char *hash;
    unsigned int solution_buf_len;
    unsigned int t_id;
    unsigned char mem[32 + 8];
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
    hash = mem + 8 - (size_t)mem % 8;
    t_id = k;
}

void sorting_solver::loop()
{
    set_affinity(t_id);

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
    if (solution_buf_len < c.n * 20 + 1) {
        solution_buf_len = c.n * 20 + 1;
        solution_buf = (char *)realloc(solution_buf,
            solution_buf_len * sizeof(*solution_buf));
        assert(solution_buf != NULL);
    }
    char seed_str[64 + 20 + 1];
    strcpy(seed_str, c.prev_hash);
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
        uint64_t seed = *(uint64_t *)hash;
        gen.seed(seed);
        for (unsigned int i = 0; i < c.n; ++i) {
            uint64_t r = gen();
            unsigned int index = r >> BIN_SHIFT;
            bins[index].ptr[bins[index].len++] = r;
        }
        char *cur_solution_buf = solution_buf;

        if (c.type == TYPE_SORTED_LIST) {
            for (unsigned int i = 0; i < N_BINS; ++i) {
                fast_sort(bins[i].ptr, bins[i].len);

                for (unsigned int j = 0; j < bins[i].len; ++j) {
                    cur_solution_buf += num_to_str(bins[i].ptr[j],
                        cur_solution_buf);
                }
            }
        } else {
            for (unsigned int i = N_BINS - 1; i < N_BINS; --i) {
                fast_sort(bins[i].ptr, bins[i].len);

                for (unsigned int j = bins[i].len - 1; j < bins[i].len; --j) {
                    cur_solution_buf += num_to_str(bins[i].ptr[j],
                        cur_solution_buf);
                }
            }
        }

        sha256(solution_buf, cur_solution_buf - solution_buf, hash);
        if (check_prefix(hash, c.prefix4, c.hash_prefix, c.hash_prefix_len)) {
            lock_guard<mutex> lk (mtx);
            if (should_end) {
                should_end = false;
                return;
            }

            our_solution.id = c.id;
            our_solution.nonce = nonce;
            hash_str(hash, our_solution.hash);
            new_solution = true;
            solution_cv.notify_one();
            return;
        }

        if (UNLIKELY(should_end)) {
            should_end = false;
            return;
        }
        for (unsigned int i = 0; i < N_BINS; ++i) {
            bins[i].len = 0;
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
