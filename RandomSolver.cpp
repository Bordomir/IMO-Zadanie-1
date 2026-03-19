#include "include/RandomSolver.hpp"

#include <vector>
#include <ranges>
#include <algorithm>

using namespace std;

inline int RandomSolver::randomInt(int min, int max)
{
    return rng() % (max - min + 1) + min;
}

void RandomSolver::solve()
{
    // losowanie liczby wybranych wierzchołków
    int numChosenNodes = randomInt(1, data.numNodes);

    // tworzenie widoku wszystkich wierzchołków
    auto allNodesView = views::iota(0, data.numNodes);

    // losowanie numChosenNodes wierzchołków
    solution.clear();
    solution.resize(numChosenNodes);
    sample(allNodesView.begin(), allNodesView.end(), solution.begin(), numChosenNodes, rng);

    // losowanie kolejności wierzchołków
    shuffle(solution.begin(), solution.end(), rng);

    // wyliczanie wyniku rozwiązania
    solutionScore = data.nodeProfits[solution[0]];
    int currentNode = 0;
    for (currentNode = 1; currentNode < numChosenNodes; currentNode++)
    {
        solutionScore -= data.distanceMatrix[solution[currentNode - 1]][solution[currentNode]];
        solutionScore += data.nodeProfits[solution[currentNode]];
    }
    solutionScore -= data.distanceMatrix[solution[currentNode - 1]][solution[0]];
}
