#pragma once


namespace util {
namespace random {

// Sorts a list using the given weight. The chance that an element will be the
// first in the sorted list is proportional to the
template <typename RandomIter>
void ProbabilisticSort(RandomIter left, RandomIter right);

template <typename RandomIter, typename Weighter>
void ProbabilisticSort(RandomIter left, RandomIter right, Weighter weighter);
}}

#include "probabilistic_sort.impl.h"
