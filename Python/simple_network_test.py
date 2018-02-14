
import unittest
from simple_network import SimpleNetwork
from numpy.testing import assert_array_equal

class TestSimpleNetwork(unittest.TestCase):
    # output:  (0)       (0)        (0)
    #            (0)              (0)
    # input:             (a)
    #
    # (a) -> (0)
    @staticmethod
    def setup_network_1() -> SimpleNetwork:
        network = SimpleNetwork([1, 2, 3])
        network._layers[0].connections[0, 0] = 1
        network._layers[1].connections[1, 2] = 1
        network._layers[1].weights[1, 2] = 0.5
        return network

    def test_layer_number(self):
        self.assertEqual(self.setup_network_1().layer_number(), 3)

    def test_layer_size(self):
        network = self.setup_network_1()
        self.assertEqual(network.layer_size(0), 1)
        self.assertEqual(network.layer_size(1), 2)
        self.assertEqual(network.layer_size(2), 3)

    def test_has_connection(self):
        network = self.setup_network_1()
        self.assertTrue(network.has_connection(SimpleNetwork.Edge(0, 0, 0)))
        self.assertFalse(network.has_connection(SimpleNetwork.Edge(0, 0, 1)))
        self.assertFalse(network.has_connection(SimpleNetwork.Edge(1, 0, 0)))
        self.assertFalse(network.has_connection(SimpleNetwork.Edge(1, 0, 1)))
        self.assertFalse(network.has_connection(SimpleNetwork.Edge(1, 0, 2)))
        self.assertFalse(network.has_connection(SimpleNetwork.Edge(1, 1, 0)))
        self.assertFalse(network.has_connection(SimpleNetwork.Edge(1, 1, 1)))
        self.assertTrue(network.has_connection(SimpleNetwork.Edge(1, 1, 2)))

    def test_connection_weight(self):
        network = self.setup_network_1()
        self.assertEqual(network.connection_weight(SimpleNetwork.Edge(0, 0, 0)), 0.0)
        self.assertEqual(network.connection_weight(SimpleNetwork.Edge(1, 1, 2)), 0.5)

    def test_add_connection_1(self):
        network = self.setup_network_1()
        edges = [SimpleNetwork.Edge(1, 1, 0), SimpleNetwork.Edge(1, 1, 1), SimpleNetwork.Edge(1, 1, 2)]
        network.add_connection(edges[0])  # add new edge
        network.add_connection(edges[2])  # add edges which already exists
        self.assertTrue(network.has_connection(edges[0]))
        self.assertFalse(network.has_connection(edges[1]))
        self.assertTrue(network.has_connection(edges[2]))
        self.assertEqual(network.connection_weight(edges[0]), 0.0)
        self.assertEqual(network.connection_weight(edges[1]), 0.0)
        self.assertEqual(network.connection_weight(edges[2]), 0.5)

    def test_add_connection_2(self):
        network = self.setup_network_1()
        edges = [SimpleNetwork.Edge(1, 1, 0), SimpleNetwork.Edge(1, 1, 1), SimpleNetwork.Edge(1, 1, 2)]
        network.add_connection(edges[0], 1.0)  # add new edge
        network.add_connection(edges[2], 1.0)  # add edges which already exists
        self.assertTrue(network.has_connection(edges[0]))
        self.assertFalse(network.has_connection(edges[1]))
        self.assertTrue(network.has_connection(edges[2]))
        self.assertEqual(network.connection_weight(edges[0]), 1.0)
        self.assertEqual(network.connection_weight(edges[1]), 0.0)
        self.assertEqual(network.connection_weight(edges[2]), 1.0)

    def test_remove_connection(self):
        network = self.setup_network_1()
        edges = [SimpleNetwork.Edge(0, 0, 0), SimpleNetwork.Edge(1, 1, 1), SimpleNetwork.Edge(1, 1, 2)]
        network.remove_connection(edges[0])  # remove edge which does not exist
        network.remove_connection(edges[1])  # remove actual edge
        self.assertFalse(network.has_connection(edges[0]))
        self.assertFalse(network.has_connection(edges[1]))
        self.assertTrue(network.has_connection(edges[2]))
        self.assertEqual(network.connection_weight(edges[2]), 0.5)

    # output: (a + (b - a) / 2)      (2 * (a + b / 2))
    #            (a)     (a + b / 2)          (b - a)
    # input:          (a)               (b)
    #
    # (a, b) -> (a / 2 + b / 2, 2 * a + b)
    @staticmethod
    def setup_network_2() -> SimpleNetwork:
        network = SimpleNetwork([2, 3, 2])
        network.add_connection(SimpleNetwork.Edge(0, 0, 0), 1.0)
        network.add_connection(SimpleNetwork.Edge(0, 0, 1), 1.0)
        network.add_connection(SimpleNetwork.Edge(0, 0, 2), -1.0)
        network.add_connection(SimpleNetwork.Edge(0, 1, 1), 0.5)
        network.add_connection(SimpleNetwork.Edge(0, 1, 2), 1.0)
        network.add_connection(SimpleNetwork.Edge(1, 0, 0), 1.0)
        network.add_connection(SimpleNetwork.Edge(1, 1, 1), 2.0)
        network.add_connection(SimpleNetwork.Edge(1, 2, 0), 0.5)
        return network

    def forward_network_2(self, a: float, b: float):
        # For a given input, checks that network 2 computes the correct output.
        network = self.setup_network_2()
        output = network.forward([a, b])
        expected_output = [a/2 + b/2, a*2 + b]
        self.assertEqual(len(output), 2)
        assert_array_equal(output, expected_output)

    def test_simple_forward(self):
        self.forward_network_2(0.0, 0.0)
        self.forward_network_2(1.0, 0.0)
        self.forward_network_2(0.0, 1.0)
        self.forward_network_2(0.5, -0.5)
        self.forward_network_2(-0.4, 0.4)

    # (a, b) -> (a - b)
    @staticmethod
    def setup_network_3() -> SimpleNetwork:
        network = SimpleNetwork([2, 1])
        network.add_connection(SimpleNetwork.Edge(0, 0, 0), 0.5)
        network.add_connection(SimpleNetwork.Edge(0, 1, 0), -0.5)
        network.activation_function = (lambda x: x*2)
        return network

    def forward_network_3(self, a: float, b: float):
        # For a given input, checks that network 3 computes the correct output.
        network = self.setup_network_2()
        output = network.forward([a, b])
        expected_output = a-b
        self.assertEqual(len(output), 1)
        self.assertEqual(output[0], expected_output)

    def test_activation_forward(self):
        self.forward_network_3(0.4, 0.7)
        self.forward_network_3(-1.0, 1.0)
        self.forward_network_3(0.2, 0.0)