
#include "nn/simple_network.h"
#include "snowhouse/snowhouse.h"

using namespace snowhouse;

SimpleNetwork::SimpleNetwork(const std::vector<int>& layer_sizes) {
  try {
    AssertThat(layer_sizes.size(), IsGreaterThan(1u));
  } catch (const AssertionException& ex) {
    std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::cerr << ex.GetMessage() << std::endl;
    exit(1);
  }

  const auto begin_iter = layer_sizes.cbegin();
  const auto end_iter = std::prev(layer_sizes.cend());
  for (auto iter = begin_iter; iter != end_iter; ++iter) {
    layers_.emplace_back(*iter, *std::next(iter));
  }
}

unsigned int SimpleNetwork::LayerNumber() const { return layers_.size() + 1; }

unsigned int SimpleNetwork::LayerSize(unsigned int layer) const {
  try {
    AssertThat(layer, IsLessThan(LayerNumber()));
  } catch (const AssertionException& ex) {
    std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::cerr << ex.GetMessage() << std::endl;
    exit(1);
  }

  if (layer + 1 == LayerNumber()) {
    return layers_.back().connectivity_matrix.n_cols;
  } else {
    return layers_[layer].connectivity_matrix.n_rows;
  }
}

bool SimpleNetwork::HasConnection(const Edge& edge) const {
  AssertEdgeIsValid(edge);
  const Layer& layer = layers_[edge.from.layer];
  return !!(layer.connectivity_matrix(edge.from.index, edge.to.index));
}

double SimpleNetwork::ConnectionWeight(const Edge& edge) const {
  AssertEdgeIsValid(edge);
  const Layer& layer = layers_[edge.from.layer];
  return layer.weight_matrix(edge.from.index, edge.to.index);
}

void SimpleNetwork::AddConnection(const Edge& edge) {
  if (HasConnection(edge)) {
    AddConnection(edge, ConnectionWeight(edge));
  } else {
    AddConnection(edge, 0.0);
  }
}

void SimpleNetwork::AddConnection(const Edge& edge, double weight) {
  AssertEdgeIsValid(edge);
  Layer& layer = layers_[edge.from.layer];
  layer.connectivity_matrix(edge.from.index, edge.to.index) = true;
  layer.weight_matrix(edge.from.index, edge.to.index) = weight;
}

void SimpleNetwork::RemoveConnection(const Edge& edge) {
  AssertEdgeIsValid(edge);
  Layer& layer = layers_[edge.from.layer];
  layer.connectivity_matrix(edge.from.index, edge.to.index) = false;
  layer.weight_matrix(edge.from.index, edge.to.index) = 0.0;
}

void SimpleNetwork::AssertEdgeIsValid(const Edge& edge) const {
  AssertNodeIsValid(edge.from);
  AssertNodeIsValid(edge.to);
}

void SimpleNetwork::AssertNodeIsValid(const Node& node) const {
  try {
    AssertThat(node.layer, IsLessThan(LayerNumber()));
    AssertThat(node.index, IsLessThan(LayerSize(node.layer)));
  } catch (const AssertionException& ex) {
    std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::cerr << ex.GetMessage() << std::endl;
    exit(1);
  }
}

std::vector<double> SimpleNetwork::Forward(
    const std::vector<double>& input) const {
  arma::rowvec layer_values(input);

  // Propagate the values forward through the layers.
  for (const Layer& layer : layers_) {
    layer_values = ForwardOneLayer(layer_values, layer);
  }

  // Convert the result back to a vector and return.
  std::vector<double> result;
  for (unsigned int i = 0; i < layer_values.n_elem; ++i) {
    result.push_back(layer_values(i));
  }
  return result;
}

std::vector<SimpleNetwork::Edge> SimpleNetwork::AllEdges() const {
  return AllEdges([](const Edge&) { return true; });
}

std::vector<SimpleNetwork::Edge> SimpleNetwork::AllEdges(
    const std::function<bool(const Edge&)>& predicate) const {
  std::vector<Edge> result;
  for (unsigned int i = 0; i < layers_.size(); ++i) {
    for (const Node& node1 : NodesOnLayer(i)) {
      for (const Node& node2 : NodesOnLayer(i + 1)) {
        const Edge edge(node1, node2);
        if (predicate(edge)) {
          result.push_back(edge);
        }
      }
    }
  }
  return result;
}

std::vector<SimpleNetwork::Node> SimpleNetwork::NodesOnLayer(
    unsigned int layer) const {
  try {
    AssertThat(layer, IsLessThan(LayerNumber()));
  } catch (const AssertionException& ex) {
    std::cerr << __FILE__ << " " << __LINE__ << std::endl;
    std::cerr << ex.GetMessage() << std::endl;
    exit(1);
  }

  std::vector<Node> result;
  for (unsigned int i = 0; i < LayerSize(layer); ++i) {
    result.emplace_back(layer, i);
  }
  return result;
}

arma::rowvec SimpleNetwork::ForwardOneLayer(const arma::rowvec& input,
                                            const Layer& layer) const {
  arma::rowvec next_values = input * layer.weight_matrix;
  next_values.transform(activation_function);
  return next_values;
}
