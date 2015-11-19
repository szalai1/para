#include "Genome.h"


Genome::Genome(Genome&& g) {
  length_ = g.length_;
  g.length_ = 0;
  genome_ = g.genome_;
  g.genome_ = nullptr;
  mutation_rate_ = g.mutation_rate_;
}

Genome::Genome(size_t len,
               float mutation_rate,
               std::function<char(size_t)> f)  :length_(len),
                                          mutation_rate_(mutation_rate)
{
  genome_ = new char[length_];
  for ( size_t ii = 0; ii < length_; ++ii) {
    genome_[ii] = f(ii);
  }
}

Genome Genome::get_begin(size_t k) const {
  if (k >= length_ ) {
    return Genome{0, mutation_rate_};
  } 
  Genome begin_genome{k, mutation_rate_};
  for ( size_t ii = 0; ii < k; ++ii ) {
    begin_genome.genome_[ii] = genome_[ii];
  }
  return begin_genome;
}

Genome Genome::get_end(size_t k) const {
  if (k >= length_ ) {
    return Genome{0, mutation_rate_};
  }
  Genome begin_genome{length_ - k, mutation_rate_};
  for ( size_t ii = k; ii < length_; ++ii ) {
    begin_genome.genome_[ii-k] = genome_[ii];
  }
  return begin_genome;
}

Genome& Genome::append(const Genome &g) {
  size_t len = length_ + g.length_;
  char *tmp = new char[len];
  strcpy(tmp, genome_);
  strcpy(tmp + length_, g.genome_);
  delete[] genome_;
  length_ = len;
  genome_ = tmp;
  return *this;
}


char *Genome::serialize() const {
  char *ser = new char[sizeof(float) + sizeof(size_t) + length_];
  memcpy(ser, &length_, sizeof(size_t));
  memcpy(ser + sizeof(size_t), &mutation_rate_, sizeof(float));
  memcpy(ser + sizeof(size_t) + sizeof(float), &genome_, length_);
  return ser;
}

void Genome::mutation() {
  for ( size_t ii = 0; ii < length_ -1; ++ii ) {
    if (static_cast<double>(rand())/RAND_MAX < mutation_rate_) {
      std::swap<char>(genome_[ii], genome_[ii + 1]);
    }
  }
}

void Genome::dot_mutation(size_t where) {
  if ( where < length_ ) {
    genome_[where] = rand()%255;
  }
}

























