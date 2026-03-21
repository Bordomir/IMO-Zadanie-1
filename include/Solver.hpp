#pragma once

#include <vector>
#include <string>

#include "DataLoader.hpp"

using namespace std;

class Solver
{
public:
    DataLoader *data;
    vector<int> solution;
    int solutionScore;

    Solver(DataLoader &data) : data(&data), solution(), solutionScore(0) {};
    virtual string getAlgorithmName() = 0;
    virtual void solve() = 0;
    void calculateScore();
    void print();
    void saveToFile(const string &filename = "");
};
