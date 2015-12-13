#include <gtest/gtest.h>
#include <iostream>
#include <functional>
#include "../src/Population.hpp"
#include "../src/Genome.h"

double eval(const char *genome_, size_t length_) {
  int score = 0;
  std::vector<bool> check(length_, false);
  std::vector<bool> diag(2*length_, false);
  std::vector<bool> diag2(2*length_, false);
  for (size_t ii = 0; ii < length_; ++ii) {
    size_t num = static_cast<size_t>(genome_[ii]);
    if (num > length_ or num < 0) {
      score += 100;
    }
    else {
      if (check[num]) {
        score += 1;
      }
      else {
        check[num] = true;
      }
      if (diag[length_ + ii-num]) {
        score+=1;
      }
      else {
        diag[length_ + ii-num] = true;
      }
      if (diag2[ii+num]) {
        score+=1;
      }
      else {
        diag2[ii+num] = true;
      }
    }
  }
  return -score;
}


TEST(Population, Poptest) {
  Population pop{1000, 20, 0.1, 100, eval};
  std:: cout << pop.round(5000, 0) << " round" << std::endl;
  std::cout << "best "  << pop.get_best_score() << std::endl;
  std::cout << pop.get_best().evaluate(eval) << std::endl;;
  std::cout << "best "  << pop.get_best() << std::endl;
}

