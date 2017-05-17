#pragma once

#include <unordered_map>

#include "nn/simple_network.h"
#include "tictactoe/game.h"

namespace TicTacToe {
// Given a neural network and a TicTacToe game, calculates the next move from
// the network.
Game::Position AINextMove(const Game& game, const SimpleNetwork& network);

// Converts the state of a TTT game to the input of a network.
std::vector<double> GameToNetworkInput(const Game& game);

// Converts the output of a network to the position in a game.
Game::Position OutputToPosition(const std::vector<double>& output);

// Calculates a fitness score of this network by having it play against all
// possible strategies.
struct SimpleNetworkSlowFitness {
 public:
  SimpleNetworkSlowFitness(const SimpleNetwork* network) : network_(network) {}

  double operator()() const;

 private:
  // Try to access the fitness from the memory map.
  double FitnessFrom(const Game& game) const;
  // Actually compute the fitness because it was not found in the memory map.
  double ComputeFitness(const Game& game) const;
  // The final fitness score of a game that has ended.
  double EndedGameFitness(const Game& game) const;

  const SimpleNetwork* network_;
  mutable std::unordered_map<unsigned int, double> fitness_memory_;
};

// Calculates a fitness score of this network by having it play against one
// specific predefined AI.
struct SimpleNetworkFastFitness {
 public:
  SimpleNetworkFastFitness(const SimpleNetwork* network) : network_(network) {}

  double operator()() const;

 private:
  // Compute the fitness starting from a given game. This assumes that its the
  // opponents turn.
  double FitnessFrom(Game game) const;
  // The next move of the predefined ai.
  Game::Position OpponentMove(const Game& game) const;
  // The final fitness score of a game that has ended.
  double EndedGameFitness(const Game& game) const;

  const SimpleNetwork* network_;
};
}
