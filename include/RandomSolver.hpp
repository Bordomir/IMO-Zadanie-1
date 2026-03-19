#pragma once

#include <random>

#include "Solver.hpp"
#include "DataLoader.hpp"

class RandomSolver : public Solver
{
public:
    std::mt19937 rng;

    RandomSolver(DataLoader &data, unsigned int seed) : Solver(data), rng(seed) {};
    RandomSolver(DataLoader &data) : Solver(data), rng(std::random_device{}()) {};
    void solve() override;

private:
    int randomInt(int min, int max);
};