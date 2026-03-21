#include "include/GreedySolver.hpp"

#include <vector>
#include <numeric>
#include <limits>
#include <list>

using namespace std;

void GreedySolver::solve()
{
    // ustawienie aktualnego rozwiązania na zawierający jedynie startowy wierzchołek
    solution.clear();
    solution.reserve(data.numNodes);
    solution.push_back(startNode);

    // utworzenie tablicy nieodwiedzonych wierzchołków
    unvisitedNodes = vector<int>(data.numNodes);
    iota(unvisitedNodes.begin(), unvisitedNodes.end(), 0);
    swapAndPop(startNode);

    // wybór typu algorytmu
    switch (type)
    {
    case GreedyType::NearestNeighbour:
        solvePhaseI([&](auto scoreFunction)
            { solveNN(scoreFunction); });
        break;
    case GreedyType::GreedyCycle:
        solvePhaseI([&](auto scoreFunction)
            { solveGC(scoreFunction); });
        break;
    }

    // wykonanie fazy II
    solvePhaseII();

    // wyliczanie wyniku rozwiązania
    calculateScore();
}

// usuwa wierzchołek o podanym indeksie z tablicy nieodwiedzonych wierzchołków
// w czasie O(1) bez zachowania porządku elementów
void GreedySolver::swapAndPop(int index)
{
    unvisitedNodes[index] = unvisitedNodes.back();
    unvisitedNodes.pop_back();
}

void GreedySolver::solvePhaseII()
{
    while (solution.size() > 1)
    {
        int bestImprovement = -1;
        int bestI = -1;
        int n = solution.size();
        for (int i = 0; i < n; i++)
        {
            // uzyskanie sąsiadujących elementów z uwzględnieniem cyklu
            int prev = solution[i == 0 ? n - 1 : i - 1];
            int curr = solution[i];
            int next = solution[i == n - 1 ? 0 : i + 1];
            // wyliczenie poprawy po usunęciu wierzchołka
            // + odległość prev -> curr
            int improvement = data.distanceMatrix[prev][curr];
            // - odległość prev -> next
            improvement -= data.distanceMatrix[prev][next];
            // + odległość curr -> next
            improvement += data.distanceMatrix[curr][next];
            // - zysk z odwiedzenia wierzchołka
            improvement -= data.nodeProfits[curr];
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
