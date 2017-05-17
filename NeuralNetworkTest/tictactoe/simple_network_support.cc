
#include <algorithm>

#include "snowhouse/snowhouse.h"
#include "tictactoe/simple_network_support.h"
#include "util/random/util.h"

namespace TicTacToe {
Game::Position AINextMove(const Game& game, const SimpleNetwork& network) {
  try {
    AssertThat(network.LayerSize(0), snowhouse::Equals(10u));
    AssertThat(network.LayerSize(network.LayerNumber() - 1),
               snowhouse::Equals(9u));
    AssertThat(game.FreeMoves().size(), snowhouse::IsGreaterThan(0u));
  } catch (const snowhouse::AssertionException& ex) {
    std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::cerr << ex.GetMessage() << std::endl;
    exit(1);
  }

  std::vector<double> output = network.Forward(GameToNetworkInput(game));

  // Make sure that no tile that is already taken will be selected.
  const double min_value = *std::min_element(output.begin(), output.end());
  const double below_min =
      std::nextafter(min_value, -std::numeric_limits<double>::infinity());
  for (int x = 0; x < 3; ++x) {
    for (int y = 0; y < 3; ++y) {
      if (!game.Tile(x, y) == None) {
        output[x + y * 3] = below_min;
      }
    }
  }

  return OutputToPosition(output);
}

std::vector<double> GameToNetworkInput(const Game& game) {
  std::vector<double> input;
  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      const Player tile = game.Tile(x, y);
      switch (tile) {
        case X:
          input.push_back(-1.0);
          break;
        case O:
          input.push_back(1.0);
          break;
        default:
          input.push_back(0.0);
          break;
      }
    }
  }
  input.push_back(1.0);
  return input;
}

Game::Position OutputToPosition(const std::vector<double>& output) {
  // Now select the element with the highest value.
  const auto max_iter = std::max_element(output.begin(), output.end());
  const int position = std::distance(output.begin(), max_iter);
  return Game::Position(position % 3, position / 3);
}

double SimpleNetworkSlowFitness::operator()() const {
  Game human_start;
  Game ai_start;
  ai_start.SetTile(AINextMove(ai_start, *network_), O);
  return FitnessFrom(human_start) + FitnessFrom(ai_start);
}

double SimpleNetworkSlowFitness::FitnessFrom(const Game& game) const {
  const unsigned int game_id = game.ID();
  if (fitness_memory_.find(game_id) == fitness_memory_.end()) {
    fitness_memory_[game_id] = ComputeFitness(game);
  }
  return fitness_memory_[game_id];
}

double SimpleNetworkSlowFitness::ComputeFitness(const Game& game) const {
  double fitness_sum = 0.0;
  for (const Game::Position& free_position : game.FreeMoves()) {
    // Make the opponents move.
    Game next_state = game;
    next_state.SetTile(free_position, X);
    if (next_state.FreeMoves().size() == 0) {
      fitness_sum += EndedGameFitness(next_state);
      continue;
    }

    // Make the AI's move.
    next_state.SetTile(AINextMove(next_state, *network_), O);
    if (next_state.FreeMoves().size() == 0) {
      fitness_sum += EndedGameFitness(next_state);
    } else {
      fitness_sum += FitnessFrom(next_state);
    }
  }
  return fitness_sum;
}

double SimpleNetworkSlowFitness::EndedGameFitness(const Game& game) const {
  // Player won.
  if (game.Winner() == X) {
    return game.Turns() - 10;
  }

  // AI won.
  if (game.Winner() == O) {
    return 10 - game.Turns();
  }

  // Draw or invalid input.
  return 0.0;
}

double SimpleNetworkFastFitness::operator()() const {
  Game human_start;
  Game ai_start;
  ai_start.SetTile(AINextMove(ai_start, *network_), O);
  return FitnessFrom(human_start) + FitnessFrom(ai_start);
}

double SimpleNetworkFastFitness::FitnessFrom(Game game) const {
  if (game.Over()) {
    return EndedGameFitness(game);
  }
  game.SetTile(OpponentMove(game), X);
  if (game.Over()) {
    return EndedGameFitness(game);
  }
  game.SetTile(AINextMove(game, *network_), O);
  return FitnessFrom(std::move(game));
}

Game::Position SimpleNetworkFastFitness::OpponentMove(const Game& game) const {
  // Win if possible.
  for (const std::array<Game::Position, 3>& winning_combination :
       Game::WinningCombinations) {
    Game::Position a = winning_combination[0], b = winning_combination[1],
                   c = winning_combination[2];
    if (game.Tile(a) == X && game.Tile(b) == X && game.Tile(c) == None) {
      return c;
    }
    if (game.Tile(a) == X && game.Tile(b) == None && game.Tile(c) == X) {
      return b;
    }
    if (game.Tile(a) == None && game.Tile(b) == X && game.Tile(c) == X) {
      return a;
    }
  }

  // Stop a victory if possible.
  for (const std::array<Game::Position, 3>& winning_combination :
       Game::WinningCombinations) {
    Game::Position a = winning_combination[0], b = winning_combination[1],
                   c = winning_combination[2];
    if (game.Tile(a) == O && game.Tile(b) == O && game.Tile(c) == None) {
      return c;
    }
    if (game.Tile(a) == O && game.Tile(b) == None && game.Tile(c) == O) {
      return b;
    }
    if (game.Tile(a) == None && game.Tile(b) == O && game.Tile(c) == O) {
      return a;
    }
  }

  // Set in the center if it is free.
  if (game.Tile(1, 1) == None) {
    return Game::Position(1, 1);
  }

  // Choose a the first free position. (random is not an option beause fitness
  // needs to be deterministic)
  return game.FreeMoves().front();
}

double SimpleNetworkFastFitness::EndedGameFitness(const Game& game) const {
  // Player won.
  if (game.Winner() == X) {
    return game.Turns() - 10;
  }

  // AI won.
  if (game.Winner() == O) {
    return 10 - game.Turns();
  }

  // Draw or invalid input.
  return 0.0;
}
}
