#pragma once

#include <random>
#include <vector>
#include <limits>

#include "Solver.hpp"
#include "DataLoader.hpp"

enum class GreedyType
{
    NearestNeighbour,
    GreedyCycle
};

class GreedySolver : public Solver
{
public:
    int startNode;
    GreedyType type;
    bool isConsideringProfits;

    GreedySolver(DataLoader &data, int startNode, GreedyType type = GreedyType::NearestNeighbour, bool isConsideringProfits = true) : 
        Solver(data),
        startNode(startNode),
        type(type),
        isConsideringProfits(isConsideringProfits) {};
    GreedySolver(DataLoader &data, GreedyType type = GreedyType::NearestNeighbour, bool isConsideringProfits = true) : 
        GreedySolver(data, random_device()() % data.numNodes, type, isConsideringProfits) {};
    void solve() override;

private:
    vector<int> unvisitedNodes;

    void swapAndPop(int index);
    template <typename ScoreFunction>
    int findNeareastNode(int node, ScoreFunction scoreFunction);

    template <typename SolveFunction>
    void solvePhaseI(SolveFunction solveFunction);
    template <typename ScoreFunction>
    void solveNN(ScoreFunction scoreFunction);
    template <typename ScoreFunction>
    void solveGC(ScoreFunction scoreFunction);
    void solvePhaseII();
};

template <typename ScoreFunction>
int GreedySolver::findNeareastNode(int node, ScoreFunction scoreFunction)
{
    const auto &distancesFromNode = data.distanceMatrix[node];

    int bestScore = numeric_limits<int>::min();
    int bestI = -1;
    // iterujemy po wszystkich nieodwiedzonych wierzchołkach
    for (size_t i = 0; i < unvisitedNodes.size(); i++)
    {
        // (zysk z odwiedzenia wierzchołka) - odległość node -> nowy
        int score = scoreFunction(unvisitedNodes[i], distancesFromNode);

        if (score > bestScore)
        {
            bestScore = score;
            bestI = i;
        }
    }

    return bestI;
}

template <typename SolveFunction>
void GreedySolver::solvePhaseI(SolveFunction solveFunction)
{
    // wybór odpowiedniej funkcji oceny
    if (isConsideringProfits)
        solveFunction([&](int node, const auto &dists)
            { return data.nodeProfits[node] - dists[node]; });
    else
        solveFunction([&](int node, const auto &dists)
            { return -dists[node]; });
}

template <typename ScoreFunction>
void GreedySolver::solveNN(ScoreFunction scoreFunction)
{
    // wybieramy wierzchołki do momentu uzyskania cyklu Hamiltona
    while (!unvisitedNodes.empty())
    {
        int bestI = findNeareastNode(solution.back(), scoreFunction);
        // dodanie odpowiedniego wierzchołka do rozwiazania i usunięcie z tablicy nieodwiedzonych wierzchołków
        solution.push_back(unvisitedNodes[bestI]);
        swapAndPop(bestI);
    }
}

template <typename ScoreFunction>
void GreedySolver::solveGC(ScoreFunction scoreFunction)
{
    // wybieramy 1. najlepszy wierzchołek metodą Nearest Neighbour
    int bestFirstI = findNeareastNode(solution.back(), scoreFunction);
    solution.push_back(unvisitedNodes[bestFirstI]);
    swapAndPop(bestFirstI);

    // wybieramy pozostałe wierzchołki do momentu uzyskania cyklu Hamiltona
    while (!unvisitedNodes.empty())
    {
        int bestScore = numeric_limits<int>::min();
        int bestI = -1;
        int bestJ = -1;
        const int n = solution.size();
        // iterujemy po wszystkich nieodwiedzonych wierzchołkach
        for (size_t i = 0; i < unvisitedNodes.size(); i++)
        {
            const auto &distancesFromPreviousNode = data.distanceMatrix[solution[0]];
            const int newNode = unvisitedNodes[i];

            // przypadek 0 -> nowy -> N - 1
            // (zysk z odwiedzenia wierzchołka) - odległość 0 -> nowy
            int score = scoreFunction(newNode, distancesFromPreviousNode);
            // + odległość 0 -> N - 1
            score += distancesFromPreviousNode[solution[n - 1]];
            // - odległość nowy -> N - 1
            score -= data.distanceMatrix[newNode][solution[n - 1]];
            if (score > bestScore)
            {
                bestScore = score;
                bestI = i;
                bestJ = 0;
            }
            // przypadki j -> nowy -> j - 1 dla 0 < j < N
            for (int j = 1; j < n; j++)
            {
                const auto &distancesFromPreviousNode = data.distanceMatrix[solution[j]];

                // (wartość z dodania wierzchołka) - odległość j -> nowy
                int score = scoreFunction(newNode, distancesFromPreviousNode);
                // + odległość j -> j - 1
                score += distancesFromPreviousNode[solution[j - 1]];
                // - odległość nowy -> j - 1
                score -= data.distanceMatrix[newNode][solution[j - 1]];
                if (score > bestScore)
                {
                    bestScore = score;
                    bestI = i;
                    bestJ = j;
                }
            }
        }

        solution.insert(solution.begin() + bestJ, unvisitedNodes[bestI]);
        swapAndPop(bestI);
    }
}
