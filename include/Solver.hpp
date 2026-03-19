#pragma once

#include <vector>

#include "DataLoader.hpp"

class Solver
{
public:
    DataLoader &data;
    vector<int> solution;
    int solutionScore;
    
    Solver(DataLoader &data) : data(data) {};
    virtual void solve() = 0;
};
