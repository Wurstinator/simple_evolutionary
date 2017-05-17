#pragma once

#include <array>
#include <vector>

namespace TicTacToe {

enum Player { None = 0, X = 1, O = 2, Both = 3 };

class Game {
 public:
  using Position = std::pair<int, int>;

  Game() = default;

  // Getter and setter for single tiles.
  Player Tile(int x, int y) const;
  void SetTile(int x, int y, Player player);
  Player Tile(const Position& xy) const;
  void SetTile(const Position& xy, Player player);

  // List of all winning combinations.
  static const std::array<std::array<Position, 3>, 8> WinningCombinations;

  // Return the winner (if there is one).
  Player Winner() const;

  // Returns the number of turns that has passed (i.e. the number of tiles that
  // are not free).
  int Turns() const;

  // Return a list of all possible moves (i.e. the tiles that are free).
  std::vector<Position> FreeMoves() const;

  // Returns whether the game is over; this is achieved by filling the board or
  // by reaching a winner.
  bool Over() const;

  // Computes a number which uniquely identifies this game state.
  unsigned int ID() const;

 private:
  std::array<std::array<Player, 3>, 3> board_{
      {std::array<Player, 3>{None, None, None},
       std::array<Player, 3>{None, None, None},
       std::array<Player, 3>{None, None, None}}};
};
}

namespace std {
template <>
struct hash<TicTacToe::Player> {
  size_t operator()(TicTacToe::Player p) const { return std::hash<int>()(p); }
};
}
