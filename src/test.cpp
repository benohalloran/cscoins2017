#ifdef TESTING

#include "solver.hpp"

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <thread>

void test_solver();

// const char *sorted_list_input = "
//     "challenge_id": 0,
//     "challenge_name": "sorted_list",
//     "last_solution_hash": "0000000000000000000000000000000000000000000000000000000000000000",
//     "hash_prefix": "94f9",
//     "parameters": {
//         "nb_elements": 20
//     }
// }
// """
//
// reverse_sorted_list_input = """
// {
//     "challenge_id": 1,
//     "challenge_name": "reverse_sorted_list",
//     "last_solution_hash": "0000000000000000000000000000000000000000000000000000000000000000",
//     "hash_prefix": "94f9",
//     "parameters": {
//         "nb_elements": 20
//     }
// }
// """
//
// shortest_path_input = """
// {
//     "challenge_id": 2,
//     "challenge_name": "shortest_path",
//     "last_solution_hash": "0000000000000000000000000000000000000000000000000000000000000000",
//     "hash_prefix": "94f8",
//     "parameters": {
//         "grid_size": 25,
//         "nb_blockers": 80
//     }
// }
// """

int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    init_solver();
    char input[4096] = {0};
    std::cin.read(input, 4096);
    solution s = solve(input);
    printf("%d %lu %s\n", s.id, s.nonce, s.hash);
    _exit(0);
}

#endif
