
#include "simple_network_support.h"
#include "gtest/gtest.h"

namespace {
SimpleNetwork test_network() {
  SimpleNetwork network(std::vector<int>{10, 9});
  // Goal: If everything is free, play center. If the center is taken by human,
  // play (0, 0). If it is taken by AI, play (2, 2).
  network.AddConnection(SimpleNetwork::Edge(0, 9, 4), 1.0);
  network.AddConnection(SimpleNetwork::Edge(0, 4, 0), -0.5);
  network.AddConnection(SimpleNetwork::Edge(0, 4, 8), 0.5);

  return network;
}
}

// Make sure that "AINextMove" works correctly by calling it on a small
// predefined network.
TEST(SimpleNetworkSupportTest, SimpleNetworkFitnessTest) {
  SimpleNetwork network = test_network();

  TicTacToe::Game game1, game2, game3;
  game2.SetTile(1, 1, TicTacToe::X);
  game3.SetTile(1, 1, TicTacToe::O);

  EXPECT_EQ(TicTacToe::AINextMove(game1, network),
            TicTacToe::Game::Position(1, 1));
  EXPECT_EQ(TicTacToe::AINextMove(game2, network),
            TicTacToe::Game::Position(0, 0));
  EXPECT_EQ(TicTacToe::AINextMove(game3, network),
            TicTacToe::Game::Position(2, 2));
}

TEST(SimpleNetworkSupportTest, AINextMoveTest) {
  SimpleNetwork network = test_network();
  const double fitness = TicTacToe::SimpleNetworkFitness(&network)();
  EXPECT_DOUBLE_EQ(fitness, -104);
}
