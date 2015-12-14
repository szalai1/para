#include "Individual.h"
#include <iomanip>

std::pair<Individual, Individual> crossover(Individual const &,
                                                      Individual const &);

Individual::Individual(size_t n) {
  length_ = n;
  genome_ = new char[length_];
  for (size_t ii = 0; ii < length_; ++ii) {
    genome_[ii] = rand()%255;
  }
}

Individual::Individual(Individual const &ind) {                    
  length_ = ind.length_;
  genome_ = new char[length_];
  memcpy(genome_, ind.genome_, length_);
}

Individual::Individual(Individual &&ind) {                    
  length_ = ind.length_;
  genome_ = ind.genome_;
  ind.genome_ = nullptr;
}

Individual &Individual::operator=(Individual const &ind) {
  Individual id{ind};
  length_ = id.length_;
  genome_ = id.genome_;
  id.genome_ = nullptr;
  return *this;
}

Individual &Individual::operator=(Individual &&ind) {
  delete genome_;
  genome_ = ind.genome_;
  length_ = ind.length_;
  ind.genome_ = nullptr;
  return *this;
}

Individual::Individual(const char *cc) {
  
  memcpy(&length_, cc, sizeof(size_t));
  genome_ = new char[length_];
  memcpy(genome_, cc + sizeof(size_t),length_);
}

char *Individual::serialize() const {
  char *ser = new char[sizeof(size_t) + length_];
  memcpy(ser, &length_, sizeof(size_t));
  memcpy(ser + sizeof(size_t), genome_, length_);
  return ser;
}

void Individual::mutation() {
  size_t n =  rand()%length_;
  size_t where = rand() % 8;
  bool bit = (genome_[n] >> where)%2;
  if ( bit ) {
    genome_[n] &= ~(1u << where);
  }
  else {
    genome_[n] |= (1u << where);
  }
}

std::pair<Individual, Individual> crossover(Individual const &A,
                                            Individual const &B) {
  auto ret_val = std::make_pair(Individual{A.length_}, Individual{A.length_});
  size_t where = rand() % A.length_;
  memcpy(ret_val.first.genome_, &A.genome_ , where);
  memcpy(ret_val.first.genome_ + where,
         B.genome_ + where ,
         A.length_ - where);
  memcpy(ret_val.second.genome_, B.genome_ , where);
  memcpy(ret_val.second.genome_ + where,
         A.genome_ + where ,
         A.length_ - where);
  return ret_val;
}

double Individual::evaluate(std::function<double(const char*,size_t)> f) const {
  return f(genome_, length_);  
}

std::ostream& operator<<(std::ostream &os, Individual const &indiv) {
  double val = 0;
  memcpy(&val, &indiv.genome_, sizeof(double) );
    os << "Genome(" << indiv.length_ << " #;# ";
  //  <<", 0x"<< std::hex ;
  for (size_t ii = 0; ii < indiv.length_; ++ii) {
    os << " " << std::setfill ('0') << std::setw (2)<<   static_cast<int>(indiv.genome_[ii]);
  }
  os << " )" << std::dec;
     
  return os;  
}

