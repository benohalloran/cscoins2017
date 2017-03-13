#ifndef SOLVER_HPP_
#define SOLVER_HPP_ 1

#include <cstdint>
#include <string>

struct solution {
    unsigned int nonce;
    int id;
    char hash[65];
};

solution solve(const std::string &json);

#endif
