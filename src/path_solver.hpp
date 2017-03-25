#ifndef SOLVER_CPP_
#error "cannot be included"
#endif

#define grid_val(g,n,r,c) ((g)[(r) * (n) + (c)])

struct path_solver {
    path_solver(path_solver &&);
    path_solver(unsigned int);
    void loop();

    thread t;
    atomic<bool> should_end;

private:
    void solve(const challenge &);
    void do_realloc(unsigned int, unsigned int);
    void fill_grid(uint64_t, unsigned int, unsigned int);
    unsigned int fill_solution_string(unsigned int);
    void submit_solution(int, unsigned long, const buffer<32> &);
    unsigned int dfs(unsigned int, unsigned int, unsigned int);
    unsigned int dfs2(int [][2], unsigned int, unsigned int, unsigned int);
    // unsigned int dfs4a(unsigned int, unsigned int, unsigned int);
    // unsigned int dfs4b(unsigned int, unsigned int, unsigned int);
    void print_grid(unsigned int, unsigned int);

    unsigned int *grid_orig, *grid;
    char *solution_buf;
    unsigned int *path;
    unsigned int solution_buf_len;
    unsigned int grid_size;
    unsigned int path_len, path_cap;
    unsigned int start_r, start_c, end_r, end_c;
    unsigned int t_id;
};

path_solver::path_solver(path_solver &&s)
{
    (void)s;
    assert(0);
}

path_solver::path_solver(unsigned int k)
{
    grid = NULL;
    grid_orig = NULL;
    path = NULL;
    grid_size = 0;
    path_len = 0;
    path_cap = 0;
    solution_buf = NULL;
    solution_buf_len = 0;
    do_realloc(1024, 4096);
    should_end = false;
    t_id = k;
}

void
path_solver::loop()
{
    set_affinity(t_id);
    ++ready;

    for (;;) {
        challenge c;
        unique_lock<mutex> lk (mtx);
        path_cv.wait(lk, [this] () {
            return new_path_challenge > 0;
        });
        should_end = false;
        --new_path_challenge;
        c = current_challenge;
        lk.unlock();
        assert(c.type == TYPE_SHORTEST_PATH);
        solve(c);
    }
}

void
path_solver::do_realloc(unsigned int n, unsigned int m)
{
    (void)m;
    if (solution_buf_len < n * 2 * 20 + 8) {
        solution_buf_len = n * 2 * 20 + 8;
        solution_buf = (char *)realloc(solution_buf,
            solution_buf_len * sizeof(*solution_buf));
        assert(solution_buf != NULL);
    }
    if (grid_size < n * n) {
        grid_size = n * n;
        grid = (unsigned int *)realloc(grid, grid_size * sizeof(*grid));
        assert(grid != NULL);
        grid_orig = (unsigned int *)realloc(grid_orig, grid_size *
            sizeof(*grid_orig));
        assert(grid_orig != NULL);
    }
    if (path_cap < (n + 1) * 2) {
        path_cap = (n + 1) * 2;
        path = (unsigned int *)realloc(path, path_cap * sizeof(*path));
        path_len = 0;
        assert(path != NULL);
    }
}

void
path_solver::print_grid(unsigned int n, unsigned int d)
{
    for (unsigned int r = 0; r < n; ++r) {
        for (unsigned int c = 0; c < n; ++c) {
            if (r == start_r &&  c == start_c) {
                printf("s");
            } else if (r == end_r && c == end_c) {
                printf("e");
            } else {
                for (unsigned int i = 0; i < d; ++i) {
                    if (path[i * 2] == r && path[i * 2 + 1] == c) {
                        printf("p");
                        goto next;
                    }
                }
                printf("%c", grid_val(grid, n, r, c) ? 'x' : ' ');
            next: ;
            }
        }
        fputc('\n', stdout);
    }
    fputc('\n', stdout);
    fflush(stdout);
}

void
path_solver::fill_grid(uint64_t seed, unsigned int n, unsigned int m)
{
    mt19937_64 gen (seed);

    memset(grid_orig + n, 0, ((n * n - n) * sizeof(*grid_orig)));
    memset(grid_orig, 1, n * sizeof(*grid_orig));
    memset(grid_orig - n, 1, n * sizeof(*grid_orig));

    for (unsigned int i = 0; i < n; ++i) {
        grid_val(grid_orig, n, i, 0) = 1;
        grid_val(grid_orig, n, i, n - 1) = 1;
    }

    do {
        start_r = gen() % n;
        start_c = gen() % n;
    } while (start_r == 0 || start_r == n - 1 || start_c == 0
        || start_c == n - 1);

    do {
        end_r = gen() % n;
        end_c = gen() % n;
    } while (end_r == 0 || end_r == n - 1 || end_c == 0 || end_c == n - 1
        || (start_r == end_r && start_c == end_c));

    for (unsigned int i = 0; i < m; ++i) {
        const unsigned int blocker_r = gen() % n;
        const unsigned int blocker_c = gen() % n;

        grid_val(grid_orig, n, blocker_r, blocker_c) = 1;
    }
}

unsigned int
path_solver::dfs2(int dirs[][2], unsigned int n, unsigned int depth,
    unsigned int max_depth)
{
    if (depth == max_depth) {
        return 0;
    }

    const unsigned int pos_r = path[depth * 2];
    const unsigned int pos_c = path[depth * 2 + 1];

    for (int i = 0; i < 2; ++i) {
        const unsigned int r = pos_r + dirs[i][0];
        const unsigned int c = pos_c + dirs[i][1];

        unsigned int v = grid_val(grid, n, r, c);
        if (v != 0) { //  && v - 1 <= depth) {
            continue;
        }

        const unsigned int nd = depth + 1;
        path[nd * 2] = r;
        path[nd * 2 + 1] = c;
        if (r == end_r && c == end_c) {
            return nd;
        }

        unsigned int d = dfs2(dirs, n, nd, max_depth);
        if (d > 0) {
            return d;
        }
    }

    grid_val(grid, n, pos_r, pos_c) = 1; // depth + 1;
    return 0;
}

unsigned int
path_solver::dfs(unsigned int n, unsigned int depth, unsigned int max_depth)
{
    static int dirs[4][2] = {
        {-1, 0}, {0, -1}, {0, 1}, {1, 0}
    };

    if (depth == max_depth) {
        return 0;
    }

    const unsigned int pos_r = path[depth * 2];
    const unsigned int pos_c = path[depth * 2 + 1];

    for (int i = 0; i < 4; ++i) {
        const unsigned int r = pos_r + dirs[i][0];
        const unsigned int c = pos_c + dirs[i][1];

        unsigned int v = grid_val(grid, n, r, c);
        if (v != 0 && v - 1 <= depth) {
            continue;
        }

        const unsigned int nd = depth + 1;
        path[nd * 2] = r;
        path[nd * 2 + 1] = c;
        if (r == end_r && c == end_c) {
            return nd;
        }

        unsigned int d = dfs(n, nd, max_depth);
        if (d > 0) {
            return d;
        }
    }

    grid_val(grid, n, pos_r, pos_c) = depth + 1;
    return 0;
}

unsigned int
path_solver::fill_solution_string(unsigned int n)
{
    char *cur_solution_buf = solution_buf;
    path[0] = start_r;
    path[1] = start_c;

    unsigned int len = (start_r > end_r ? start_r - end_r : end_r - start_r) +
        (start_c > end_c ? start_c - end_c : end_c - start_c);
    int diff[2] = {start_r > end_r ? -1 : start_r == end_r ? 0 : 1, start_c >
        end_c ? -1 : start_c == end_c ? 0 : 1};
    int dirs[2][2];

    if (diff[0] < 0 && diff[1] < 0) {
        dirs[0][0] = -1;
        dirs[0][1] = 0;
        dirs[1][0] = 0;
        dirs[1][1] = -1;
    } else if (diff[0] < 0 && diff[1] >= 0) {
        dirs[0][0] = -1;
        dirs[0][1] = 0;
        dirs[1][0] = 0;
        dirs[1][1] = 1;
    } else if (diff[0] >= 0 && diff[1] < 0) {
        dirs[0][0] = 0;
        dirs[0][1] = -1;
        dirs[1][0] = 1;
        dirs[1][1] = 0;
    } else if (diff[0] >= 0 && diff[1] >= 0) {
        dirs[0][0] = 0;
        dirs[0][1] = 1;
        dirs[1][0] = 1;
        dirs[1][1] = 0;
    }

    for (unsigned int d = len; d <= len; d += 2) {
        memcpy(grid, grid_orig, n * n * sizeof(*grid));
        // print_grid(n, 0);
        unsigned int k = dfs2(dirs, n, 0, d);
        assert(k == 0 || k == d);
        if (k == 0) {
            continue;
        }

        // print_grid(n, d);

        for (unsigned int i = 0; i <= d; ++i) {
            cur_solution_buf += num_to_str_sm(path[i * 2], cur_solution_buf);
            cur_solution_buf += num_to_str_sm(path[i * 2 + 1], cur_solution_buf);
        }
        break;
    }

    return cur_solution_buf - solution_buf;
}

void
path_solver::submit_solution(int id, unsigned long nonce,
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
    end_all_solvers();
    solution_cv.notify_one();
}

void
path_solver::solve(const challenge &c)
{
    buffer<32> hash;
    char seed_str[64 + 20 + 8];
    unsigned long nonce;
    unsigned int seed_len;

    nonce = 1000 * (t_id + TEAM_ID * T + 1);
    strcpy(seed_str, (const char *)c.prev_hash.c);
    seed_len = num_to_str(nonce, seed_str + 64) + 64;
    do_realloc(c.n, c.m);

    for (;;) {
        unsigned int len;

        sha256(seed_str, seed_len, hash);
        fill_grid(hash.l[0], c.n, c.m);

        len = fill_solution_string(c.n);
        if (len == 0) {
            goto skip;
        }
        sha256(solution_buf, len, hash);
        if (check_prefix(hash, c.prefix4, c.hash_prefix, c.hash_prefix_len)) {
            submit_solution(c.id, nonce, hash);
            return;
        }
    skip:

        if (UNLIKELY(should_end)) {
            should_end = false;
            submit_solution(-1, -1, hash);
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
