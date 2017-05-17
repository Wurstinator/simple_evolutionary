
cc_library(
  name = "simple_network",
  srcs = [
    "simple_network.cc",
  ],
  hdrs = [
    "simple_network.h",
  ],
  deps = [
    "@armadillo//:main",
    "@snowhouse//:main",
  ],
  visibility = [
    "//nn:__subpackages__",
    "//tictactoe:__pkg__",
  ],
)

cc_test(
  name = "simple_network_test",
  srcs = ["simple_network_test.cc"],
  deps = [
    ":simple_network",
    "@gtest//:main",
  ],
  size = "small",
)

cc_library(
  name = "simple_network_evolver",
  srcs = [
    "simple_network_evolver.cc",
  ],
  hdrs = [
    "simple_network_evolver.h",
  ],
  deps = [
    ":simple_network",
    "//util/random:util",
    "//util/random:weighted_distribution",
    "//evolution:evolver",
    "@snowhouse//:main",
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "simple_network_evolver_test",
  srcs = [
    "simple_network_evolver_test.cc",
  ],
  deps = [
    ":simple_network_evolver",
    "@gtest//:main",
  ],
  size = "small",
)
