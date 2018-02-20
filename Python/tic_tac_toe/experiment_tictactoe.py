
from simple_network.constant_network_evolver import ConstantNetworkEvolver
from tic_tac_toe.simple_network_support import fitness_vs_random
from evolution_process import evolution, EvolutionProcessOptions, terminate_after_n_generations, log_sorted_generations
from typing import Tuple, List, Mapping
from simple_network.simple_network import SimpleNetwork
import matplotlib.pyplot as plt

gen_log = []


def fit(generation: List[SimpleNetwork], age: int) -> Mapping[SimpleNetwork, float]:
    return {network: fitness_vs_random(network, age*2) for network in generation}


def log(g: Mapping[SimpleNetwork, float]):
    gen_log.append(g)
    print(list(reversed(sorted(g.values()))))


evolution_options = EvolutionProcessOptions(
    natural_selection_strategy=EvolutionProcessOptions.NaturalSelectionStrategy.KILL_PRECISE_WORST,
    generation_size=15,
    parent_count=2,
    offspring_count=10,
    terminate_condition=terminate_after_n_generations(35, log_sorted_generations(log, fit))
)

evolver = ConstantNetworkEvolver(
    layer_sizes=[10, 10, 9],
    mate_strategy=ConstantNetworkEvolver.MateStrategy.RANDOM_MERGE
)
g = evolution(evolver, evolution_options, fit)
#for g in gen_log:
#    print([x[1] for x in g])
plt.plot(list(map(lambda g: max(g.values()), gen_log)))
plt.title('VS fitness')
plt.show()