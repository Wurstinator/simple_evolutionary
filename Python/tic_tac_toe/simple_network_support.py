
from tic_tac_toe.game import Game
from simple_network.simple_network import SimpleNetwork
from typing import List, Tuple
from random import choice


def ai_next_move(game: Game, network: SimpleNetwork) -> Tuple[int, int]:
    # Computes the next move of the network AI.
    assert(network.layer_size(0) == 10)  # 9x tile inputs, one constant 1.0 input
    assert(network.layer_size(network.layer_number() - 1) == 9)  # weights for each of the tiles
    network_output = network.forward(_game_to_input(game))

    # Convert the network output to a position. Only free tiles are considered.
    def index_to_pos(i: int) -> Tuple[int, int]:
        return i // 3, i % 3
    valid_outputs = [(i, x) for i, x in enumerate(network_output) if game[index_to_pos(i)] == Game.Player.Non]
    return index_to_pos(max(valid_outputs, key=lambda ix: ix[1])[0])


def _game_to_input(game: Game) -> List[float]:
    # Computes the input vector to a simple network from a given game.
    weights = [_player_to_score(game[pos]) for pos in Game.all_tiles()]
    return weights + [1.0]


def _player_to_score(player: Game.Player) -> float:
    if player == Game.Player.X:
        return -1.0
    elif player == Game.Player.O:
        return 1.0
    else:
        return 0.0


def fitness_vs_random(network: SimpleNetwork, n: int) -> float:
    # Lets the network play n games against a random AI and returns the relative number of wins minus losses.
    winners = [_play_vs_random(network, i % 2 == 0) for i in range(n)]
    return sum(map(_player_to_score, winners)) / n


def _play_vs_random(network: SimpleNetwork, ai_begins: bool) -> int:
    # Lets the network play against a random AI and returns the winner.
    current_player = (Game.Player.X if ai_begins else Game.Player.O)
    game = Game()
    while not game.is_over():
        if current_player == Game.Player.X:
            # Random AI turn
            move = choice(list(game.free_moves()))
            game[move] = Game.Player.X
            current_player = Game.Player.O
        else:
            # Network turn
            move = ai_next_move(game, network)
            game[move] = Game.Player.O
            current_player = Game.Player.X

    w = game.winner()

    return game.winner()
