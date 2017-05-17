
#include <set>

#include "gtest/gtest.h"
#include "util/permute.h"

TEST(PermuteTest, CycleToTranspositionsTest) {
  const std::vector<unsigned int> cycle{0, 1, 5, 8};
  std::vector<std::pair<unsigned int, unsigned int>> output;
  util::permute::CycleToTranspositions(cycle.begin(), cycle.end(),
                                       std::back_inserter(output));
  ASSERT_EQ(3, output.size());
  EXPECT_EQ((std::make_pair<unsigned int, unsigned int>(0, 1)), output[0]);
  EXPECT_EQ((std::make_pair<unsigned int, unsigned int>(0, 5)), output[1]);
  EXPECT_EQ((std::make_pair<unsigned int, unsigned int>(0, 8)), output[2]);
}

TEST(PermuteTest, PermutationToCyclesTest) {
  // Split into cycles.
  const std::vector<unsigned int> permutation{1, 0, 2, 4, 5, 3};
  std::set<std::vector<unsigned int>> cycles;
  util::permute::PermutationToCycles(permutation.begin(), permutation.end(),
                                     std::inserter(cycles, cycles.end()));
  ASSERT_EQ(3, cycles.size());

  // We have to test for all possible combinations of a cycle.
  const auto is_cycle = [&cycles](const std::vector<unsigned int>& v) {
    return cycles.find(v) != cycles.end();
  };
  const std::vector<unsigned int> cycle_01{0, 1}, cycle_10{1, 0}, cycle_2{2},
      cycle_345{3, 4, 5}, cycle_534{5, 3, 4}, cycle_453{4, 5, 3};
  EXPECT_TRUE(is_cycle(cycle_01) || is_cycle(cycle_10));
  EXPECT_TRUE(is_cycle(cycle_2));
  EXPECT_TRUE(is_cycle(cycle_345) || is_cycle(cycle_534) ||
              is_cycle(cycle_453));
}

TEST(PermuteTest, PermuteTest) {
  const std::vector<unsigned int> permutation{4, 1, 0, 2, 3};
  std::vector<char> list{'A', 'B', 'C', 'D', 'E'};
  util::permute::Permute(list.begin(), list.end(), permutation.begin(),
                         permutation.end());
  EXPECT_EQ('C', list[0]);
  EXPECT_EQ('B', list[1]);
  EXPECT_EQ('D', list[2]);
  EXPECT_EQ('E', list[3]);
  EXPECT_EQ('A', list[4]);
}

TEST(PermuteTest, InvertPermutationTest) {
  const std::vector<unsigned int> permutation{4, 1, 0, 2, 3};
  std::vector<unsigned int> inverted = permutation;
  const std::vector<unsigned int> inverted_ref{2, 1, 3, 4, 0};
  const std::vector<unsigned int> identity{0, 1, 2, 3, 4};
  util::permute::InvertPermutation(inverted.begin(), inverted.end());
  EXPECT_EQ(inverted_ref, inverted);

  std::vector<unsigned int> test_vec = identity;
  util::permute::Permute(test_vec.begin(), test_vec.end(), inverted.begin(),
                         inverted.end());
  util::permute::Permute(test_vec.begin(), test_vec.end(), permutation.begin(),
                         permutation.end());
  EXPECT_EQ(identity, test_vec);
}
