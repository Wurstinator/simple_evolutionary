
cc_library(
  name = "util",
  hdrs = [
    "util.h",
  ],
  srcs = [
    "util.cc",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "probabilistic_sort",
  hdrs = [
    "probabilistic_sort.h",
    "probabilistic_sort.impl.h",
  ],
  deps = [
    ":util",
    ":weighted_distribution",
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "probabilistic_sort_test",
  srcs = [
    "probabilistic_sort_test.cc",
  ],
  deps = [
    ":probabilistic_sort",
    "@gtest//:main",
  ],
  size = "small",
)

cc_library(
  name = "weighted_distribution",
  hdrs = [
    "weighted_distribution.h",
    "weighted_distribution.impl.h",
  ],
  srcs = [
    "weighted_distribution.cc",
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "weighted_distribution_test",
  srcs = [
    "weighted_distribution_test.cc",
  ],
  deps = [
    ":weighted_distribution",
    "@gtest//:main",
  ],
  size = "small",
)
