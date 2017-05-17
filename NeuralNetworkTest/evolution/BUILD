
cc_library(
  name = "evolver",
  hdrs = [
    "evolver.h",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "process",
  hdrs = [
    "process.h",
    "process.impl.h",
  ],
  deps = [
    ":evolver",
    "//util/random:probabilistic_sort",
    "//util:sort"
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "process_test",
  srcs = [
    "process_test.cc",
  ],
  deps = [
    ":process",
    "@gtest//:main",
  ],
  size = "small",
)
