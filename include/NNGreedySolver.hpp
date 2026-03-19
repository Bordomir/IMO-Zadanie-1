#pragma once

#include <vector>
#include <limits>

#include "Solver.hpp"
#include "DataLoader.hpp"

class NNGreedySolver : public Solver
{
public:
    bool isConsideringProfits;
    int startNode;

    NNGreedySolver(DataLoader &data) : Solver(data), isConsideringProfits(true), startNode(rand() % data.numNodes) {};
    NNGreedySolver(DataLoader &data, int start) : Solver(data), isConsideringProfits(true), startNode(start) {};
    NNGreedySolver(DataLoader &data, bool considerProfits) : Solver(data), isConsideringProfits(considerProfits), startNode(rand() % data.numNodes) {};
    NNGreedySolver(DataLoader &data, bool considerProfits, int start) : Solver(data), isConsideringProfits(considerProfits), startNode(start) {};
    void solve() override;

private:
    vector<int> unvisitedNodes;

    template <typename ScoreFunction>
    inline void solvePhaseI(ScoreFunction scoreFunction);
    inline void solvePhaseII();
};

template <typename ScoreFunction>
inline void NNGreedySolver::solvePhaseI(ScoreFunction scoreFunction)
{
    while (!unvisitedNodes.empty())
    {
        const auto &distanceFromLastNode = data.distanceMatrix[solution.back()];

        int bestI = -1;
        int bestScore = numeric_limits<int>::min();
        for (size_t i = 0; i < unvisitedNodes.size(); i++)
        {
            int score = scoreFunction(unvisitedNodes[i], distanceFromLastNode);

            if (score > bestScore)
            {
                bestScore = score;
                bestI = i;
            }
        }

        solution.push_back(unvisitedNodes[bestI]);
        unvisitedNodes[bestI] = unvisitedNodes.back();
        unvisitedNodes.pop_back();
    }
}
