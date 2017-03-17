#ifdef TESTING

#include "solver.hpp"

#include <iostream>
#include <cstdio>
#include <unistd.h>

void test_solver();

int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    init_solver();
    test_solver();
    // char input[4096] = {0};
    // std::cin.read(input, 4096);
    // solution s = solve(input);
    // printf("%d %u %s\n", s.id, s.nonce, s.hash);
    _exit(0);
}

#endif
