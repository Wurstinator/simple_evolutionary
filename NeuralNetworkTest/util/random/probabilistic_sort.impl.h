
#include <type_traits>

#include "util/random/util.h"
#include "util/random/weighted_distribution.h"


namespace util {
namespace random {


template <typename RandomIter>
void ProbabilisticSortHelper(RandomIter left, RandomIter right,
                             WeightedDistribution* distribution) {
  // If the sequence is empty, nothing is do to.
  if (left == right) return;

  // The distribution generates the index of the "minimal" element, i.e. the
  // element that will be placed at index 0. We swap that element to that
  // position.
  const unsigned int first_index = (*distribution)(*StaticGenerator());
  std::swap(*left, *(left + first_index));

  // Update the distribution parameters in the same way.
  WeightedDistribution::param_type parameters = distribution->param();
  std::swap(parameters[0], parameters[first_index]);
  parameters.erase(parameters.begin());
  distribution->param(std::move(parameters));

  // Recursively sort the rest.
  ProbabilisticSortHelper(left + 1, right, distribution);
}

template <typename RandomIter>
void ProbabilisticSort(RandomIter left, RandomIter right) {
  // Dereferencing RandomIter might give e.g. int. Then IterCref = const int&.
  using IterCref = typename std::add_const<
      typename std::add_lvalue_reference<decltype(*left)>::type>::type;
  const auto identity_weighter = [](IterCref x) { return x; };
  ProbabilisticSort(left, right, identity_weighter);
}

template <typename RandomIter, typename Weighter>
void ProbabilisticSort(RandomIter left, RandomIter right, Weighter weighter) {
  // Create weighted distribution.
  std::vector<double> weight_list;
  std::transform(left, right, std::back_inserter(weight_list), weighter);
  WeightedDistribution distribution(weight_list);

  // Let the helper function handle the rest.
  ProbabilisticSortHelper(left, right, &distribution);
}
}}
