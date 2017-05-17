
cc_library(
  name = "sort",
  hdrs = [
    "sort.h",
    "sort.impl.h",
  ],
  deps = [
    ":permute",
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "sort_test",
  srcs = [
    "sort_test.cc",
  ],
  deps = [
    ":sort",
    "@gtest//:main",
  ],
  size = "small",
)

cc_library(
  name = "permute",
  hdrs = [
    "permute.h",
    "permute.impl.h",
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "permute_test",
  srcs = [
    "permute_test.cc",
  ],
  deps = [
    ":permute",
    "@gtest//:main",
  ],
  size = "small",
)
