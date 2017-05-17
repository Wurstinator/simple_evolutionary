#pragma once

#include <functional>
#include <vector>

#include "evolution/evolver.h"

template <typename T>
class Process {
 public:
  using FitnessFunction = std::function<double(const T&)>;
  using Generation = std::vector<T>;

  struct Options {
    // How natural selection works.
    enum NaturalSelectionStrategy {
      // In each generation, kill all specimen until the generation size limit
      // is reached, starting from the least fit.
      KILL_PRECISE_WORST = 0,
      KILL_PROBAB_WORST = 1
    };
    NaturalSelectionStrategy natural_selection_strategy;

    // The number of specimen that are kept in each generation.
    unsigned int generation_size;

    // A subset of the first generation. If this has size less than
    // generation_size, it will be filled up with InitialSpecimen from the
    // evolver.
    std::vector<T> starting_generation{};

    // This function is evaluated exactly once after each generation to
    // determine whether the process should terminate.
    std::function<bool(const Generation&)> evolution_terminate;

    // The number of children that are caused by each pair of parents.
    int offspring_count;
  };

  // Runs a process of evolution and returns the resulting speciment.
  static Generation Evolution(Evolver<T>* evolver,
                              FitnessFunction fitness_function,
                              Options options);

  // A helper function for the Options struct. This function allows a simple
  // "cancel if X is satisfied or N generations passed".
  static std::function<bool(const Generation&)> TerminateAfterNGenerations(
      int n, std::function<bool(const Generation&)> second_condition =
                 [](const Generation&) { return false; });

 private:
  // Constructs a new process object from an evolver subclass.
  Process(Evolver<T>* evolver, FitnessFunction fitness_function,
          Options options);

  // Runs a process of evolution and returns the resulting specimen.
  Generation RunProcess() const;

  // Evolves one generation to the next.
  Generation Evolve(const Generation& old_generation) const;

  // Mates two specimen to create part of a new generation.
  Generation Mate(const T& father, const T& mother) const;

  // Kills all weak specimen.
  Generation NaturalSelection(const Generation& old_generation) const;

  // Kills all weak specimen.
  Generation NaturalSelection_KillPreciseWorst(
      const Generation& old_generation) const;

  // Kills all weak specimen.
  Generation NaturalSelection_KillProbabWorst(
      const Generation& old_generation) const;

  // Compares two T values according to their fitness.
  static bool FitnessComparison(const T& lhs, const T& rhs);

 private:
  Evolver<T>* evolver_;
  FitnessFunction fitness_function_;
  Options options_;
};

#include "evolution/process.impl.h"
