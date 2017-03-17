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
    mt gen;
    bin bins[VECTOR_LENGTH][N_BINS];
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
    for (unsigned int i = 0; i < VECTOR_LENGTH; ++i) {
        for (unsigned int j = 0; j < N_BINS; ++j) {
            bin *b = &bins[i][j];
            b->ptr = (uint64_t *)malloc(1024 * sizeof(*b->ptr));
            assert(b->ptr != NULL);
            b->len = 0;
            b->cap = 1024;
        }
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
    if (solution_buf_len < c.n * 20 + 1) {
        solution_buf_len = c.n * 20 + 1;
        solution_buf = (char *)realloc(solution_buf,
            solution_buf_len * sizeof(*solution_buf));
        assert(solution_buf != NULL);
    }
    char seed_str[VECTOR_LENGTH][64 + 20 + 1];
    for (unsigned int i = 0; i < VECTOR_LENGTH; ++i) {
        strcpy(seed_str[i], c.prev_hash);
    }
    for (unsigned int i = 0; i < VECTOR_LENGTH; ++i) {
        for (unsigned int j = 0; j < N_BINS; ++j) {
            bin *b = &bins[i][j];
            if (b->cap < c.n) {
                b->ptr = (uint64_t *)realloc(b->ptr, c.n * sizeof(*b->ptr));
                assert(b->ptr != NULL);
                b->len = 0;
                b->cap = c.n;
            }
        }
    }
    unsigned long nonce[VECTOR_LENGTH];
    unsigned int seed_len[VECTOR_LENGTH];
    for (unsigned int i = 0; i < VECTOR_LENGTH; ++i) {
        nonce[i] = t_id * VECTOR_LENGTH * 10 + i * 10;
        seed_len[i] = num_to_str(nonce[i], seed_str[i] + 64) + 64;
    }

    for (;;) {
        uint64_t seed[VECTOR_LENGTH];
        for (unsigned int i = 0; i < VECTOR_LENGTH; ++i) {
            sha256(seed_str[i], seed_len[i], hash);
            seed[i] = *(uint64_t *)hash;
        }
        gen.seed(seed);
        for (unsigned int i = 0; i < c.n; ++i) {
            uint64_t r[VECTOR_LENGTH];
            gen.gen(r);
            for (unsigned int j = 0; j < VECTOR_LENGTH; ++j) {
                unsigned int index = r[j] >> BIN_SHIFT;
                bin *b = &bins[j][index];
                b->ptr[b->len++] = r[j];
            }
        }

        for (unsigned int i = 0; i < VECTOR_LENGTH; ++i) {
            char *cur_solution_buf = solution_buf;

            if (c.type == TYPE_SORTED_LIST) {
                for (unsigned int j = 0; j < N_BINS; ++j) {
                    bin *b = &bins[i][j];
                    fast_sort(b->ptr, b->len);

                    for (unsigned int k = 0; k < b->len; ++k) {
                        cur_solution_buf += num_to_str(b->ptr[k],
                            cur_solution_buf);
                    }
                }
            } else {
                for (unsigned int j = N_BINS - 1; j < N_BINS; --j) {
                    bin *b = &bins[i][j];
                    fast_sort(b->ptr, b->len);

                    for (unsigned int k = 0; k < b->len; ++k) {
                        cur_solution_buf += num_to_str(b->ptr[k],
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
                our_solution.nonce = nonce[i];
                hash_str(hash, our_solution.hash);
                new_solution = true;
                solution_cv.notify_one();
                return;
            }
        }

        if (UNLIKELY(should_end)) {
            should_end = false;
            return;
        }
        for (unsigned int i = 0; i < VECTOR_LENGTH; ++i) {
            for (unsigned int j = 0; j < N_BINS; ++j) {
                bins[i][j].len = 0;
            }
        }
        for (unsigned int i = 0; i < VECTOR_LENGTH; ++i) {
            ++nonce[i];
            if (seed_str[i][seed_len[i] - 1] == '9') {
                nonce[i] += 10 * VECTOR_LENGTH * (T - 1);
                seed_len[i] = num_to_str(nonce[i], seed_str[i] + 64) + 64;
            } else {
                ++seed_str[i][seed_len[i] - 1];
            }
        }
    }
}
