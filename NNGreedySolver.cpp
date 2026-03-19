#include "include/NNGreedySolver.hpp"

#include <vector>
#include <numeric>
#include <limits>
#include <list>

using namespace std;

// Zaimplementować algorytmy zachłanne na bazie najbliższego sąsiada (NN).
// Najpierw w fazie I budujemy cykl Hamiltona na wszystkich wierzchołkach,
// a następnie w fazie II usuwamy wierzchołek dający największą poprawę zysku tak długo, aż przynosi to
// poprawę. W fazie I należy rozważyć opcję, w której dodając wierzchołek nie uwzględniamy
// jego zysku (NNa, GCa) i taką ,w której zysk z wierzchołka uwzględniamy.

void NNGreedySolver::solve()
{
    solution.clear();
    solution.reserve(data.numNodes);
    solution.push_back(startNode);

    unvisitedNodes = vector<int>(data.numNodes);
    iota(unvisitedNodes.begin(), unvisitedNodes.end(), 0);
    unvisitedNodes[startNode] = unvisitedNodes.back();
    unvisitedNodes.pop_back();

    if (isConsideringProfits)
        solvePhaseI([&](int node, const auto &dists)
                    { return data.nodeProfits[node] - dists[node]; });
    else
        solvePhaseI([&](int node, const auto &dists)
                    { return -dists[node]; });

    solvePhaseII();
}

inline void NNGreedySolver::solvePhaseII()
{
    while (solution.size() > 1)
    {
        int bestImprovement = -1;
        int bestI = -1;
        int n = solution.size();
        for (int i = 0; i < n; i++)
        {
            int prev = solution[i == 0 ? n - 1 : i - 1];
            int curr = solution[i];
            int next = solution[i == n - 1 ? 0 : i + 1];
            int improvement = data.distanceMatrix[prev][curr] + data.distanceMatrix[curr][next] - data.distanceMatrix[prev][next] - data.nodeProfits[curr];
            if (improvement > bestImprovement)
            {
                bestImprovement = improvement;
                bestI = i;
            }
        }

        if (bestImprovement <= 0)
        {
            break;
        }
        solution.erase(solution.begin() + bestI);
    }
}
