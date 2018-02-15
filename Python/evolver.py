
from typing import Generic, TypeVar
from abc import ABC

T = TypeVar('T')


class Evolver(ABC, Generic[T]):
    # Abstract base class for objects which can evolve in a genetic algorithm.

    def initial_specimen(self) -> T:
        # Creates a random specimen for the first generation.
        pass

    def mate(self, father: T, mother: T) -> T:
        # Combines two specimen to a single one.
        pass

    def mutate(self, specimen: T) -> T:
        # Performs random mutation on a specimen.
        pass
