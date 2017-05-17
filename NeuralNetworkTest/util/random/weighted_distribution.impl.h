

namespace util {
namespace random {


template <typename GeneratorT>
WeightedDistribution::result_type WeightedDistribution::operator()(
    GeneratorT& generator) {
  return GenerateNumber(parameters_, helper_distribution_, generator);
}

template <typename GeneratorT>
WeightedDistribution::result_type WeightedDistribution::operator()(
    GeneratorT& generator, const param_type& parameters) {
  const std::uniform_real_distribution<double> helper_distribution(
      0, std::accumulate(parameters_.begin(), parameters_.end(), 0));
  return GenerateNumber(parameters, helper_distribution, generator);
}

template <typename GeneratorT>
unsigned int WeightedDistribution::GenerateNumber(
    const param_type& parameters,
    std::uniform_real_distribution<double>& helper_distribution,
    GeneratorT& generator) {
  double r = helper_distribution(generator);
  unsigned int i = 0;
  while (r >= 0) {
    r -= parameters[i++];
  }
  return i - 1;
}

template <typename MapT>
std::pair<WeightedDistribution,
          std::unordered_map<WeightedDistribution::result_type,
                             typename MapT::key_type>>
WeightedDistribution::FromMap(const MapT& probabilities) {
  std::unordered_map<WeightedDistribution::result_type, typename MapT::key_type>
      back_map;

  // Create vector of weights and fill the back map.
  std::vector<double> distribution_weights;
  for (const typename MapT::value_type& kv_pair : probabilities) {
    back_map[distribution_weights.size()] = kv_pair.first;
    distribution_weights.push_back(kv_pair.second);
  }

  // Construct the distribution and return.
  WeightedDistribution distribution(distribution_weights);
  return std::make_pair(distribution, back_map);
}

template <typename StreamT>
StreamT& operator<<(StreamT& os, const WeightedDistribution& distribution) {
  for (double w : distribution.param()) {
    os << w << ' ';
  }
  return os;
}

template <typename StreamT>
StreamT& operator>>(StreamT& is, WeightedDistribution& distribution) {
  double w;
  std::vector<double> parameters;
  while (is >> w) {
    parameters.push_back(w);
  }
  distribution.param(std::move(parameters));
  distribution.UpdateHelperDistribution();
  return is;
}
}}
