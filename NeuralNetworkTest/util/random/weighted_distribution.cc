
#include "util/random/weighted_distribution.h"


namespace util {
namespace random {


WeightedDistribution::WeightedDistribution(param_type parameters)
    : parameters_(std::move(parameters)) {
  UpdateHelperDistribution();
}

void WeightedDistribution::reset() {
  parameters_.clear();
  helper_distribution_.reset();
}

WeightedDistribution::param_type WeightedDistribution::param() const {
  return parameters_;
}

void WeightedDistribution::param(param_type parameters) {
  parameters_ = std::move(parameters);
  UpdateHelperDistribution();
}

WeightedDistribution::result_type WeightedDistribution::min() const {
  return 0;
}

WeightedDistribution::result_type WeightedDistribution::max() const {
  return parameters_.size();
}

void WeightedDistribution::UpdateHelperDistribution() {
  const std::uniform_real_distribution<double>::param_type
      helper_distribution_parameters(
          0, std::accumulate(parameters_.begin(), parameters_.end(), 0));
  helper_distribution_.param(helper_distribution_parameters);
}

bool operator==(const WeightedDistribution& lhs,
                const WeightedDistribution& rhs) {
  return lhs.param() == rhs.param();
}

bool operator!=(const WeightedDistribution& lhs,
                const WeightedDistribution& rhs) {
  return !(lhs == rhs);
}

}}
