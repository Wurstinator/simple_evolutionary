
#include "nn/simple_network_evolver.h"
#include "snowhouse/snowhouse.h"
#include "util/random/util.h"
#include "util/random/weighted_distribution.h"

using namespace snowhouse;

SimpleNetworkEvolver::SimpleNetworkEvolver(const Options& options)
    : options_(options) {}

SimpleNetwork SimpleNetworkEvolver::InitialSpecimen() {
  SimpleNetwork specimen(options_.layer_sizes);
  // Create a random path from the input to each output node.
  for (unsigned int i = 0; i < specimen.LayerSize(specimen.LayerNumber() - 1);
       ++i) {
    const SimpleNetwork::Node from(
        0, ::util::random::RandomInt(0, specimen.LayerSize(0) - 1));
    const SimpleNetwork::Node to(specimen.LayerNumber() - 1, i);
    const std::vector<SimpleNetwork::Edge> path =
        RandomPath(specimen, from, to);

    // Add the edges with random weights to the network.
    for (const SimpleNetwork::Edge& edge : path) {
      specimen.AddConnection(edge, ::util::random::RandomDouble(-1.0, 1.0));
    }
  }
  return specimen;
}

SimpleNetwork SimpleNetworkEvolver::Mate(const SimpleNetwork& father,
                                         const SimpleNetwork& mother) {
  // Both parents have to have the same layer sizes which is also copied to the
  // offspring.
  try {
    AssertThat(father.LayerNumber(), Equals(mother.LayerNumber()));
    for (unsigned int i = 0; i < father.LayerNumber(); ++i) {
      AssertThat(father.LayerSize(i), Equals(mother.LayerSize(i)));
    }
  } catch (const AssertionException& ex) {
    std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::cerr << "father-mother mismatch: " << ex.GetMessage() << std::endl;
    exit(1);
  }

  std::vector<int> layer_sizes;
  for (unsigned int i = 0; i < father.LayerNumber(); ++i) {
    layer_sizes.push_back(father.LayerSize(i));
  }
  SimpleNetwork offspring(layer_sizes);

  // For each edge, randomly copy the father or the mother.
  for (const SimpleNetwork::Edge& edge : offspring.AllEdges()) {
    const SimpleNetwork& hereditary_parent =
        ::util::random::RollPercentage(0.5) ? father : mother;
    if (hereditary_parent.HasConnection(edge)) {
      offspring.AddConnection(edge, hereditary_parent.ConnectionWeight(edge));
    }
  }
  return offspring;
}

SimpleNetwork SimpleNetworkEvolver::Mutate(const SimpleNetwork& specimen) {
  SimpleNetwork mutated_specimen = specimen;

  // Add or remove edges.
  if (::util::random::RollPercentage(options_.mutation_grow_chance)) {
    MutateGrowth(&mutated_specimen);
  }

  // Change weights on existing edges.
  const auto edge_exists = std::bind(&SimpleNetwork::HasConnection, &specimen,
                                     std::placeholders::_1);
  for (const SimpleNetwork::Edge& edge :
       mutated_specimen.AllEdges(edge_exists)) {
    if (::util::random::RollPercentage(options_.mutation_weight_chance)) {
      MutateWeight(&mutated_specimen, edge);
    }
  }

  return mutated_specimen;
}

std::vector<SimpleNetwork::Edge> SimpleNetworkEvolver::RandomPath(
    const SimpleNetwork& network, const SimpleNetwork::Node& from,
    const SimpleNetwork::Node& to) const {
  // If the nodes are directly connected, nothing is to be done.
  if (from.layer == to.layer) {
    return std::vector<SimpleNetwork::Edge>();
  }
  if (from.layer + 1 == to.layer) {
    return std::vector<SimpleNetwork::Edge>{SimpleNetwork::Edge(from, to)};
  }

  // Select a node from the previous layer of "to" and connect it recursively.
  const int intermediate_index =
      ::util::random::RandomInt(0, network.LayerSize(to.layer - 1) - 1);
  const SimpleNetwork::Node intermediate(to.layer - 1, intermediate_index);
  std::vector<SimpleNetwork::Edge> result =
      RandomPath(network, from, intermediate);
  result.emplace_back(intermediate, to);
  return result;
}

void SimpleNetworkEvolver::MutateGrowth(SimpleNetwork* specimen) {
  // Use a weighted distribution to select the change in edge numbers.
  const auto distribution_map_pair =
      ::util::random::WeightedDistribution::FromMap(
          options_.mutation_grow_probabilities);
  ::util::random::WeightedDistribution distribution =
      distribution_map_pair.first;
  std::unordered_map<::util::random::WeightedDistribution::result_type, int>
      back_map = distribution_map_pair.second;

  const int growth = back_map[distribution(*::util::random::StaticGenerator())];
  const std::function<void(SimpleNetworkEvolver*, SimpleNetwork*)>
      remove_or_add = (growth < 0 ? &SimpleNetworkEvolver::RemoveRandomEdge
                                  : &SimpleNetworkEvolver::AddRandomEdge);
  for (int i = 0; i < std::abs(growth); ++i) {
    remove_or_add(this, specimen);
  }
}

void SimpleNetworkEvolver::AddRandomEdge(SimpleNetwork* specimen) {
  // Select random edge that does not exist.
  const auto not_has_edge = [specimen](const SimpleNetwork::Edge& edge) {
    return !specimen->HasConnection(edge);
  };
  const std::vector<SimpleNetwork::Edge> edges =
      specimen->AllEdges(not_has_edge);
  if (edges.empty()) {
    return;
  }
  const SimpleNetwork::Edge edge =
      *::util::random::RandomSequenceElement(edges.begin(), edges.end());

  // Add the edge.
  specimen->AddConnection(edge, ::util::random::RandomDouble(-1.0, 1.0));
}

void SimpleNetworkEvolver::RemoveRandomEdge(SimpleNetwork* specimen) {
  // Select random existing edge.
  const auto has_edge = [specimen](const SimpleNetwork::Edge& edge) {
    return specimen->HasConnection(edge);
  };
  const std::vector<SimpleNetwork::Edge> edges = specimen->AllEdges(has_edge);
  if (edges.empty()) {
    return;
  }
  const SimpleNetwork::Edge edge =
      *::util::random::RandomSequenceElement(edges.begin(), edges.end());

  // Remove the edge.
  specimen->RemoveConnection(edge);
}

void SimpleNetworkEvolver::MutateWeight(SimpleNetwork* specimen,
                                        const SimpleNetwork::Edge& edge) {
  std::normal_distribution<double> normal_distribution(
      0.0, options_.mutation_weight_stddev);
  const double weight_change =
      normal_distribution(*::util::random::StaticGenerator());
  const double new_weight = std::max(
      -1.0, std::min(1.0, specimen->ConnectionWeight(edge) + weight_change));
  specimen->AddConnection(edge, new_weight);
}
