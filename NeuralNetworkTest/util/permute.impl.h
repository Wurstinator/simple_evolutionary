
#include <algorithm>
#include <unordered_set>

namespace util {
namespace permute {

template <typename IterT>
void InvertPermutation(IterT left, IterT right) {
  std::vector<unsigned int> result(std::distance(left, right), 0);
  unsigned int i = 0;
  for (IterT iter = left; iter != right; ++iter) {
    result[*iter] = i++;
  }
  std::copy(result.begin(), result.end(), left);
}

template <typename InputIterT, typename PermIterT>
void Permute(InputIterT input_left, InputIterT input_right,
             PermIterT permutation_left, PermIterT permutation_right) {
  // Convert the permutation to cycles.
  std::vector<std::vector<unsigned int>> cycles;
  PermutationToCycles(permutation_left, permutation_right,
                      std::back_inserter(cycles));

  // Convert the cycles to transpositions.
  std::vector<std::vector<std::pair<unsigned int, unsigned int>>> split_cycles;
  const auto cycle_to_transpositions =
      [](const std::vector<unsigned int>& cycle) {
        std::vector<std::pair<unsigned int, unsigned int>> transpositions;
        CycleToTranspositions(cycle.begin(), cycle.end(),
                              std::back_inserter(transpositions));
        return transpositions;
      };
  std::transform(cycles.begin(), cycles.end(), std::back_inserter(split_cycles),
                 cycle_to_transpositions);

  // Apply the transpositions.
  for (const std::vector<std::pair<unsigned int, unsigned int>>&
           transpositions : split_cycles) {
    for (const std::pair<unsigned int, unsigned int>& transposition :
         transpositions) {
      const InputIterT lhs = std::next(input_left, transposition.first);
      const InputIterT rhs = std::next(input_left, transposition.second);
      std::swap(*lhs, *rhs);
    }
  }
}

template <typename PermIterT, typename OutputIterT>
void PermutationToCycles(PermIterT left, PermIterT right, OutputIterT output) {
  // TODO this can be improved e.g. with union-find
  std::unordered_set<unsigned int> indices_todo;
  for (unsigned int i = 0; i < std::distance(left, right); ++i) {
    indices_todo.insert(i);
  }

  while (!indices_todo.empty()) {
    std::vector<unsigned int> cycle{*indices_todo.begin()};
    PermIterT iter = std::next(left, cycle[0]);
    while (*iter != cycle.front()) {
      cycle.push_back(*iter);
      iter = std::next(left, *iter);
    }
    for (unsigned int i : cycle) {
      indices_todo.erase(indices_todo.find(i));
    }
    *(output++) = cycle;
  }
}

template <typename IterT, typename OutputIterT>
void CycleToTranspositions(IterT left, IterT right, OutputIterT output) {
  for (IterT iter = std::next(left); iter != right; ++iter) {
    const std::pair<unsigned int, unsigned int> transposition(*left, *iter);
    *(output++) = transposition;
  }
}
}
}
