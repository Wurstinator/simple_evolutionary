
#include "simple_network.h"

#include "gtest/gtest.h"

class SimpleNetworkTest : public ::testing::Test {
 protected:
  SimpleNetwork test_network_1() {
    SimpleNetwork network(std::vector<int>{1, 2, 3});
    network.layers_[0].connectivity_matrix(0, 0) = 1;
    network.layers_[1].connectivity_matrix(1, 2) = 1;
    network.layers_[1].weight_matrix(1, 2) = 0.5;
    return network;
  }

  //  output:   (a + (b - a)/2)    (2*(a + b/2))
  //          (a)    (a + b/2)      (b - a)
  //   input:     (a)          (b)
  //
  // (a, b) -> (a/2 + b/2, 2*a + b)
  SimpleNetwork test_network_2() {
    SimpleNetwork network(std::vector<int>{2, 3, 2});
    network.AddConnection(SimpleNetwork::Edge(0, 0, 0), 1.0);
    network.AddConnection(SimpleNetwork::Edge(0, 0, 1), 1.0);
    network.AddConnection(SimpleNetwork::Edge(0, 0, 2), -1.0);
    network.AddConnection(SimpleNetwork::Edge(0, 1, 1), 0.5);
    network.AddConnection(SimpleNetwork::Edge(0, 1, 2), 1.0);
    network.AddConnection(SimpleNetwork::Edge(1, 0, 0), 1.0);
    network.AddConnection(SimpleNetwork::Edge(1, 1, 1), 2.0);
    network.AddConnection(SimpleNetwork::Edge(1, 2, 0), 0.5);
    return network;
  }

  // (a, b) -> (a - b)
  SimpleNetwork test_network_3() {
    SimpleNetwork network(std::vector<int>{2, 1});
    network.AddConnection(SimpleNetwork::Edge(0, 0, 0), 0.5);
    network.AddConnection(SimpleNetwork::Edge(0, 1, 0), -0.5);
    network.activation_function = [](double x) { return x * 2; };
    return network;
  }

  // Sets up the network and input and output vectors for ForwardTest.
  void ForwardTestSetup(double a, double b) {
    const SimpleNetwork network = test_network_2();
    const std::vector<double> input{a, b};
    const std::vector<double> expected_output{a / 2 + b / 2, 2 * a + b};
    const std::vector<double> actual_output = network.Forward(input);
    ASSERT_EQ(actual_output.size(), 2);
    EXPECT_DOUBLE_EQ(actual_output[0], expected_output[0]);
    EXPECT_DOUBLE_EQ(actual_output[1], expected_output[1]);
  }

  // Sets up the network and input and output vectors for ActiationFunctionTest.
  void ActivationFunctionTestSetup(double a, double b) {
    const SimpleNetwork network = test_network_3();
    const std::vector<double> input{a, b};
    const double expected_output = a - b;
    const std::vector<double> actual_output = network.Forward(input);
    ASSERT_EQ(actual_output.size(), 1);
    EXPECT_DOUBLE_EQ(actual_output[0], expected_output);
  }
};

TEST_F(SimpleNetworkTest, LayerNumberTest) {
  const SimpleNetwork network = test_network_1();
  EXPECT_EQ(network.LayerNumber(), 3);
}

TEST_F(SimpleNetworkTest, LayerSizeTest) {
  const SimpleNetwork network = test_network_1();
  EXPECT_EQ(network.LayerSize(0), 1);
  EXPECT_EQ(network.LayerSize(1), 2);
  EXPECT_EQ(network.LayerSize(2), 3);
}

TEST_F(SimpleNetworkTest, HasConnectionTest) {
  const SimpleNetwork network = test_network_1();
  EXPECT_TRUE(network.HasConnection(SimpleNetwork::Edge(0, 0, 0)));
  EXPECT_FALSE(network.HasConnection(SimpleNetwork::Edge(0, 0, 1)));
  EXPECT_FALSE(network.HasConnection(SimpleNetwork::Edge(1, 0, 0)));
  EXPECT_FALSE(network.HasConnection(SimpleNetwork::Edge(1, 0, 1)));
  EXPECT_FALSE(network.HasConnection(SimpleNetwork::Edge(1, 0, 2)));
  EXPECT_FALSE(network.HasConnection(SimpleNetwork::Edge(1, 1, 0)));
  EXPECT_FALSE(network.HasConnection(SimpleNetwork::Edge(1, 1, 1)));
  EXPECT_TRUE(network.HasConnection(SimpleNetwork::Edge(1, 1, 2)));
}

TEST_F(SimpleNetworkTest, ConnectionWeightTest) {
  const SimpleNetwork network = test_network_1();
  EXPECT_EQ(network.ConnectionWeight(SimpleNetwork::Edge(0, 0, 0)), 0.0);
  EXPECT_EQ(network.ConnectionWeight(SimpleNetwork::Edge(1, 1, 2)), 0.5);
}

TEST_F(SimpleNetworkTest, AddConnectionTest1) {
  // Make sure the network is in the desired state.
  SimpleNetwork network = test_network_1();
  const SimpleNetwork::Edge edge1(1, 1, 0), edge2(1, 1, 1), edge3(1, 1, 2);
  ASSERT_FALSE(network.HasConnection(edge1));
  ASSERT_FALSE(network.HasConnection(edge2));
  ASSERT_TRUE(network.HasConnection(edge3));
  ASSERT_EQ(network.ConnectionWeight(edge1), 0.0);
  ASSERT_EQ(network.ConnectionWeight(edge2), 0.0);
  ASSERT_EQ(network.ConnectionWeight(edge3), 0.5);

  // Add connections to the network.
  network.AddConnection(edge1);
  network.AddConnection(edge3);

  // Test new state.
  EXPECT_TRUE(network.HasConnection(edge1));
  EXPECT_FALSE(network.HasConnection(edge2));
  EXPECT_TRUE(network.HasConnection(edge3));
  EXPECT_EQ(network.ConnectionWeight(edge1), 0.0);
  EXPECT_EQ(network.ConnectionWeight(edge2), 0.0);
  EXPECT_EQ(network.ConnectionWeight(edge3), 0.5);
}

TEST_F(SimpleNetworkTest, AddConnectionTest2) {
  // Make sure the network is in the desired state.
  SimpleNetwork network = test_network_1();
  const SimpleNetwork::Edge edge1(1, 1, 0), edge2(1, 1, 1), edge3(1, 1, 2);
  ASSERT_FALSE(network.HasConnection(edge1));
  ASSERT_FALSE(network.HasConnection(edge2));
  ASSERT_TRUE(network.HasConnection(edge3));
  ASSERT_EQ(network.ConnectionWeight(edge1), 0.0);
  ASSERT_EQ(network.ConnectionWeight(edge2), 0.0);
  ASSERT_EQ(network.ConnectionWeight(edge3), 0.5);

  // Add connections to the network.
  network.AddConnection(edge1, 1.0);
  network.AddConnection(edge3, 1.0);

  // Test new state.
  EXPECT_TRUE(network.HasConnection(edge1));
  EXPECT_FALSE(network.HasConnection(edge2));
  EXPECT_TRUE(network.HasConnection(edge3));
  EXPECT_EQ(network.ConnectionWeight(edge1), 1.0);
  EXPECT_EQ(network.ConnectionWeight(edge2), 0.0);
  EXPECT_EQ(network.ConnectionWeight(edge3), 1.0);
}

TEST_F(SimpleNetworkTest, RemoveConnectionTest) {
  // Make sure the network is in the desired state.
  SimpleNetwork network = test_network_1();
  const SimpleNetwork::Edge edge1(0, 0, 0), edge2(1, 1, 1), edge3(1, 1, 2);
  ASSERT_TRUE(network.HasConnection(edge1));
  ASSERT_FALSE(network.HasConnection(edge2));
  ASSERT_TRUE(network.HasConnection(edge3));
  ASSERT_EQ(network.ConnectionWeight(edge1), 0.0);
  ASSERT_EQ(network.ConnectionWeight(edge2), 0.0);
  ASSERT_EQ(network.ConnectionWeight(edge3), 0.5);

  // Add connections to the network.
  network.RemoveConnection(edge1);
  network.RemoveConnection(edge2);

  // Test new state.
  EXPECT_FALSE(network.HasConnection(edge1));
  EXPECT_FALSE(network.HasConnection(edge2));
  EXPECT_TRUE(network.HasConnection(edge3));
  EXPECT_EQ(network.ConnectionWeight(edge3), 0.5);
}

TEST_F(SimpleNetworkTest, ForwardTest_1) { ForwardTestSetup(0.0, 0.0); }

TEST_F(SimpleNetworkTest, ForwardTest_2) { ForwardTestSetup(1.0, 0.0); }

TEST_F(SimpleNetworkTest, ForwardTest_3) { ForwardTestSetup(0.0, 1.0); }

TEST_F(SimpleNetworkTest, ForwardTest_4) { ForwardTestSetup(0.5, -0.5); }

TEST_F(SimpleNetworkTest, ForwardTest_5) { ForwardTestSetup(-0.4, 0.4); }

TEST_F(SimpleNetworkTest, ActivationFunctionTest_1) {
  ActivationFunctionTestSetup(0.4, 0.7);
}

TEST_F(SimpleNetworkTest, ActivationFunctionTest_2) {
  ActivationFunctionTestSetup(-1.0, 1.0);
}

TEST_F(SimpleNetworkTest, ActivationFunctionTest_3) {
  ActivationFunctionTestSetup(0.2, 0.0);
}
