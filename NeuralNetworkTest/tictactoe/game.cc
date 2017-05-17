
#include <unordered_set>
#include <vector>

#include "tictactoe/game.h"

namespace TicTacToe {

const std::array<std::array<Game::Position, 3>, 8> Game::WinningCombinations = {
    std::array<Position, 3>{Position{0, 0}, Position{1, 1}, Position{2, 2}},
    std::array<Position, 3>{Position{2, 0}, Position{1, 1}, Position{0, 2}},
    std::array<Position, 3>{Position{0, 0}, Position{0, 1}, Position{0, 2}},
    std::array<Position, 3>{Position{1, 0}, Position{1, 1}, Position{1, 2}},
    std::array<Position, 3>{Position{2, 0}, Position{2, 1}, Position{2, 2}},
    std::array<Position, 3>{Position{0, 0}, Position{1, 0}, Position{2, 0}},
    std::array<Position, 3>{Position{0, 1}, Position{1, 1}, Position{2, 1}},
    std::array<Position, 3>{Position{0, 2}, Position{1, 2}, Position{2, 2}}};

Player Game::Tile(int x, int y) const { return board_[x][y]; }

void Game::SetTile(int x, int y, Player player) { board_[x][y] = player; }

Player Game::Tile(const Position& xy) const {
  return Tile(xy.first, xy.second);
}

void Game::SetTile(const Position& xy, Player player) {
  SetTile(xy.first, xy.second, player);
}

Player Game::Winner() const {
  // Collect all players which met at least one winning condition.
  std::unordered_set<Player> winners;
  for (const std::array<Position, 3>& winning_combination :
       WinningCombinations) {
    const Position a = winning_combination[0], b = winning_combination[1],
                   c = winning_combination[2];
    if (Tile(a) == Tile(b) && Tile(b) == Tile(c)) {
      winners.insert(Tile(a));
    }
  }

  // Return the actual winner.
  if (winners.find(None) != winners.end()) {
    winners.erase(winners.find(None));
  }
  if (winners.empty()) {
    return None;
  } else if (winners.size() == 1) {
    return *winners.begin();
  } else {
    return Both;
  }
}

int Game::Turns() const {
  int result = 0;
  for (int x = 0; x < 3; ++x) {
    for (int y = 0; y < 3; ++y) {
      if (Tile(x, y) != None) {
        result++;
      }
    }
  }
  return result;
}

std::vector<Game::Position> Game::FreeMoves() const {
  std::vector<Position> result;
  for (int x = 0; x < 3; ++x) {
    for (int y = 0; y < 3; ++y) {
      if (Tile(x, y) == None) {
        result.emplace_back(x, y);
      }
    }
  }
  return result;
}

bool Game::Over() const { return Winner() != None || Turns() == 9; }

unsigned int Game::ID() const {
  unsigned int result = 0;
  unsigned int factor = 1;
  for (int x = 0; x < 3; ++x) {
    for (int y = 0; y < 3; ++y) {
      result += static_cast<int>(Tile(x, y)) * factor;
      factor *= 3;
    }
  }
  return result;
}
}
