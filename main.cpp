#include <vector>
#include <memory>

#include "include/DataLoader.hpp"
#include "include/Solver.hpp"
#include "include/RandomSolver.hpp"
#include "include/GreedySolver.hpp"

int main()
{
    DataLoader dataA("../data/TSPA.csv");
    DataLoader dataB("../data/TSPB.csv");

    int startNode = 0;
    
    vector<unique_ptr<Solver>> solvers;
    solvers.emplace_back(make_unique<RandomSolver>(dataA));
    solvers.emplace_back(make_unique<GreedySolver>(dataA, startNode, GreedyType::NearestNeighbour));
    solvers.emplace_back(make_unique<GreedySolver>(dataA, startNode, GreedyType::NearestNeighbour, false));
    solvers.emplace_back(make_unique<GreedySolver>(dataA, startNode, GreedyType::GreedyCycle));
    solvers.emplace_back(make_unique<GreedySolver>(dataA, startNode, GreedyType::GreedyCycle, false));
    
    solvers.emplace_back(make_unique<RandomSolver>(dataB));
    solvers.emplace_back(make_unique<GreedySolver>(dataB, startNode, GreedyType::NearestNeighbour));
    solvers.emplace_back(make_unique<GreedySolver>(dataB, startNode, GreedyType::NearestNeighbour, false));
    solvers.emplace_back(make_unique<GreedySolver>(dataB, startNode, GreedyType::GreedyCycle));
    solvers.emplace_back(make_unique<GreedySolver>(dataB, startNode, GreedyType::GreedyCycle, false));

    for(auto &solver : solvers)
    {
        solver->solve();
        solver->print();
    }

    return 0;
}