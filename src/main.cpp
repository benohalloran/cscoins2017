#include "solver.hpp"

#include <iostream>
#include <cstdio>

int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    char input[4096] = {0};
    std::cin.read(input, 4096);
    solution s = solve(input);
    printf("%d %u %s\n", s.id, s.nonce, s.hash);
}
