#ifndef POPULATION
#define POPULATION
#include <exception>
#include <vector>
#include <map>
#include <algorithm>
#include <mpi.h>
#include "../src/Individual.h"


class Population {
public:
  Population(size_t , size_t , double ,  size_t , std::function<double(const char*, size_t)> );
  void init(int *, char ***);
  double round();
  double round(size_t k);
  size_t round(size_t k, double max);
  double get_best_score();
  Individual& get_best();
  ~Population() {MPI_Finalize();}
  int get_rank() {return rank_;}
private:
  bool check_done(bool);
  void kill();
  void offspring();
  std::vector<std::pair<double,Individual>> population_;
  std::pair<double, Individual> best_;
  double mut_rate_;
  size_t multi_rate_;
  size_t pop_;
  size_t round_counter_;
  size_t gen_;
  int numtasks_;
  int rank_;
  int checkpoint_num_;
  std::function<double(const char *,size_t)> eval_;
  MPI_Status status_;
};
#endif
