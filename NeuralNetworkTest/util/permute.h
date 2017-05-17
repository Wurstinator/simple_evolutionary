#pragma once

namespace util {
namespace permute {
// Inverts a permutation.
// input before = {3, 0, 1, 2}
// input after = {1, 2, 3, 0}
template <typename IterT>
void InvertPermutation(IterT left, IterT right);

// Performs a permutation on a sequence.
// input before = {A, B, C, D}
// permutation = {3, 0, 1, 2}
// input after = {B, C, D, A}
template <typename InputIterT, typename PermIterT>
void Permute(InputIterT input_left, InputIterT input_right,
             PermIterT permutation_left, PermIterT permutation_right);

// Transforms a permutation into a list of cyclic permutations.
// input = {0, 1, 3, 2}
// output = {{0}, {1}, {2, 3}}
template <typename PermIterT, typename OutputIterT>
void PermutationToCycles(PermIterT left, PermIterT right, OutputIterT output);

// Transforms a cyclic permutation to a list of transpositions.
// input = {0, 1, 3, 2}
// output = {{0, 1}, {0, 3}, {0, 2}}
// Note that this will be a left-to-right list, i.e. applying the cyclic
// permutation is equivalent to applying the transpositions from left to right.
template <typename IterT, typename OutputIterT>
void CycleToTranspositions(IterT left, IterT right, OutputIterT output);
}
}

#include "util/permute.impl.h"
