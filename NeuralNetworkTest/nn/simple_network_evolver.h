#pragma once

#include <map>

#include "evolution/evolver.h"
#include "nn/simple_network.h"

class SimpleNetworkEvolver : public Evolver<SimpleNetwork> {
 public:
  struct Options {
    // The layer sizes of the networks this evolver is creating.
    std::vector<int> layer_sizes;

    // The chance of a network to mutate by adding or removing an edge.
    double mutation_grow_chance = 0.0;

    // The chances how many edges are added/removed in case of a mutation. The
    // key of the map is the change in the number of edges; the value is the
    // unnormalized probability.
    std::map<int, double> mutation_grow_probabilities{{-1, 0.5}, {1, 0.5}};

    // The chances of a network to mutate by changing the weight of an edge.
    double mutation_weight_chance = 0.0;

    // The standard deviation of the normal distribution that is used to mutate
    // weights on edges. The mean of the distribution is 0. The weights will
    // never leave the interval [-1,1].
    double mutation_weight_stddev = 1.0;
  };

  // Creates a new evolver which is working on networks of the given size.
  SimpleNetworkEvolver(const Options& options);

  SimpleNetwork InitialSpecimen() override;

  SimpleNetwork Mate(const SimpleNetwork& father,
                     const SimpleNetwork& mother) override;

  SimpleNetwork Mutate(const SimpleNetwork& specimen) override;

 private:
  // Constructs a random path from node a to b.
  std::vector<SimpleNetwork::Edge> RandomPath(
      const SimpleNetwork& network, const SimpleNetwork::Node& from,
      const SimpleNetwork::Node& to) const;

  // Adds or removes edges from the network.
  void MutateGrowth(SimpleNetwork* specimen);

  // Adds/removes a single random edge in the network.
  void AddRandomEdge(SimpleNetwork* specimen);
  void RemoveRandomEdge(SimpleNetwork* specimen);

  // Changes the weight of an edge.
  void MutateWeight(SimpleNetwork* specimen, const SimpleNetwork::Edge& edge);

  Options options_;
};
