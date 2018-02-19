
from typing import Iterable, List
from itertools import product

class Game:
    # Class for handling the game logic of TicTacToe.

    class Player:
        Non = 0
        X = 1
        O = 2
        Both = 3

    def __init__(self):
        self.board = [[Game.Player.Non]*3]*3

    def __getitem__(self, item: int) -> int:
        x, y = item
        return self.board[x][y]

    def __setitem__(self, key: int, value: int):
        x, y = key
        self.board[x][y] = value

    @staticmethod
    def winning_combinations() -> Iterable[List[(int, int)]]:
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
    def all_tiles() -> Iterable[(int,int)]:
        # Returns a list of all 9 positions.
        return product(range(3), repeat=2)

    def winner(self) -> int:
        # Returns the Player enum corresponding to a player who has won the current game.
        winners = {}
        for wc in Game.winning_combinations():
            for player in [Game.Player.X, Game.Player.O]:
                if all(self[pos] == player for pos in wc):
                    winners.add(player)

        if winners.empty():
            return Game.Player.Non
        elif len(winners) == 2:
            return Game.Player.Both
        else:
            return winners.pop()

    def is_over(self) -> bool:
        # Returns whether the game is over, i.e. there is a winner of the board is full.
        return (self.winner() != Game.Player.Non) or (self.turns_passed() == 9)

    def turns_passed(self) -> int:
        # Returns the number of turns passed so far, i.e. the number of tiles that are not free.
        return 9 - sum(map(lambda x: 1, self.free_moves()))

    def free_moves(self) -> Iterable[(int, int)]:
        # Returns a list of tiles that are still free.
        def free(pos):
            return self[pos] == Game.Player.Non
        return filter(free, Game.all_tiles())
