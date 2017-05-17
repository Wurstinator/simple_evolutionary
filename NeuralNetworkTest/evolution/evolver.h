#pragma once

template <typename T>
class Evolver {
 public:
  ~Evolver() = default;

  // Generates a new specimen. Usually this is a random instance of T.
  virtual T InitialSpecimen() = 0;

  // Given two parent specimen, constructs a new one.
  virtual T Mate(const T& father, const T& mother) = 0;

  // Given one single speciment, performs a random mutation.
  virtual T Mutate(const T& specimen) = 0;
};
