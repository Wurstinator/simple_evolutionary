#pragma once

#include <iterator>
#include <random>

namespace util {
namespace random {


// Returns a static random bit generator.
std::mt19937* StaticGenerator();

// Checks if a percentage roll passes.
bool RollPercentage(double percentage);

// Returns a random integer in range [min, max].
int RandomInt(int min, int max);

// Returns a random double in range [min, max].
double RandomDouble(double min, double max);

// Returns a random element from a sequence.
template <typename IterT>
IterT RandomSequenceElement(IterT left, IterT right) {
  return std::next(left, RandomInt(0, std::distance(left, right) - 1));
}

}
}
