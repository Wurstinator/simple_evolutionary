
#include "nn/simple_network_evolver.h"
#include "gtest/gtest.h"

namespace {
// Checks if there is a path from the network input layer to this node.
bool ConnectedFromInput(const SimpleNetwork& network,
                        const SimpleNetwork::Node& node) {
  // If the node is on the input layer, nothing to do.
  if (node.layer == 0) {
    return true;
  }

  // Otherwise, the node is connected to the input iff there is a node on the
  // previous layer connected to the input, that this one is connected to.
  for (const SimpleNetwork::Node& previous_node :
       network.NodesOnLayer(node.layer - 1)) {
    if (network.HasConnection(SimpleNetwork::Edge(previous_node, node)) &&
        ConnectedFromInput(network, previous_node)) {
      return true;
    }
  }
  return false;
}

// Creates a complete network with all edges and one with no edges and mates
// them. The return value is the ratio of edges to non-edges in the offspring
// which should converge against 1.
double MateExperiment(int edges) {
  // Create the parent networks.
  const std::vector<int> layer_sizes{edges, 1};
  SimpleNetwork complete_network(layer_sizes);
  SimpleNetwork empty_network(layer_sizes);
  for (const SimpleNetwork::Edge& edge : complete_network.AllEdges()) {
    complete_network.AddConnection(edge, 1.0);
  }

  // Mate the two.
  SimpleNetworkEvolver evolver((SimpleNetworkEvolver::Options()));
  const SimpleNetwork offspring = evolver.Mate(complete_network, empty_network);

  // Calculate the experiment result.
  const auto has_edge = [&offspring](const SimpleNetwork::Edge& edge) {
    return offspring.HasConnection(edge);
  };
  const auto not_has_edge = [&offspring](const SimpleNetwork::Edge& edge) {
    return !offspring.HasConnection(edge);
  };
  const int added_edges = offspring.AllEdges(has_edge).size();
  const int removed_edges = offspring.AllEdges(not_has_edge).size();
  return static_cast<double>(added_edges) / removed_edges;
}
}

#include <iostream>

TEST(SimpleNetworkEvolverTest, InitialSpecimenTest) {
  const int TEST_SIZE = 20;

  // Create the evolver.
  SimpleNetworkEvolver::Options options;
  options.layer_sizes = std::vector<int>{2, 2, 3};
  SimpleNetworkEvolver evolver(options);

  // Create specimen.
  std::vector<SimpleNetwork> specimen;
  std::generate_n(std::back_inserter(specimen), TEST_SIZE,
                  std::bind(&SimpleNetworkEvolver::InitialSpecimen, &evolver));

  // Make sure they all have a path from the input to each output node.
  for (const SimpleNetwork& network : specimen) {
    for (unsigned int i = 0; i < network.LayerSize(network.LayerNumber() - 1);
         ++i) {
      EXPECT_TRUE(ConnectedFromInput(
          network, SimpleNetwork::Node(network.LayerNumber() - 1, i)));
    }
  }
}

TEST(SimpleNetworkEvolverTest, MateTest) {
  const int TEST_SIZE = 1000;
  const double ERROR_ALLOWED = 0.02;
  const int RETRIES = 5;

  bool passed = false;
  for (int i = 0; i < RETRIES; ++i) {
    // See if this experiment run passes the error threshold.
    const double FatherMotherRatio = MateExperiment(TEST_SIZE);
    const double error = std::abs(FatherMotherRatio - 1);
    if (error <= ERROR_ALLOWED) {
      passed = true;
    }
  }
  if (!passed) {
    FAIL() << "The ratio of father-mother inherited edges is not within the "
              "error threshold. Try rerunning the test to see if "
              "you were just unlucky. If it still fails, there might be a bug.";
  }
}

TEST(SimpleNetworkEvolverTest, MutateTest) {
  // TODO
}
