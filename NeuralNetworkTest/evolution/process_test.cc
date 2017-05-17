
#include <vector>

#include "evolution/process.h"
#include "gtest/gtest.h"

using IntProcess = Process<int>;

class EvolverForTest : public Evolver<int> {
 public:
  int i = 0;
  int InitialSpecimen() override { return ++i; }

  int Mate(const int& father, const int& mother) override {
    return father + mother;
  }

  int Mutate(const int& specimen) override { return specimen - 1; }
};

double FitnessFunctionForTest(const int& specimen) { return specimen; }

// Generation 0: 1, 2, 3
//    Offspring: 3, 3, 4, 4, 5, 5
//      Mutated: 2, 2, 3, 3, 4, 4
// Generation 1: 4, 4, 3
//    Offspring: 8, 8, 7, 7, 7, 7
//      Mutated: 7, 7, 6, 6, 6, 6
// Generation 1: 7, 7, 6
TEST(ProcessTest, EvolutionTest) {
  // Setup options.
  std::vector<IntProcess::Generation> generations;
  const auto register_generation =
      [&generations](const IntProcess::Generation& generation) {
        generations.push_back(generation);
        std::sort(generations.back().begin(), generations.back().end());
        return false;
      };
  IntProcess::Options options;
  options.natural_selection_strategy = KILL_PRECISE_WORST;
  options.generation_size = 3;
  options.evolution_terminate =
      IntProcess::TerminateAfterNGenerations(2, register_generation);
  options.offspring_count = 2;

  // Test.
  EvolverForTest evolver;
  IntProcess::Generation generation =
      IntProcess::Evolution(&evolver, &FitnessFunctionForTest, options);
  std::sort(generation.begin(), generation.end());
  ASSERT_EQ(generations.size(), 3);
  EXPECT_EQ(generations.back(), generation);
  EXPECT_EQ(generations[0][0], 1);
  EXPECT_EQ(generations[0][1], 2);
  EXPECT_EQ(generations[0][2], 3);
  EXPECT_EQ(generations[1][0], 3);
  EXPECT_EQ(generations[1][1], 4);
  EXPECT_EQ(generations[1][2], 4);
  EXPECT_EQ(generations[2][0], 6);
  EXPECT_EQ(generations[2][1], 7);
  EXPECT_EQ(generations[2][2], 7);
}
