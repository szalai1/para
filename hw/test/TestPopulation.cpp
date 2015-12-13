#include <gtest/gtest.h>
#include <iostream>
#include "../src/Population.hpp"
#include "../src/Genome.h"

TEST(Population, Poptest) {

  Population<Individual> pop{10, 0.1, 2};
  pop.round(100);
  std::cout << "best "  << pop.get_best_score() << std::endl;
}
