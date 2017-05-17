
#include "util/random/util.h"

namespace util {
namespace random {


std::mt19937* StaticGenerator() {
  static std::random_device device;
  static std::mt19937 generator(device());
  return &generator;
}

bool RollPercentage(double percentage) {
  if (percentage == 0) {
    return false;
  } else {
    return RandomDouble(0, 1) <= percentage;
  }
}

int RandomInt(int min, int max) {
  static std::uniform_int_distribution<int> distribution;
  const std::uniform_int_distribution<int>::param_type parameters(min, max);
  return distribution(*StaticGenerator(), parameters);
}

double RandomDouble(double min, double max) {
  static std::uniform_real_distribution<double> distribution;
  const std::uniform_real_distribution<double>::param_type parameters(min, max);
  return distribution(*StaticGenerator(), parameters);
}
}}
