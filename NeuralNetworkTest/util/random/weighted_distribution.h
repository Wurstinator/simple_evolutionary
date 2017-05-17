#pragma once

#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>


namespace util {
namespace random {


// Satisfies the RandomNumberDistribution concept. Takes a sequence of weights
// (real numbers >= 0) as parameters and returns their index with probability
// proportional to their weight.
class WeightedDistribution {
 public:
  using result_type = unsigned int;
  using param_type = std::vector<double>;

  WeightedDistribution() = default;

  WeightedDistribution(param_type parameters);

  void reset();

  param_type param() const;

  void param(param_type parameters);

  template <typename GeneratorT>
  result_type operator()(GeneratorT& generator);

  template <typename GeneratorT>
  result_type operator()(GeneratorT& generator, const param_type& parameters);

  result_type min() const;

  result_type max() const;

  // Takes a map from T instances to probabilities as argument and creates a
  // matching WeightedDistribution. Also returns a map from the result_type that
  // is returned to the original T instance in the map parameter.
  template <typename MapT>
  static std::pair<WeightedDistribution,
                   std::unordered_map<result_type, typename MapT::key_type>>
  FromMap(const MapT& probabilities);

  template <typename IterT, typename StreamT>
  friend StreamT& operator>>(StreamT& is, WeightedDistribution& distribution);

 private:
  void UpdateHelperDistribution();

  template <typename GeneratorT>
  static unsigned int GenerateNumber(
      const param_type& parameters,
      std::uniform_real_distribution<double>& helper_distribution,
      GeneratorT& generator);

  param_type parameters_;
  std::uniform_real_distribution<double> helper_distribution_;
};

bool operator==(const WeightedDistribution& lhs,
                const WeightedDistribution& rhs);
bool operator!=(const WeightedDistribution& lhs,
                const WeightedDistribution& rhs);
              }}

#include "weighted_distribution.impl.h"
