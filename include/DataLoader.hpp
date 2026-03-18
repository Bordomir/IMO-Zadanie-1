#pragma once

#include <vector>
#include <string>

using namespace std;

class DataLoader
{
public:
    vector<int> nodeProfits;
    vector<vector<int>> distanceMatrix;

    DataLoader(const string &filename);
};
