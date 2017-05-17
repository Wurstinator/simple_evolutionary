
cc_binary(
  name = "experiment",
  srcs = [
    "experiment.cc",
  ],
  deps = [
    ":simple_network_support",
    "//nn:simple_network",
    "//nn:simple_network_evolver",
    "//evolution:process",
    ":interactive",
  ],
)

cc_library(
  name = "game",
  srcs = [
    "game.cc",
  ],
  hdrs = [
    "game.h",
  ],
  deps = [],
)

cc_library(
  name = "simple_network_support",
  srcs = [
    "simple_network_support.cc",
  ],
  hdrs = [
    "simple_network_support.h",
  ],
  deps = [
    ":game",
    "//nn:simple_network",
    "//util/random:util",
  ]
)

cc_test(
  name = "simple_network_support_test",
  srcs = [
    "simple_network_support_test.cc",
  ],
  deps = [
    "@gtest//:main",
    ":simple_network_support",
  ],
  size = "small",
)

cc_library(
  name = "interactive",
  srcs = [
    "interactive.cc",
  ],
  hdrs = [
    "interactive.h",
  ],
  deps = [
    ":game",
    ":simple_network_support",
    "//nn:simple_network",
  ],
)
