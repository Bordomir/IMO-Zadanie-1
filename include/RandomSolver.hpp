#pragma once

#include <random>

#include "Solver.hpp"
#include "DataLoader.hpp"

class RandomSolver : public Solver
{
public:
    std::mt19937 rng;

    RandomSolver(DataLoader &data) : Solver(data), rng(std::random_device{}()) {};
    RandomSolver(DataLoader &data, unsigned int seed) : Solver(data), rng(seed) {};
    void solve() override;

private:
    inline int randomInt(int min, int max);
};