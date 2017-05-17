
#include <string>

#include "gtest/gtest.h"
#include "util/sort.h"

TEST(UtilTest, SortTest) {
  struct Test {
    int v;
    bool operator==(const Test& other) const { return v == other.v; }
  };
  Test one{1}, two{2}, four{4}, six{6};
  std::vector<Test> unsorted{{four, six, two, one}};
  std::vector<Test> sorted1{{one, two, four, six}};
  const std::vector<Test> sorted2 = sorted1;
  const auto select_v = [](const Test& t) { return t.v; };
  util::sort::Sort(unsorted.begin(), unsorted.end(), select_v);
  util::sort::Sort(sorted1.begin(), sorted1.end(), select_v);

  EXPECT_EQ(sorted2, unsorted);
  EXPECT_EQ(sorted2, sorted1);
}
