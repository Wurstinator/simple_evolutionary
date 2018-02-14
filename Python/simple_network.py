
from typing import List, Callable
import numpy

class SimpleNetwork:
    # Network of fixed size with nodes that pass values to the next layer.

    class Node:
        # A node represented by the index of its layer and its own index inside the layer.
        def __init__(self, layer : int, index : int):
            # Index of the layer that this node is placed on.
            self.layer = layer
            # Index of this node on the layer.
            self.index = index

    class Edge:
        # An edge inside the network.
        def __init__(self, layer_from : int, node_from : int, node_to : int):
            # Node from which the edge starts.
            self.nfrom = SimpleNetwork.Node(layer_from, node_from)
            # Node at which the edge ends.
            self.nto = SimpleNetwork.Node(layer_from + 1, node_to)

    class Layer:
        # An entire layer in the network. Composed of connections to the next layer with weights.
        def __init__(self, size: int, next_layer_size: int):
            self.connections = numpy.zeros((size, next_layer_size))
            self.weights = numpy.zeros((size, next_layer_size))

    def __init__(self, layer_sizes: List[int]):
        # Initialize the list of layers from the given sizes. The layers are all empty / not connected at the start.
        self._layers = [SimpleNetwork.Layer(i, j) for (i, j) in zip(layer_sizes, layer_sizes[1:])]
        # The activation function is applied to each output of a layer.
        self.activation_function : Callable[[float], float] = (lambda x: x)

    def layer_number(self) -> int:
        # Returns the number of layers.
        return len(self._layers) + 1

    def layer_size(self, layer: int) -> int:
        # Returns the number of nodes on a specific layer.
        assert(layer < self.layer_number())

        if layer + 1 == self.layer_number():
            return self._layers[-1].connections.shape[1]
        else:
            return self._layers[layer].connections.shape[0]

    def has_connection(self, edge: Edge) -> bool:
        # Returns whether a specific edge exists in the network.
        self.assert_edge_valid(edge)
        layer = self._layers[edge.nfrom.layer]
        return layer.connections[edge.nfrom.index, edge.nto.index] != 0

    def connection_weight(self, edge: Edge) -> float:
        # Returns the weight of an edge in the network. If the edge does not exist, the result is undefined.
        self.assert_edge_valid(edge)
        layer = self._layers[edge.nfrom.layer]
        return layer.weights[edge.nfrom.index, edge.nto.index]

    def add_connection(self, edge: Edge, w: float = None):
        # Tries to add an edge to the network. If the edge is already present and the weight parameters is not set,
        # nothing is done. If the weight parameter is set, then the edge weight is updated. If the edge is not present,
        # it is added with the specified weight, or 0 by default.
        self.assert_edge_valid(edge)
        if w is not None:
            self._add_connection(edge, w)
        elif not self.has_connection(edge):
            self._add_connection(edge, 0)

    def _add_connection(self, edge: Edge, w: float):
        # Makes sure that the edge is present with given weight. Overwrites any previous values.
        layer = self._layers[edge.nfrom.layer]
        layer.connections[edge.nfrom.index, edge.nto.index] = 1
        layer.weights[edge.nfrom.index, edge.nto.index] = w

    def remove_connection(self, edge: Edge):
        # Removes a given edge from the network, if it exists.
        self.assert_edge_valid(edge)
        layer = self._layers[edge.nfrom.layer]
        layer.connections[edge.nfrom.index, edge.nto.index] = 0

    def assert_node_valid(self, node: Node):
        # Asserts whether a given node has valid indices for this network.
        assert(node.layer < self.layer_number())
        assert(node.index < self.layer_size(node.layer))

    def assert_edge_valid(self, edge: Edge):
        # Asserts whether a given edge has valid indices for this network.
        self.assert_node_valid(edge.nfrom)
        self.assert_node_valid(edge.nto)

    def forward(self, input: List[float]):
        # Computes the output for a given input.
        for layer in self._layers:
            input = self._forward_layer(input, layer)
        return input

    def _forward_layer(self, input: List[float], layer: Layer) -> List[float]:
        # Given a vector of inputs to a layer, computes its output.
        raw_values = numpy.dot(input, layer.weights)
        return [self.activation_function(x) for x in raw_values]