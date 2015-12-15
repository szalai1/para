#ifndef POPULATION
#define POPULATION
#include <exception>
#include <vector>
#include <map>
#include <algorithm>
#include <mpi.h>
#include <functional>
#include "../src/Individual.h"
#include <time.h>
#include <stdlib.h>


class Population {
public:
  Population(size_t , size_t , double ,  size_t , std::function<double(const char*, size_t)> );
  void init(int *, char ***,std::function<char(size_t)> f = [](size_t){return rand()%255;});
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
  void send_individual(int id, Individual const &indiv, double score) const;
  void recv_individual(int from, Individual &indiv, double& score);
  void migrate();
  double compute_threshold(double *lists) const;
  double* broadcast();
  void kill_under_threshold(double threshold);
  void balance(double *, double);
  void lowest_scores(double*);
  void migrate_one(int from, int to);
};
#endif
