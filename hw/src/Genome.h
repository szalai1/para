<<<<<<< HEAD
<<<<<<< HEAD:hw/Genome.h

=======
#include <functional>
#include <cstring>
>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.h
=======
#ifndef GENOME_H
#define GENOME_H

#include <cstring>
#include <map>
#include <cmath>
#include <iostream>
#include <functional>
>>>>>>> 55f7f87ba25cb959725232be2c45f9baec682df1

class Individual {
 public:
<<<<<<< HEAD
  Genome() = delete;
<<<<<<< HEAD:hw/Genome.h
  Genome(size_t lg, float m);
  Genome(Genome&&);
  Genome &operator=(Genome &);
  Genome &operator=(Genome &&);
=======
  Genome(size_t, float,
         std::function<char(size_t)> f = [](size_t s)->char
           {return rand() % 255; } );
  Genome(const Genome &);
  Genome(Genome&&);
  double evaluate(std::function<double(char*,size_t)>) const;
>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.h
  Genome get_begin(size_t) const;
  Genome get_end(size_t) const;
=======
  Individual(const char *);
  Individual();
  Individual(Individual const &ind);
  Individual(Individual &&ind);
  Individual &operator=(Individual const &ind);
  Individual &operator=(Individual &&ind);
>>>>>>> 55f7f87ba25cb959725232be2c45f9baec682df1
  int size() const {
    return length_;
  }
  float mutation_rate() {
    return mutation_rate_;
  }
  char *serialize() const;
<<<<<<< HEAD
<<<<<<< HEAD:hw/Genome.h
  Genome append(Genome&);
  void deserialize(const char *);
=======
  Genome& append(const Genome&);
>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.h
  char const *get_data();
  void mutation();
  void dot_mutatuin(size_t);
  char *serilize() const;
  ~Genome() {
    delete[] genome_;
  }
<<<<<<< HEAD:hw/Genome.h
=======
  Genome operator=(const Genome &);
  Genome& operator=(Genome &&);
  void dot_mutation(size_t);
>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.h
=======
  void mutation();
  double evaluate(std::function<double(const char *)>) const;
  friend std::ostream& operator<<(std::ostream &os, Individual const &indiv);
  ~Individual() { delete[] genome_;}
>>>>>>> 55f7f87ba25cb959725232be2c45f9baec682df1
 private:
  size_t length_;
  char *genome_;
  friend std::pair<Individual, Individual> crossover(Individual const &,
                                                      Individual const &);
  
};

#endif
