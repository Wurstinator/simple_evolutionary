
from tic_tac_toe.game import Game
from simple_network import SimpleNetwork
from typing import List, Tuple


def ai_next_move(game: Game, network: SimpleNetwork) -> Tuple[int, int]:
    # Computes the next move of the network AI.
    assert(network.layer_size(0) == 10)  # 9x tile inputs, one constant 1.0 input
    assert(network.layer_size(network.layer_number() - 1) == 9)  # weights for each of the tiles
    network_output = network.forward(game_to_input(game))

    # Convert the network output to a position. Only free tiles are considered.
    def index_to_pos(i: int) -> Tuple[int, int]:
        return i // 3, i % 3
    valid_outputs = [(i, x) for i, x in enumerate(network_output) if game[index_to_pos(i)] == Game.Player.Non]
    return index_to_pos(max(valid_outputs, key=lambda ix: ix[1])[0])


def game_to_input(game: Game) -> List[float]:
    # Computes the input vector to a simple network from a given game.
    def tile_to_weight(pos: Tuple[int, int]) -> float:
        if game[pos] == Game.Player.X:
            return -1.0
        elif game[pos] == Game.Player.O:
            return 1.0
        else:
            return 0.0

    weights = [tile_to_weight(pos) for pos in Game.all_tiles()]
    return weights + [1.0]
