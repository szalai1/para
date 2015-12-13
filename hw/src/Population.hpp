#ifndef POPULATION
#define POPULATION
#include <exception>
#include <vector>
#include <map>
#include <algorithm>
#include "../src/Genome.h"

class Population {
public:
  Population(size_t n,
             size_t gen,
             double mutation,
             size_t multiply,
             std::function<double(const char*, size_t)> f)
    :mut_rate_{mutation},
    multi_rate_{multiply},
    pop_{n},
    round_counter_{0},
    gen_{gen},
    eval_{f}
  {
    if (n < multiply && multi_rate_ < 2) {
      throw std::logic_error("Population::const error");
    }
    for ( size_t ii = 0; ii < pop_; ++ii) {
      population_.push_back(std::make_pair(0,Individual{gen}));
      population_[ii].first = population_[ii].second.evaluate(eval_);
    }
    std::make_heap(population_.begin(),
                   population_.end(),
                   [](std::pair<double, Individual> a,
                      std::pair<double, Individual> b) {
                     return a.first > b.first;                     
                   });
  }
  double round() {
    kill();
    offspring();
    round_counter_+=1;
    return get_best_score();
  }
  double round(size_t k) {
    for ( size_t ii = 0; ii < k; ++ii) { round(); }
    return get_best_score();
  }

  size_t round(size_t k, double max) {
    for ( size_t ii = 0; ii < k; ++ii) {
      if (round() >= max) {
        return ii;
      }  
    }
    return k-1;
  }
  double get_best_score() {
    return best_.first;
  }
  Individual& get_best() {
    return best_.second;
  }
private:
  void kill() {
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
  void offspring() {
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
  std::vector<std::pair<double,Individual>> population_;
  std::pair<double, Individual> best_;
  double mut_rate_;
  size_t multi_rate_;
  size_t pop_;
  size_t round_counter_;
  size_t gen_;
  std::function<double(const char *,size_t)> eval_;
};


#endif
