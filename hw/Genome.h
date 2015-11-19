#include<functional>

class Genome {
 public:
  Genome() = delete;
  Genome(size_t, float );
  Genome(Genome&&);
  double evaluate(std::function<double(char*,size_t)>) const;
  Genome get_begin(size_t) const;
  Genome get_end(szie_t) const;
  int size() const {
    return length_;
  }
  char *serialize() const;
  Genome append(Genome&);
  char const *get_data();
  void mutation();
  ~Genome() {
    delete[] genome_;
  }
  Genome operator=(const Genome &);
  Genome& operator=(Genome &&);
 private:
  size_t length_;
  char *genome_;
  float mutation_rate_;
};
