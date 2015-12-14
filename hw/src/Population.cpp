#include <exception>
#include <vector>
#include <map>
#include <algorithm>
#include "../src/Population.h"

Population::Population(size_t n,
           size_t gen,
           double mutation,
           size_t multiply,
           std::function<double(const char*, size_t)> f)
  :mut_rate_{mutation},
   multi_rate_{multiply},
   pop_{n},
   round_counter_{0},
   gen_{gen},
   eval_{f} {}

void Population::init(int *argc, char ***argv) {
  std::cout << "init" << std::endl;
  MPI_Init(argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks_);
  std::cout << "MPI init done " << std::endl;
  if (*argc == 2) {
    std::cout << "CSK KKK " << (*argv)[1] << std::endl;
    checkpoint_num_ = atoi((*argv)[1]);
  }
  else {
  }
  std::cout << rank_ << "  " << numtasks_ << std::endl;
  if (rank_ < numtasks_-1) {
    pop_ /= numtasks_;
    multi_rate_ /= numtasks_;
  }
  else {
    multi_rate_ = multi_rate_ - (multi_rate_*(numtasks_-1) /numtasks_);
    pop_ = pop_ - (pop_*(numtasks_-1) /numtasks_);
  }
  if (pop_ < multi_rate_ && multi_rate_ < 2) {
    throw std::logic_error("Population::const error");
  }
  std::cout << "pop: " << pop_ << std::endl;
  for ( size_t ii = 0; ii < pop_; ++ii) {
    population_.push_back(std::make_pair(0,Individual{gen_}));
    population_[ii].first = population_[ii].second.evaluate(eval_);
  }
  std::make_heap(population_.begin(),
                 population_.end(),
                 [](std::pair<double, Individual> a,
                    std::pair<double, Individual> b) {
                   return a.first > b.first;                     
                 });
  std::cout << "population init done" << std::endl;
}

double Population::round() {
  kill();
  offspring();
  round_counter_+=1;
  return get_best_score();
}

double Population::round(size_t k) {
    for ( size_t ii = 0; ii < k; ++ii) { round(); }
    return get_best_score();
  }

size_t Population::round(size_t k, double max) {
  std::cout << "CSK " << checkpoint_num_ << std::endl;
  for ( size_t ii = 0; ii < k; ++ii) {
    if ( ii % checkpoint_num_ == 0 && check_done(false)) {
      return ii;
    }
    if (round() >= max) {
      std::cout << "\t\tI send true" << std::endl;
      check_done(true);
      return ii;
    }  
  }
  return k-1;
}

double Population::get_best_score() {
  return best_.first;
}

Individual& Population::get_best() {
  return best_.second;
}

void Population::kill() {
  for (size_t ii = 0; ii < multi_rate_; ++ii) {
    std::pop_heap(population_.begin(),
                  population_.end(),
                  [](std::pair<double, Individual> a,
                     std::pair<double, Individual> b) {
                    return a.first > b.first;
                  });
    population_.pop_back();
  }
}

void Population::offspring() {
  std::vector<Individual> offspring;
  for( size_t ii = pop_-multi_rate_; ii < pop_; ii+=2) {
    size_t a = rand()%(ii);
    size_t b = rand()%(ii);
    if ( a==b) {
      if ( a == 0) {
        a+=1;
      }
      else {
        a-=1;
      }
    }
    auto new_ones = crossover(population_[a].second,
                              population_[b].second);
    new_ones.first.mutation();
    double ev_1 = new_ones.first.evaluate(eval_);
    if (round_counter_ == 0 or  ev_1 > best_.first) {
      best_ = std::make_pair(ev_1, new_ones.first);
    }
    new_ones.second.mutation();
    double ev_2 = new_ones.second.evaluate(eval_);
    if (ev_2 > best_.first) {
      best_ = std::make_pair(ev_2, new_ones.second);
    }
    population_.push_back(std::make_pair(ev_1, new_ones.first));
    std::push_heap(population_.begin(),
                   population_.end(),
                   [](std::pair<double, Individual> a,
                      std::pair<double, Individual> b) {
                     return a.first > b.first;
                   });
    population_.push_back(std::make_pair(ev_2, new_ones.second));
    std::push_heap(population_.begin(),
                   population_.end(),
                   [](std::pair<double, Individual> a,
                      std::pair<double, Individual> b) {
                     return a.first > b.first;
                   });
  }
}

bool Population::check_done(bool done) {
  bool or_done = false;
  bool other_done = false;
  for (int ii = 1; ii < numtasks_; ++ii) {
    if (rank_ == ii) {
      if ( done ) {
        std::cout << rank_ << ": send " <<  (done? " true" : " false") << std::endl;
      }
      MPI_Send (&done,
                1,
                MPI_C_BOOL,
                0,
                0,
                MPI_COMM_WORLD);
      if (done) {
        std::cout << rank_ << ": done" << std::endl;
        char *best_ser = get_best().serialize();
        MPI_Send (best_ser,
                  get_best().get_size(),
                  MPI_CHAR,
                  0,
                  0,
                  MPI_COMM_WORLD);
        std::cout << "send:   " << best_ser << std::endl;
        delete[] best_ser;
        double best_score = get_best_score();
        MPI_Send (&best_score,
                  1,
                  MPI_DOUBLE,
                  0,
                  0,
                  MPI_COMM_WORLD);
      }
    }
    if (rank_ == 0) {
      MPI_Recv(&other_done,
               1,
               MPI_C_BOOL,
               ii,
               0,
               MPI_COMM_WORLD,
               &status_);
      if (other_done) {
        or_done = true;
        std::cout << rank_ << ": knows "<< ii << " done" << std::endl;
        size_t siz = get_best().get_size();
        char *buff = new char[siz];
        MPI_Recv(buff,
                 siz,
                 MPI_CHAR,
                 ii,
                 0,
                 MPI_COMM_WORLD,
                 &status_);
        best_ = std::make_pair(0, Individual{buff});
        std::cout << "receev:   "<< buff << std::endl;
        std::cout << rank_ << "\n  " << Individual{buff} << std::endl;
        delete[] buff;
        double best_score = 0;
        MPI_Recv(&best_score,
                 1,
                 MPI_DOUBLE,
                 ii,
                 0,
                 MPI_COMM_WORLD,
                 &status_);
        best_.first = best_score;
        std::cout << "MAKE BEST PAIR" << std::endl;
      }
      if ( done) {
        or_done = true;
      }
      if ( other_done ) { or_done = true;}
    }
  }
  MPI_Bcast (&or_done,1,MPI_C_BOOL,0,MPI_COMM_WORLD);
  return or_done;
}
