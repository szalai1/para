#include "Genome.h"

using Genome;

Genome(Genome&& g) {
  length_ = g.length_;
  g.length_ = 0;
  genome_ = g.genome_;
  g.genome_ = nullptr;
  mutation_rate_ = g.mutation_rate_;
}

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
    return Genome{0, mutation_rate_};
  }
  
  Genome begin_genome{k, mutation_rate_};
  for ( int ii = 0; ii < k; ++ii ) {
    begin_genome.genome_[ii] = genome_[ii];
  }
  return begin_genome;
}

Genome get_end(size_t k) const {
  if (k >= length ) {
    return Genome{0, mutation_rate_};
  }
  Genome begin_genome{length_ - k, mutation_rate_};
  for ( int ii = k; ii < length; ++ii ) {
    begin_genome.genome_[ii-k] = genome_[ii];
  }
  return begin_genome;
}


  size_t len = length_ + g.length_;
  char *tmp = new char[len];
  memcpy(tmp, genome_, length_);
  memcpy(tmp + length_, g.genome_, g.length_);
  delete[] genome_;
  length_ = len;
  genome_ = tmp;
  return *this;
}

char *serialize() const {
  char *ser = new char[sizeof(float) + sizeof(size_t) + length_];
  memcpy(ser, &length_, sizeof(size_t));
  memcpy(ser + sizeof(size_t), &mutation_rate_, sizeof(float));
  memcpy(ser + sizeof(size_t) + sizeof(float), &genome_, length_);
  return ser;
}

void deserialize(const char* ser) {
  memcpy(ser, lenght_, sizeof(size_t));
  memcpy(ser + sizeof(size_t), mutation_rate_, sizeof(float));
  genome_ = new char[length_];
  memcpy(ser + sizeof(size_t) + sizeof(float), genome_, length_);
}

void mutation() {
  for ( int ii = 0; ii < length_ -1; ++ii ) {
    if (std::dynamic_cast<double>(rand())/RAND_MAX < mutation_rate_) {
      std::swap<char>(genome_[ii], genome_[ii + 1]);
    }
  }
}

void dot_mutation(size_t where) {
  if ( where < length_ ) {
    genome_[where] = rand()%255;
  }
}

Genome & operator=(Genome &rhs){
  delete[] genome_;
  geneom_ = new char[rhs.length_];
  memcpy(genome_, rhs.genome_, length_);
  length_ = rhs.length_;
  mutation_rate_ = rhs.mutation_rate_;  
  return *this;
}

Genome & operator=(Genome &&rhs) {
  delete[] genome_;
  genome_ = rhs.genome_;
  rhs.genome_ = nullptr;
  length_ = rhs.length_;
  mutation_rate_ = rhs.mutation_rate_;
}

