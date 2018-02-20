
from tic_tac_toe.game import Game
from typing import Callable, Tuple
from random import choice


def ai_game(starting_player: Game.Player,
            ai_x: Callable[[Game], Tuple[int, int]],
            ai_o: Callable[[Game], Tuple[int, int]]) -> Game.Player:
    # Runs a game between to AIs and returns the winner.
    player = starting_player
    game = Game()
    while not game.is_over():
        if player == Game.Player.X:
            move = ai_x(game)
            game[move] = Game.Player.X
            player = Game.Player.O
        else:
            move = ai_o(game)
            game[move] = Game.Player.O
            player = Game.Player.X

    return game.winner()


def random_ai(game: Game) -> Tuple[int, int]:
    # An AI that takes a random (valid) move every turn.
    return choice(list(game.free_moves()))
