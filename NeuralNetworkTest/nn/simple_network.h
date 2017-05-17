#pragma once

#include <functional>
#include <vector>
#include "armadillo"

// Network of fixed size with nodes that pass values to the next layer.
class SimpleNetwork {
 public:
  // A node represented by the index of its layer and its own index inside the
  // layer.
  struct Node {
    unsigned int layer;
    unsigned int index;

    Node(unsigned int layer, unsigned int index) : layer(layer), index(index) {}
  };

  // An edge inside the network.
  struct Edge {
    Node from;
    Node to;

    Edge(Node from, Node to) : from(std::move(from)), to(std::move(to)) {}

    Edge(unsigned int layer_from, unsigned int node_from, unsigned int node_to)
        : from(layer_from, node_from), to(layer_from + 1, node_to) {}
  };

  // Constructs a new network with the given number of layers and nodes on each
  // layer. The first layer is the input, the last layer is the output.
  SimpleNetwork(const std::vector<int>& layer_sizes);

  // Returns the number of layers (including input and output).
  unsigned int LayerNumber() const;

  // Returns the number of nodes in a certain layer.
  unsigned int LayerSize(unsigned int layer) const;

  // Returns whether there is a connection between to nodes.
  bool HasConnection(const Edge& edge) const;

  // Returns the weight of the connection between nodes. If there is no
  // connection, an undefined value is returned.
  double ConnectionWeight(const Edge& edge) const;

  // Adds a connection with weight 0 to the network. If it already exists,
  // nothing is done.
  void AddConnection(const Edge& edge);

  // Adds a connection to the network. If it already exists, the weight is
  // updated.
  void AddConnection(const Edge& edge, double weight);

  // Removes a connection from the network.
  void RemoveConnection(const Edge& edge);

  // Runs the network on some input and returns the output.
  std::vector<double> Forward(const std::vector<double>& input) const;

  // Returns a list of all edges that can possibly exist in this node setup. If
  // the parameter is set, it is used as a filter to only keep those edges in
  // the list which evaluate to true.
  std::vector<Edge> AllEdges() const;
  std::vector<Edge> AllEdges(
      const std::function<bool(const Edge&)>& predicate) const;

  // Returns a list of all nodes on a certain layer.
  std::vector<Node> NodesOnLayer(unsigned int layer) const;

  // The function that is used at each node to aggregate the sum of the incoming
  // signals.
  std::function<double(double)> activation_function = [](double x) {
    return x;
  };

  // For unit testing.
  friend class SimpleNetworkTest;

 private:
  struct Layer {
    Layer(int size, int next_layer_size)
        : connectivity_matrix(size, next_layer_size, arma::fill::zeros),
          weight_matrix(size, next_layer_size, arma::fill::zeros) {}

    arma::Mat<unsigned int> connectivity_matrix;
    arma::Mat<double> weight_matrix;
  };

  // Asserts that the edge/node is present in the network.
  void AssertEdgeIsValid(const Edge& edge) const;
  void AssertNodeIsValid(const Node& node) const;

  // Propagates data of one layer to the next.
  arma::rowvec ForwardOneLayer(const arma::rowvec& input,
                               const Layer& layer) const;

  // The list of layers, beginning with the input layer and followed by the
  // inner layers. The output layer is not present as it does not have outgoing
  // edges.
  std::vector<Layer> layers_;
};

template <typename StreamT>
StreamT& operator<<(StreamT& os, const SimpleNetwork::Edge& edge) {
  os << edge.from.layer << ' ' << edge.from.index << ' ' << edge.to.layer << ' '
     << edge.to.index;
  return os;
}
