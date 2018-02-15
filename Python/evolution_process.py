from typing import Generic, TypeVar, List, Callable
from evolver import Evolver

T = TypeVar('T')


class EvolutionProcessOptions(Generic[T]):
    # This class defines a set of options used by the evolution process.

    class NaturalSelectionStrategy:
        # Kills all specimen from worst to best until generation size is met.
        KILL_PRECISE_WORST = 0
        # Kills all specimen until generation size is met. Worse specimen are more likely to be killed.
        #KILL_PROB_WORST = 1

    def __init__(self,
                 natural_selection_strategy: int = 0,
                 generation_size: int = 0,
                 starting_generation: List[T] = [],
                 offspring_count: int = 0,
                 terminate_condition: Callable[[List[T]], bool] = (lambda x: True)):
        # Defines the strategy which defines which specimen to keep in each generation.
        self.natural_selection_strategy = natural_selection_strategy
        # The size of each generation.
        self.generation_size = generation_size
        # A sample of specimen for the first generation. The remaining spots will be filled by the evolver object.
        self.starting_generation = starting_generation
        # Number of children for each pair of parents.
        self.offspring_count = offspring_count
        # Callable that defines at which generation to stop. Is called exactly once before a mating step.
        self.terminate_condition = terminate_condition


def terminate_after_n_generations(n: int, second_condition: Callable[[List[T]], bool] = (lambda x: False)) \
        -> Callable[[List[T]], bool]:
    # Helper function. If used as terminate_condition, the process will stop after n generations have been evolved or
    # the second condition is satisfied.
    class Counter:
        def __init__(self):
            self.counter = 0

        def __call__(self, generation: List[T]) -> bool:
            if self.counter == n:
                return True
            else:
                self.counter += 1
                return second_condition(generation)

    return Counter()


def evolution(evolver: Evolver[T], options: EvolutionProcessOptions[T],
              fitness_function: Callable[[T], float]) -> List[T]:
    # Performs evolution and returns the final generation.
    generation = _initial_generation(evolver, options)
    while not options.terminate_condition(generation):
        generation = _natural_selection(_evolve(generation, evolver, options), options, fitness_function)
    return generation


def _initial_generation(evolver: Evolver[T], options: EvolutionProcessOptions[T]) -> List[T]:
    # Creates the first generation for a evolution process.
    generation = options.starting_generation
    return generation + [evolver.initial_specimen() for _ in range(options.generation_size - len(generation))]


def _evolve(generation: List[T], evolver: Evolver[T], options: EvolutionProcessOptions[T]) -> List[T]:
    # Mate all specimen with each other.
    offspring = [evolver.mate(x, y) for x in generation for y in generation for _ in range(options.offspring_count)]
    # Mutate all specimen in the new generation.
    return [evolver.mutate(x) for x in offspring]


def _natural_selection(generation: List[T], options: EvolutionProcessOptions[T],
                       fitness_function: Callable[[T], float]) -> List[T]:
    # Use natural selection and the fitness function to keep only the strongest specimen.
    etf = { EvolutionProcessOptions.NaturalSelectionStrategy.KILL_PRECISE_WORST: _natural_selection_kill_precise_worst}
    return etf[options.natural_selection_strategy](generation, fitness_function, options.generation_size)


def _natural_selection_kill_precise_worst(generation: List[T], fitness_function: Callable[[T], float],
                                          n: int) -> List[T]:
    return sorted(generation, key=fitness_function)[-n:]
