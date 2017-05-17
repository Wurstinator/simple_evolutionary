cc_library(
    name = "main",
    srcs = [
      "src/wrapper.cpp"
    ],
    hdrs = glob([
      "include/*",
    ]),
    deps = [
      ":armadillo_bits"
    ],
    visibility = ["//visibility:public"],
    copts = [
      "-Iexternal/armadillo/include",
      "-DARMA_DONT_USE_WRAPPER",
    ],
    linkopts = [
      "-lblas",
      "-llapack",
    ],
    strip_include_prefix = "include/",
)

cc_library(
  name = "armadillo_bits",
  hdrs = glob([
      "include/armadillo_bits/*.hpp",
  ]),
  strip_include_prefix = "include/",
)
