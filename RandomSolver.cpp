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
    solution = views::iota(0, data.numNodes) | ranges::to<vector<int>>();

    // losowanie kolejności wierzchołków
    shuffle(solution.begin(), solution.end(), rng);

    // wybieranie pierwszych numChosenNodes wierzchołków
    solution.resize(numChosenNodes);

    // wyliczanie wyniku rozwiązania
    solutionScore = data.nodeProfits[solution[0]];
    size_t currentNode = 0;
    for (currentNode = 1; currentNode < solution.size(); currentNode++)
    {
        solutionScore -= data.distanceMatrix[solution[currentNode - 1]][solution[currentNode]];
        solutionScore += data.nodeProfits[solution[currentNode]];
    }
    solutionScore -= data.distanceMatrix[solution[currentNode - 1]][solution[0]];
}
