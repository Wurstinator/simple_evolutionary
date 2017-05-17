
#include <algorithm>
#include <vector>

#include <iostream>
#include "util/permute.h"
namespace util {
namespace sort {

namespace {
const bool SORT_DBG_IMPLEMENTATION = false;
}

// Sorts a range of elements. Unlike std::sort, this function takes a
// transformation function which is supposed to map T to a comparable type. The
// latter is then used to sort the range via std::less. This allows less
// flexibility but only evaulates the transformation function once for each
// element.
template <typename IterT, typename Transformation>
void Sort(IterT left, IterT right, Transformation f) {
  if (SORT_DBG_IMPLEMENTATION) {
    using T = typename IterT::value_type;
    std::sort(left, right,
              [&f](const T& lhs, const T& rhs) { return f(lhs) < f(rhs); });
    return;
  }

  // Create a list of the comparable objects.
  using ComparableT = decltype(f(*left));
  std::vector<ComparableT> comparables;
  std::transform(left, right, std::back_inserter(comparables), f);

  // Sort a list of indices to the elements.
  std::vector<unsigned int> indices(std::distance(left, right));
  std::iota(indices.begin(), indices.end(), 0);
  const auto sort_index = [&comparables](unsigned int lhs, unsigned int rhs) {
    return comparables[lhs] < comparables[rhs];
  };
  std::sort(indices.begin(), indices.end(), sort_index);

  // Apply the index permutation on the sequence.
  permute::InvertPermutation(indices.begin(), indices.end());
  permute::Permute(left, right, indices.begin(), indices.end());
}
}
}
