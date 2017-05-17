
#include <algorithm>

#include "util/random/probabilistic_sort.h"
#include "util/sort.h"

template <typename T>
bool Process<T>::FitnessComparison(const T& lhs, const T& rhs) {
  return FitnessComparison(lhs) > FitnessComparison(rhs);
}

template <typename T>
typename Process<T>::Generation Process<T>::Evolution(
    Evolver<T>* evolver, FitnessFunction fitness_function, Options options) {
  Process process(evolver, std::move(fitness_function), std::move(options));
  return process.RunProcess();
}

template <typename T>
Process<T>::Process(Evolver<T>* evolver, FitnessFunction fitness_function,
                    Options options)
    : evolver_(evolver),
      fitness_function_(std::move(fitness_function)),
      options_(std::move(options)) {}

template <typename T>
typename Process<T>::Generation Process<T>::RunProcess() const {
  // Construct initial generation of random specimen.
  Generation current_generation = options_.starting_generation;
  while (current_generation.size() < options_.generation_size) {
    current_generation.push_back(evolver_->InitialSpecimen());
  }
  current_generation.erase(current_generation.begin() + options_.generation_size, current_generation.end());

  // Run through the generations.
  while (!options_.evolution_terminate(current_generation)) {
    current_generation = NaturalSelection(Evolve(current_generation));
  }
  return current_generation;
}

template <typename T>
typename Process<T>::Generation Process<T>::Evolve(
    const Generation& old_generation) const {
  // Mate all specimen with each other.
  Generation new_generation;
  for (auto iter1 = old_generation.begin(); iter1 != old_generation.end();
       ++iter1) {
    for (auto iter2 = std::next(iter1); iter2 != old_generation.end();
         ++iter2) {
      Generation offspring = Mate(*iter1, *iter2);
      std::move(offspring.begin(), offspring.end(),
                std::back_inserter(new_generation));
    }
  }
  return new_generation;
}

template <typename T>
typename Process<T>::Generation Process<T>::Mate(const T& father,
                                                 const T& mother) const {
  Generation offspring;
  const auto mate_and_mutate = [this, &father, &mother]() {
    return evolver_->Mutate(evolver_->Mate(father, mother));
  };
  std::generate_n(std::back_inserter(offspring), options_.offspring_count,
                  mate_and_mutate);
  return offspring;
}

template <typename T>
typename Process<T>::Generation Process<T>::NaturalSelection(
    const Generation& old_generation) const {
  switch (options_.natural_selection_strategy) {
    case Options::KILL_PRECISE_WORST:
      return NaturalSelection_KillPreciseWorst(old_generation);
    case Options::KILL_PROBAB_WORST:
      return NaturalSelection_KillProbabWorst(old_generation);
    default:
      throw "Unsupported natural selection strategy.";
  }
}

template <typename T>
typename Process<T>::Generation Process<T>::NaturalSelection_KillPreciseWorst(
    const Generation& old_generation) const {
  // Sort the old generation by descending fitness.
  Generation new_generation = old_generation;
  const auto negative_fitness = [this](const T& specimen) {
    return -fitness_function_(specimen);
  };
  ::util::sort::Sort(new_generation.begin(), new_generation.end(),
                     negative_fitness);

  // Only keep the first n elements in the sorted list.
  new_generation.erase(new_generation.begin() + options_.generation_size,
                       new_generation.end());
  return new_generation;
}

template <typename T>
typename Process<T>::Generation Process<T>::NaturalSelection_KillProbabWorst(
    const Generation& old_generation) const {
  // Sort the old generation approximately by descending fitness.
  Generation new_generation = old_generation;
  ::util::random::ProbabilisticSort(new_generation.begin(),
                                    new_generation.end(), fitness_function_);

  // Only keep the first n elements in the sorted list.
  new_generation.erase(new_generation.begin() + options_.generation_size,
                       new_generation.end());
  return new_generation;
}

template <typename T>
std::function<bool(const typename Process<T>::Generation&)>
Process<T>::TerminateAfterNGenerations(
    int n, std::function<bool(const Generation&)> second_condition) {
  struct f {
    int i;
    std::function<bool(const Generation&)> second_condition;

    bool operator()(const Generation& generation) {
      return second_condition(generation) || i-- <= 0;
    }
  };
  return f{n, std::move(second_condition)};
}
