
from tic_tac_toe.simple_network_support import ai_next_move
from simple_network import SimpleNetwork
from tic_tac_toe.game import Game
import unittest


class TestSimpleNetworkSupport(unittest.TestCase):
    def test_ai_next_move(self):
        # Tests "ai_next_move" for a simple predefined network.

        # Set up the network.
        network = SimpleNetwork([10, 9])
        network.add_connection(SimpleNetwork.Edge(0, 9, 4), 1.0)
        network.add_connection(SimpleNetwork.Edge(0, 4, 0), -0.5)
        network.add_connection(SimpleNetwork.Edge(0, 4, 8), 0.5)

        # Set up the test games.
        game1 = Game()
        game2 = Game()
        game2[1, 1] = Game.Player.X
        game3 = Game()
        game3[1, 1] = Game.Player.O

        # Compare outputs.
        self.assertEqual(ai_next_move(game1, network), (1, 1))
        self.assertEqual(ai_next_move(game2, network), (0, 0))
        self.assertEqual(ai_next_move(game3, network), (2, 2))
