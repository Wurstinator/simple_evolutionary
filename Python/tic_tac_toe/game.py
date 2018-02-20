
from typing import Iterable, List, Tuple
from itertools import product
from enum import Enum


class Game:
    # Class for handling the game logic of TicTacToe.

    class Player(Enum):
        Non = 0
        X = 1
        O = 2
        Both = 3

    def __init__(self):
        self._board = [
            [Game.Player.Non, Game.Player.Non, Game.Player.Non],
            [Game.Player.Non, Game.Player.Non, Game.Player.Non],
            [Game.Player.Non, Game.Player.Non, Game.Player.Non]
        ]

    def __getitem__(self, item: Tuple[int, int]) -> Player:
        x, y = item
        return self._board[x][y]

    def __setitem__(self, key: Tuple[int, int], value: Player):
        x, y = key
        self._board[x][y] = value

    @staticmethod
    def winning_combinations() -> Iterable[List[Tuple[int, int]]]:
        # Returns a list of lists of positions which determine a winning player.
        yield [(0, 0), (1, 1), (2, 2)]
        yield [(2, 0), (1, 1), (0, 2)]
        yield [(0, 0), (0, 1), (0, 2)]
        yield [(1, 0), (1, 1), (1, 2)]
        yield [(2, 0), (2, 1), (2, 2)]
        yield [(0, 0), (1, 0), (2, 0)]
        yield [(0, 1), (1, 1), (2, 1)]
        yield [(0, 2), (1, 2), (2, 2)]

    @staticmethod
    def all_tiles() -> Iterable[Tuple[int, int]]:
        # Returns a list of all 9 positions.
        return product(range(3), repeat=2)

    def winner(self) -> Player:
        # Returns the Player enum corresponding to a player who has won the current game.
        o_won = False
        x_won = False
        for wc in Game.winning_combinations():
            if self[wc[0]] == self[wc[1]] == self[wc[2]]:
                o_won = (o_won or self[wc[0]] == Game.Player.O)
                x_won = (x_won or self[wc[0]] == Game.Player.X)

        if o_won:
            if x_won:
                return Game.Player.Both
            else:
                return Game.Player.O
        else:
            if x_won:
                return Game.Player.X
            else:
                return Game.Player.Non

    def is_over(self) -> bool:
        # Returns whether the game is over, i.e. there is a winner of the board is full.
        return (self.winner() != Game.Player.Non) or (self.turns_passed() == 9)

    def turns_passed(self) -> int:
        # Returns the number of turns passed so far, i.e. the number of tiles that are not free.
        return 9 - sum(map(lambda x: 1, self.free_moves()))

    def free_moves(self) -> Iterable[Tuple[int, int]]:
        # Returns a list of tiles that are still free.
        def free(pos):
            return self[pos] == Game.Player.Non
        return filter(free, Game.all_tiles())