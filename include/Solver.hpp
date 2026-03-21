#pragma once

#include <vector>

#include "DataLoader.hpp"

class Solver
{
public:
    DataLoader &data;
    vector<int> solution;
    int solutionScore;

    Solver(DataLoader &data) : data(data), solution(), solutionScore(0) {};
    virtual void solve() = 0;
    void calculateScore();
    void print();
};
