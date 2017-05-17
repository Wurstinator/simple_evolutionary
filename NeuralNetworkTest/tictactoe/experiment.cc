
#include <iostream>

#include "evolution/process.h"
#include "nn/simple_network.h"
#include "nn/simple_network_evolver.h"
#include "tictactoe/interactive.h"
#include "tictactoe/simple_network_support.h"

using SNProcess = Process<SimpleNetwork>;

double Fitness1(const SimpleNetwork& network) {
  return TicTacToe::SimpleNetworkFastFitness(&network)();
}

double Fitness2(const SimpleNetwork& network) {
  return TicTacToe::SimpleNetworkSlowFitness(&network)() + Fitness1(network) * 25;
}

void PrintNetworkWeights(const SimpleNetwork& network) {
  for (unsigned int layer = 0; layer < network.LayerNumber() - 1; ++layer) {
    for (unsigned int node1 = 0; node1 < network.LayerSize(layer); ++node1) {
      for (unsigned int node2 = 0; node2 < network.LayerSize(layer + 1);
           ++node2) {
        const double w =
            network.ConnectionWeight(SimpleNetwork::Edge(layer, node1, node2));
        std::cout << w << ' ';
      }
    }
  }
  std::cout << std::endl;
}

SimpleNetworkEvolver ConstructEvolver() {
  SimpleNetworkEvolver::Options evolver_options;
  evolver_options.layer_sizes = std::vector<int>{10, 12, 12, 9};
  evolver_options.mutation_grow_chance = 0.25;
  evolver_options.mutation_grow_probabilities = std::map<int, double>{
      {-2, 0.2}, {-1, 0.5}, {0, 0.4}, {1, 0.4}, {2, 0.8}, {3, 0.6}, {4, 0.4}};
  evolver_options.mutation_weight_chance = 0.1;
  evolver_options.mutation_weight_stddev = 0.6;
  return SimpleNetworkEvolver(evolver_options);
}

SNProcess::Options ConstructFirstOptions() {
  SNProcess::Options evolution_options;
  evolution_options.natural_selection_strategy =
      SNProcess::Options::KILL_PRECISE_WORST;
  evolution_options.generation_size = 60;
  const auto each_generation = [](const SNProcess::Generation& generation) {
    const double best = Fitness1(generation.front()), worst = Fitness1(generation.back());
    std::cout << "Fast generation done. Best: " << best
              << "  Worst: " << worst << std::endl;
    return best >= 5 && worst >= 4;
  };
  evolution_options.evolution_terminate =
      SNProcess::TerminateAfterNGenerations(6, each_generation);
  evolution_options.offspring_count = 40;
  return evolution_options;
}

SNProcess::Options ConstructSecondOptions() {
  SNProcess::Options evolution_options;
  evolution_options.natural_selection_strategy =
      SNProcess::Options::KILL_PRECISE_WORST;
  evolution_options.generation_size = 30;
  const auto each_generation = [](const SNProcess::Generation& generation) {
    std::cout << "Slow generation done. Best: " << Fitness2(generation.front())
              << "  Worst: " << Fitness2(generation.back()) << std::endl;
    return false;
  };
  evolution_options.evolution_terminate =
      SNProcess::TerminateAfterNGenerations(4, each_generation);
  evolution_options.offspring_count = 30;
  return evolution_options;
}

int main(int argc, char** argv) {
  SimpleNetworkEvolver evolver = ConstructEvolver();
  SNProcess::Options first_options = ConstructFirstOptions();
  SNProcess::Generation generation =
      SNProcess::Evolution(&evolver, &Fitness1, first_options);

  SNProcess::Options second_options = ConstructSecondOptions();
  second_options.starting_generation = generation;
  generation = SNProcess::Evolution(&evolver, &Fitness2, second_options);
  PrintNetworkWeights(generation[0]);

  for (;;) TicTacToe::PlayAgainstAI(generation[0]);
  return 0;
}
