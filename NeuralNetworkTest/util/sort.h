#pragma once

namespace util {
namespace sort {

// Sorts a range of elements. Unlike std::sort, this function takes a
// transformation function which is supposed to map T to a comparable type. The
// latter is then used to sort the range via std::less. This allows less
// flexibility but only evaulates the transformation function once for each
// element.
template <typename IterT, typename Transformation>
void Sort(IterT left, IterT right, Transformation f);
}
}

#include "util/sort.impl.h"
