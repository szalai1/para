#ifndef GENOME_H
#define GENOME_H

#include <cstring>
#include <map>
#include <cmath>
#include <iostream>
#include <functional>

class Individual {
 public:
  Individual(size_t n, std::function<char(size_t)> f);
  Individual(const char *);
  Individual():length_{0}, genome_{nullptr} {}
  Individual(size_t);
  Individual(Individual const &ind);
  Individual(Individual &&ind);
  Individual &operator=(Individual const &ind);
  Individual &operator=(Individual &&ind);
  char *serialize() const;
  void mutation();
  double evaluate(std::function<double(const char *, size_t)>) const;
  friend std::ostream& operator<<(std::ostream &os, Individual const &indiv);
  size_t get_size() const { return sizeof(size_t) + length_;}
  ~Individual() { delete[] genome_;}
 private:
  size_t length_;
  char *genome_;
  friend std::pair<Individual, Individual> crossover(Individual const &,
                                                      Individual const &);
};

#endif
