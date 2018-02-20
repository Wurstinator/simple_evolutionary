
from simple_network.simple_network import SimpleNetwork
from evolver import Evolver
from typing import List
from random import random, gauss, choice
from enum import Enum


class ConstantNetworkEvolver(Evolver):
    # An evolver for complete networks of constant layer sizes. Only mutates the weights of edges.

    class MateStrategy(Enum):
        AVERAGE_WEIGHTS = 0
        RANDOM_MERGE = 1

    def __init__(self,
                 layer_sizes: List[int],
                 mutate_chance: float = 0.5,
                 mutate_variance: float = 0.5,
                 mate_strategy: MateStrategy = MateStrategy.AVERAGE_WEIGHTS):
        self.layer_sizes = layer_sizes
        self.mutate_chance = mutate_chance
        self.mutate_variance = mutate_variance
        self.mate_strategy = mate_strategy

    def initial_specimen(self) -> SimpleNetwork:
        return SimpleNetwork.complete_network(self.layer_sizes)

    def mate(self, parents: List[SimpleNetwork]) -> SimpleNetwork:
        # Average the weights for all edges.
        assert(len(parents) != 0)
        if self.mate_strategy == ConstantNetworkEvolver.MateStrategy.AVERAGE_WEIGHTS:
            return self._mate_average(parents)
        elif self.mate_strategy == ConstantNetworkEvolver.MateStrategy.RANDOM_MERGE:
            return self._mate_random_merge(parents)

    def _mate_average(self, parents: List[SimpleNetwork]) -> SimpleNetwork:
        # Build a new network by averaging the weight of each edge among the parents.
        network = SimpleNetwork.complete_network(self.layer_sizes)
        for e in network.all_edges():
            weights = [p.connection_weight(e) for p in parents]
            network.add_connection(e, sum(weights) / len(weights))
        return network

    def _mate_random_merge(self, parents: List[SimpleNetwork]) -> SimpleNetwork:
        # Build a new network by taking the weight of a random parent for each edge.
        network = SimpleNetwork.complete_network(self.layer_sizes)
        for e in network.all_edges():
            w = choice(parents).connection_weight(e)
            network.add_connection(e, w)
        return network

    def mutate(self, specimen: SimpleNetwork) -> SimpleNetwork:
        # For each edge, there is a fixed chance that the edge will mutate according to a Gaussian distribution.
        for e in specimen.all_edges():
            if random() < self.mutate_chance:
                new_weight = gauss(specimen.connection_weight(e), self.mutate_variance)
                specimen.add_connection(e, new_weight)
        return specimen
