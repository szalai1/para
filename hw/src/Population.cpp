#include "../src/Population.h"

bool comp(std::pair<double, Individual> a, std::pair<double, Individual> b) {
  return a.first > b.first;                     
}

Population::Population(int *argc, char ***argv, std::function<double(const char*, size_t)> f) : eval_{f} {
  MPI_Init(argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks_);
  if ( *argc != 6) {
    std::cerr << "ERROR 6 parameter !!" << std::endl;
    return;
  }
  gen_ = atoi((*argv)[1]);
  pop_ = atoi((*argv)[2]);
  multi_rate_ = atoi((*argv)[3]);
  mut_rate_ = atof((*argv)[4]);
  checkpoint_num_ = atoi((*argv)[5]);
  //  std::cout << gen_  << " " << pop_ << " " << multi_rate_
  //          << " " << mut_rate_ << " " << checkpoint_num_ << std::endl;
  srand(time(nullptr));
}

void Population::init( std::function<char(size_t)> f) {
  if (rank_ < numtasks_-1) {
    pop_ /= numtasks_;
  }
  else {
    pop_ = pop_ - (pop_*(numtasks_-1) /numtasks_);
  }
  if (pop_ < multi_rate_ && multi_rate_ < 2) {
    throw std::logic_error("Population::const error");
  }
  auto best = std::make_pair(9, Individual{});
  for ( size_t ii = 0; ii < pop_; ++ii) {
    population_.push_back(std::make_pair(0, Individual{gen_, f}));
    population_[ii].first = population_[ii].second.evaluate(eval_);
    if (ii == 0 ) {
      best = population_[ii];
    }
    else if (population_[ii].first > best.first) {
       best = population_[ii];
    }
    best_ = best;
  }
  std::make_heap(population_.begin(), population_.end(), comp);
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
  for ( size_t ii = 0; ii < k; ++ii) {
    if ( ii % checkpoint_num_ == 0) {
      if (check_done(false)) {
        return ii;
      }
      if (numtasks_ > 1) {
        migrate();
      }
    }
    if (round() >= max) {
      check_done(true);
      return ii;
    }  
  }
  check_done(false);
  return k-1;
}

size_t Population::until(double max) {
  int ii = 0;
  while (round() < max) {
    ++ii;
    if ( ii % checkpoint_num_ == 0) {
      if (check_done(false)) {
        return ii;
      }
      if (numtasks_ > 1) {
        migrate();
      }
      ii = 0;
    }   
  }
  check_done(true);
  return round_counter_;
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
                  population_.end(), comp);
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
    if (pop_ == population_.size()) { return;}
    auto new_ones = crossover(population_[a].second,
                              population_[b].second);
    new_ones.first.mutation();
    double ev_1 = new_ones.first.evaluate(eval_);
    if (round_counter_ == 0 or  ev_1 > best_.first) {
      best_ = std::make_pair(ev_1, new_ones.first);
    }
    new_ones.second.mutation();
    population_.push_back(std::make_pair(ev_1, new_ones.first));
    std::push_heap(population_.begin(), population_.end(), comp);
    if (pop_ == population_.size()) { return;}
    double ev_2 = new_ones.second.evaluate(eval_);
    if (ev_2 > best_.first) {
      best_ = std::make_pair(ev_2, new_ones.second);
    }
    population_.push_back(std::make_pair(ev_2, new_ones.second));
    std::push_heap(population_.begin(), population_.end(), comp);
  }
}

bool Population::check_done(bool done) {
  bool or_done = false;
  for (int ii = 1; ii < numtasks_; ++ii) {
    bool other_done = false;
    if (rank_ == ii) {
      MPI_Send (&done, 1, MPI_C_BOOL, 0, 1, MPI_COMM_WORLD);
      if (done) {
        send_individual(0, get_best(), get_best_score(),2);
      }
    }
    if (rank_ == 0) {
      MPI_Recv(&other_done, 1, MPI_C_BOOL, ii, 1, MPI_COMM_WORLD, &status_);
      if (other_done) {
        recv_individual(ii, best_.second, best_.first, 2);
      }
      or_done = or_done or other_done or done;
    }
  }
  MPI_Bcast (&or_done,1,MPI_C_BOOL,0,MPI_COMM_WORLD);
  return or_done;
}


void Population::send_individual(int id, Individual const &indiv, double score, int tag) const {
  char *ser = indiv.serialize();
  MPI_Send (ser, indiv.get_size(), MPI_CHAR, id, tag, MPI_COMM_WORLD);
  delete[] ser;
  MPI_Send (&score, 1, MPI_DOUBLE, id, tag, MPI_COMM_WORLD);
}

void Population::recv_individual(int from, Individual &indiv, double &score, int tag) {
  size_t siz = get_best().get_size();
  char *buff = new char[siz];
  MPI_Recv(buff, siz, MPI_CHAR, from, tag, MPI_COMM_WORLD, &status_);
  indiv = Individual{buff};
  delete[] buff;
  MPI_Recv(&score, 1, MPI_DOUBLE, from, tag, MPI_COMM_WORLD, &status_);
}

void Population::migrate() {
  double *lists = broadcast();
  double threshold = compute_threshold(lists);
  kill_under_threshold(threshold);
  balance(lists, threshold);
  delete[] lists;
  offspring();
}

double *Population::broadcast() {
  double *lists = new double[numtasks_*multi_rate_];
  lowest_scores(lists + multi_rate_*rank_);
  for ( int ii = 0; ii < numtasks_; ++ii) {
    MPI_Bcast(lists + ii*multi_rate_, multi_rate_, MPI_DOUBLE, ii,MPI_COMM_WORLD);
  }
  return lists;
}

double Population::compute_threshold(double *lists) const {
  double *list_cpy = new double[numtasks_*multi_rate_];
  memcpy(list_cpy, lists, sizeof(double)*numtasks_*multi_rate_);
  std::sort(list_cpy, list_cpy + numtasks_*multi_rate_);
  double th = list_cpy[multi_rate_-1];
  delete[] list_cpy;
  return th;
}


void Population::kill_under_threshold(double threshold) {
  for (size_t ii = 0; ii < multi_rate_; ++ii) {
    std::pop_heap(population_.begin(), population_.end(), comp);
    double score = population_[population_.size()-1].first;
    if (score > threshold) {
      std::push_heap(population_.begin(), population_.end(), comp);
      return;
    }
    population_.pop_back();
  }
}

void Population::lowest_scores(double *list) {
  for ( size_t ii = 0; ii < multi_rate_; ++ii) {
    std::pop_heap(population_.begin(), population_.end()-ii, comp);
    list[ii] = population_.end()[-ii-1].first;
  }
  for ( size_t ii = 0; ii < multi_rate_; ++ii) {
    std::push_heap(population_.begin(), population_.end() - multi_rate_ + ii + 1, comp);
  }
}

void Population::balance(double *lists, double threshold) {
  size_t *multi_rate_parts = new size_t[numtasks_];
  for ( int ii = 0; ii < numtasks_; ++ii) {
    multi_rate_parts[ii] = multi_rate_;
    if ( ii < numtasks_ - 1 ) {
      multi_rate_parts[ii] /= numtasks_;
    }
    else {
      multi_rate_parts[ii] = multi_rate_parts[ii] - (multi_rate_parts[ii]*(numtasks_-1) /numtasks_);
    }
  }
  size_t to = 0;
  size_t from = 0;
  while ( from != numtasks_*multi_rate_) {
    size_t id_to = to/multi_rate_;
    size_t id_from = from/multi_rate_;
    if ( to%multi_rate_ < multi_rate_parts[id_to] ) {
      ++to;
      continue;
    }
    if ( from%multi_rate_ >= multi_rate_parts[id_from] ) {
      ++from;
      continue;
    }
    if (lists[from] <= threshold) {
      ++from ;
      continue;
    }
    if (lists[to] > threshold) {
      ++to;
      continue;
    }
    else {
      migrate_one(id_from, id_to);
      ++from;
      ++to;
    }
  }
  std::make_heap(population_.begin(), population_.end(), comp);
  delete[] multi_rate_parts;
}

void Population::migrate_one(int from, int to) {
  if ( from == rank_) {
    size_t rand_indiv = rand()%population_.size();
    send_individual(to, population_[rand_indiv].second, population_[rand_indiv].first, 3);
    population_.erase(population_.begin() + rand_indiv);
  }
  if ( to == rank_ ) {
    double score;
    Individual tmp;
    recv_individual(from, tmp, score, 3);
    population_.push_back(std::make_pair(score, tmp));
    if (score > best_.first) {
      best_ = *(population_.end()-1);
    }
  }
}


