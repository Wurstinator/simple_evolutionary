
import unittest
from evolver import Evolver
from typing import List
from evolution_process import EvolutionProcessOptions, terminate_after_n_generations, evolution


class TestEvolutionProcess(unittest.TestCase):
    class Evolver1(Evolver[int]):
        def __init__(self):
            self.i = 0

        def initial_specimen(self) -> int:
            self.i += 1
            return self.i

        def mate(self, parents: List[int]) -> int:
            return sum(parents)

        def mutate(self, specimen: int) -> int:
            return specimen-1

    def test_kill_precise_worst(self):
        generations_log = []

        def register_generation(generation: List[int]) -> bool:
            generations_log.append(sorted(generation))
            return False

        options = EvolutionProcessOptions(
            natural_selection_strategy=EvolutionProcessOptions.NaturalSelectionStrategy.KILL_PRECISE_WORST,
            generation_size=3,
            starting_generation=[],
            parent_count=2,
            offspring_count=2,
            terminate_condition=terminate_after_n_generations(2, register_generation)
        )

        evolver = TestEvolutionProcess.Evolver1()

        final_generation = evolution(evolver, options, (lambda x: x))
        final_generation.sort()

        self.assertEqual(len(generations_log), 2)
        self.assertEqual(len(generations_log[0]), 3)
        self.assertEqual(len(generations_log[1]), 3)
        self.assertEqual(len(final_generation), 3)
        self.assertEqual(generations_log[0][0], 1)
        self.assertEqual(generations_log[0][1], 2)
        self.assertEqual(generations_log[0][2], 3)
        self.assertEqual(generations_log[1][0], 4)
        self.assertEqual(generations_log[1][1], 4)
        self.assertEqual(generations_log[1][2], 5)
        self.assertEqual(final_generation[0], 8)
        self.assertEqual(final_generation[1], 8)
        self.assertEqual(final_generation[2], 9)
