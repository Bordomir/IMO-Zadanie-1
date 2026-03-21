#include "include/GreedySolver.hpp"

#include <format>
#include <vector>
#include <numeric>
#include <limits>
#include <list>

#include "include/Common.hpp"

using namespace std;

string GreedySolver::getAlgorithmName()
{
    string typeName;
    switch (type)
    {
    case GreedyType::NearestNeighbour:
        typeName = "NN";
        break;
    
    case GreedyType::GreedyCycle:
        typeName = "GC";
        break;
    }
    string isConsideringProfitsName = isConsideringProfits ? "withProfits" : "noProfits";
    return format("Greedy_{}_{}", typeName, isConsideringProfitsName);
}
void GreedySolver::solve()

{
    // ustawienie aktualnego rozwiązania na zawierający jedynie startowy wierzchołek
    solution.clear();
    solution.reserve(data->numNodes);
    solution.push_back(startNode);

    // utworzenie tablicy nieodwiedzonych wierzchołków
    unvisitedNodes = vector<int>(data->numNodes);
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
    common::improveByRemovingNodes(solution, data);
}
