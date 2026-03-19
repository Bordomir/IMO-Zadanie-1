#include "include/RandomSolver.hpp"

#include <vector>
#include <ranges>
#include <algorithm>

using namespace std;

int RandomSolver::randomInt(int min, int max)
{
    return rng() % (max - min + 1) + min;
}

void RandomSolver::solve()
{
    // losowanie liczby wybranych wierzchołków
    int numChosenNodes = randomInt(1, data.numNodes);

    // tworzenie wektora wszystkich wierzchołków
    auto allNodesView = views::iota(0, data.numNodes);

    // losowanie numChosenNodes wierzchołków
    solution.clear();
    solution.reserve(numChosenNodes);
    sample(allNodesView.begin(), allNodesView.end(), back_inserter(solution), numChosenNodes, rng);

    // losowanie kolejności wierzchołków
    shuffle(solution.begin(), solution.end(), rng);

    // wyliczanie wyniku rozwiązania
    // sumowanie profitów z odwiedzonych wierzchołków
    solutionScore = ranges::fold_left(
        solution | views::transform([&](int node) { return data.nodeProfits[node]; }),
        0, plus<int>()
    );
    // odejmowanie odległości między kolejnymi wierzchołkami w rozwiązaniu
    solutionScore -= ranges::fold_left(
        solution | views::adjacent_transform<2>([&](int a, int b) { return data.distanceMatrix[a][b]; }),
        0, plus<int>()
    );
    // odejmowanie odległości między ostatnim a pierwszym wierzchołkiem (powrót do startu)
    solutionScore -= data.distanceMatrix[solution.back()][solution.front()];
}
