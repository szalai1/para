#include "Genome.h"
#include <iomanip>

<<<<<<< HEAD
<<<<<<< HEAD:hw/Genome.cpp
using Genome;
=======
>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.cpp
=======
std::pair<Individual, Individual> crossover(Individual const &,
                                                      Individual const &);
>>>>>>> 55f7f87ba25cb959725232be2c45f9baec682df1

Individual::Individual() {
  length_ = sizeof(double);
  genome_ = new char[length_];
  for (size_t ii = 0; ii < length_; ++ii) {
    genome_[ii] = rand()%255;
  }
}

<<<<<<< HEAD
<<<<<<< HEAD:hw/Genome.cpp
Genome(size_t len, float mutation_rate)  :length_{len},
  mutation_rate_{mutation_rate}
{
  genome_ = new char[length_];
  for ( int ii = 0; ii < length_; ++ii) {
    genome_[ii] = random % 255;
  }
}

Genome(Genome &&other) 
  : length_{other.length_}, 
  mutation_rate_{other.mutation_rate_},
  genome_{other.genome} {
    other.geneme_ = nullptr; 
}

Genome get_begin(size_t k) const {
  if (k >= length ) {
=======
Genome::Genome(size_t len,
               float mutation_rate,
               std::function<char(size_t)> f)  :length_(len),
                                          mutation_rate_(mutation_rate)
{
=======
Individual::Individual(Individual const &ind) {                    
  length_ = ind.length_;
>>>>>>> 55f7f87ba25cb959725232be2c45f9baec682df1
  genome_ = new char[length_];
  memcpy(genome_, ind.genome_, length_);
}

<<<<<<< HEAD
Genome Genome::get_begin(size_t k) const {
  if (k >= length_ ) {
>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.cpp
    return Genome{0, mutation_rate_};
  } 
  Genome begin_genome{k, mutation_rate_};
  for ( size_t ii = 0; ii < k; ++ii ) {
    begin_genome.genome_[ii] = genome_[ii];
  }
  return begin_genome;
=======
Individual::Individual(Individual &&ind) {                    
  length_ = ind.length_;
  genome_ = ind.genome_;
  ind.genome_ = nullptr;
>>>>>>> 55f7f87ba25cb959725232be2c45f9baec682df1
}

Individual &Individual::operator=(Individual const &ind) {
  Individual id{ind};
  length_ = id.length_;
  genome_ = id.genome_;
  id.genome_ = nullptr;
  return *this;
}

<<<<<<< HEAD
<<<<<<< HEAD:hw/Genome.cpp

=======
Genome& Genome::append(const Genome &g) {
>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.cpp
  size_t len = length_ + g.length_;
  char *tmp = new char[len];
  memcpy(tmp, genome_, length_);
  memcpy(tmp + length_, g.genome_, g.length_);
  delete[] genome_;
  length_ = len;
  genome_ = tmp;
=======
Individual &Individual::operator=(Individual &&ind) {
  delete genome_;
  genome_ = ind.genome_;
  length_ = ind.length_;
  ind.genome_ = nullptr;
>>>>>>> 55f7f87ba25cb959725232be2c45f9baec682df1
  return *this;
}

<<<<<<< HEAD:hw/Genome.cpp
char *serialize() const {
=======

<<<<<<< HEAD
char *Genome::serialize() const {
>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.cpp
  char *ser = new char[sizeof(float) + sizeof(size_t) + length_];
=======
Individual::Individual(const char *cc):Individual() {
  memcpy(&length_, cc, sizeof(size_t));
  memcpy(&genome_, cc + sizeof(size_t),length_);
}

char *Individual::serialize() const {
  char *ser = new char[sizeof(size_t) + length_];
>>>>>>> 55f7f87ba25cb959725232be2c45f9baec682df1
  memcpy(ser, &length_, sizeof(size_t));
  memcpy(ser + sizeof(size_t), &genome_, length_);
  return ser;
<<<<<<< HEAD:hw/Genome.cpp
}

void deserialize(const char* ser) {
  memcpy(ser, lenght_, sizeof(size_t));
  memcpy(ser + sizeof(size_t), mutation_rate_, sizeof(float));
  genome_ = new char[length_];
  memcpy(ser + sizeof(size_t) + sizeof(float), genome_, length_);
=======
>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.cpp
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

<<<<<<< HEAD
Genome & operator=(Genome &rhs){
  delete[] genome_;
  geneom_ = new char[rhs.length_];
  memcpy(genome_, rhs.genome_, length_);
  length_ = rhs.length_;
  mutation_rate_ = rhs.mutation_rate_;  
  return *this;
}

<<<<<<< HEAD:hw/Genome.cpp
Genome & operator=(Genome &&rhs) {
  delete[] genome_;
  genome_ = rhs.genome_;
  rhs.genome_ = nullptr;
  length_ = rhs.length_;
  mutation_rate_ = rhs.mutation_rate_;
}
=======






















>>>>>>> e057b0ae8e82bb7d94652b697e733dfad926c7d3:hw/src/Genome.cpp
=======
std::pair<Individual, Individual> crossover(Individual const &A,
                                            Individual const &B) {
  auto ret_val = std::make_pair(Individual{}, Individual{});
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

double Individual::evaluate(std::function<double(const char*)> f) const {
  return f(genome_);  
}

std::ostream& operator<<(std::ostream &os, Individual const &indiv) {
  double val = 0;
  memcpy(&val, &indiv.genome_, sizeof(double) );
  os << "Genome(" << indiv.length_ << " #;# "
  <<", 0x"<< std::hex ;
  for (size_t ii = 0; ii < indiv.length_; ++ii) {
    os << " " << std::setfill ('0') << std::setw (2)<<   static_cast<int>(indiv.genome_[ii]);
  }
  os << " )" << std::dec;
     
  return os;  
}
>>>>>>> 55f7f87ba25cb959725232be2c45f9baec682df1

