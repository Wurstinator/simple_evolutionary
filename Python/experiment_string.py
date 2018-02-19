
from evolver import Evolver
from evolution_process import evolution, EvolutionProcessOptions, terminate_after_n_generations
from typing import List
from random import choice, choices, randrange, gauss, random
from string import ascii_letters, digits


class StringEvolver(Evolver):

    chars = ascii_letters + digits + " !?"

    # Chances for what to do with a character during mutation. Nothing / Transform / Grow / Remove
    mutation_chances = [0.4, 0.3, 0.15, 0.15]

    def initial_specimen(self) -> str:
        return ''.join(choices(StringEvolver.chars, k=round(gauss(11, 5))))

    def mate(self, parents: List[str]) -> str:
        # Combine characters from the parents.
        child = ""
        for i in range(max(map(len, parents))):
            p = parents[i % len(parents)]
            if len(p) > i:
                child += p[i]
            i += 1
        return child

    def mutate(self, specimen: str) -> str:
        # For each position in the string, perform a local mutation.
        new_specimen = ""
        for c in specimen:
            new_specimen += StringEvolver.mutate_char(c)
        return new_specimen

    @staticmethod
    def mutate_char(c: chr) -> str:
        r = random()
        mc = StringEvolver.mutation_chances

        if r < mc[0]:  # do nothing
            return c
        elif r < mc[0] + mc[1]:  # transform to another character
            return choice(StringEvolver.chars)
        elif r < mc[0] + mc[1] + mc[2]:  # add another character before
            return choice(StringEvolver.chars) + c
        else:  # remove
            return ""


string_evolution_options = EvolutionProcessOptions(
    natural_selection_strategy=EvolutionProcessOptions.NaturalSelectionStrategy.KILL_PRECISE_WORST,
    generation_size=15,
    parent_count=2,
    offspring_count=50
)


def dist(s1, s2):
    d = 0
    for i in range(min(len(s1), len(s2))):
        if s1[i] != s2[i]:
            d += 1
    return -d - 2*abs(len(s1) - len(s2))


def evolve_strings(target_string: str) -> List[List[str]]:
    log = []

    def register_generation(generation: List[str]) -> bool:
        log.append(generation)
        return False

    string_evolution_options.terminate_condition=terminate_after_n_generations(20, register_generation)
    evolver = StringEvolver()
    fitness = (lambda s: dist(s, target_string))
    log.append(evolution(evolver, string_evolution_options, fitness))
    return log


print_with_score = True
print_only_best = True
ts = 'Hello World'
for g in evolve_strings(ts):
    g = list(reversed(sorted(g, key=(lambda s: dist(s, ts)))))
    if print_with_score:
        g = list(map(lambda x: (x, dist(x, ts)), g))
    if print_only_best:
        g = g[0]
    print(g)
