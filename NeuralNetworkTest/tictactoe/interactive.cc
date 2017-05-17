
#include <iostream>

#include "tictactoe/game.h"
#include "tictactoe/interactive.h"
#include "tictactoe/simple_network_support.h"

namespace TicTacToe {

namespace {
void PrintBoard(const Game& game) {
  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      switch (game.Tile(x, y)) {
        case None:
          std::cout << ' ';
          break;
        case O:
          std::cout << 'O';
          break;
        case X:
          std::cout << 'X';
          break;
        default:
          std::cout << '?';
          break;
      }
    }
    std::cout << std::endl;
  }
}

void PlayerTurn(Game* game) {
  int x, y;
  std::cout << "Give position: " << std::flush;
  std::cin >> x >> y;
  game->SetTile(x, y, X);
  PrintBoard(*game);
}

void AiTurn(Game* game, const SimpleNetwork& ai) {
  game->SetTile(AINextMove(*game, ai), O);
  PrintBoard(*game);
}
}

void PlayAgainstAI(const SimpleNetwork& ai) {
  Game game;
  while (!game.Over()) {
    PlayerTurn(&game);
    if (!game.Over()) {
      AiTurn(&game, ai);
    }
  }
  std::cout << "Player " << game.Winner() << " won.";
}
}
