#include "Population.h"
#include "Individual.h"
#include <mpi.h>
#include <iostream>
#include <cstdlib>

double eval(const char *genome_, size_t length_) {
  int score = 0;
  std::vector<bool> check(length_, false);
  std::vector<bool> diag(2*length_, false);
  std::vector<bool> diag2(2*length_, false);
  for (size_t ii = 0; ii < length_; ++ii) {
    unsigned char num = static_cast<unsigned char>(genome_[ii]);
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

double eval2(const char *genome_, size_t length_) {
  if ( length_ == sizeof(double)) {
    double x;
    memcpy( &x, genome_, sizeof(double));
    return sin(x);
  }
  return 0;
}

int main(int argc, char **argv) {
  Population pop{&argc, &argv, eval};
  pop.init([](size_t){return rand()%8;});
  pop.until(0);
  if ( pop.get_rank() == 0) {
    std::cout << "best score:\t"  << pop.get_best_score() << std::endl;
    std::cout << "best indiv:\t"  << pop.get_best() << std::endl;
  }
}
