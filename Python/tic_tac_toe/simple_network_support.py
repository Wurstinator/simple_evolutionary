
from tic_tac_toe.game import Game
from tic_tac_toe.game_ai import ai_game, random_ai
from simple_network.simple_network import SimpleNetwork
from typing import List, Tuple
from functools import partial


def ai_next_move(game: Game, network: SimpleNetwork, ai_player=Game.Player.O) -> Tuple[int, int]:
    # Computes the next move of the network AI.
    assert(network.layer_size(0) == 10)  # 9x tile inputs, one constant 1.0 input
    assert(network.layer_size(network.layer_number() - 1) == 9)  # weights for each of the tiles
    network_output = network.forward(_game_to_input(game, ai_player))

    # Convert the network output to a position. Only free tiles are considered.
    def index_to_pos(i: int) -> Tuple[int, int]:
        return i // 3, i % 3
    valid_outputs = [(i, x) for i, x in enumerate(network_output) if game[index_to_pos(i)] == Game.Player.Non]
    return index_to_pos(max(valid_outputs, key=lambda ix: ix[1])[0])


def _game_to_input(game: Game, ai_player: Game.Player) -> List[float]:
    # Computes the input vector to a simple network from a given game.
    weights = [_player_to_score(game[pos], ai_player) for pos in Game.all_tiles()]
    weights.append(1.0)
    return weights


def _player_to_score(player: Game.Player, ai_player: Game.Player) -> float:
    # Computes the score of "player" from the view of "ai_player".
    if player == ai_player:
        return 1.0
    elif player == Game.Player.Non:
        return 0.0
    else:
        return -1.0


def fitness_vs_random(network: SimpleNetwork, n: int) -> float:
    # Lets the network play n games against a random AI and returns the relative number of wins minus losses.
    def play_game(i: int) -> Game.Player:
        starting_player = Game.Player.X if i % 2 == 0 else Game.Player.O
        return ai_game(starting_player, ai_x=random_ai, ai_o=partial(ai_next_move, network=network))
    winners = [play_game(i) for i in range(n)]
    scores = map(partial(_player_to_score, ai_player=Game.Player.O), winners)
    return sum(scores) / n


def fitness_vs_network(network: SimpleNetwork, opponent: SimpleNetwork) -> float:
    # Computes the fitness by letting two networks play against each other (twice, with each one starting once).
    network_ai = partial(ai_next_move, ai_player=Game.Player.O, network=network)
    opponent_ai = partial(ai_next_move, ai_player=Game.Player.X, network=opponent)
    x = ai_game(Game.Player.O, network_ai, opponent_ai)
    y = ai_game(Game.Player.X, network_ai, opponent_ai)
    return (_player_to_score(x, Game.Player.O) + _player_to_score(y, Game.Player.O)) / 2