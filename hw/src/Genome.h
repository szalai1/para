#ifndef GENOME_H
#define GENOME_H

#include <cstring>
#include <map>
#include <cmath>
#include <iostream>
#include <functional>

class Individual {
 public:
  Individual(const char *);
  Individual();
  Individual(Individual const &ind);
  Individual(Individual &&ind);
  Individual &operator=(Individual const &ind);
  Individual &operator=(Individual &&ind);
  int size() const {
    return length_;
  }
  char *serialize() const;
  void mutation();
  double evaluate(std::function<double(const char *)>) const;
  friend std::ostream& operator<<(std::ostream &os, Individual const &indiv);
  ~Individual() { delete[] genome_;}
 private:
  size_t length_;
  char *genome_;
  friend std::pair<Individual, Individual> crossover(Individual const &,
                                                      Individual const &);
  
};

#endif
