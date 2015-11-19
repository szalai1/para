#include <functional>
#include <cstring>

class Genome {
 public:
  Genome() = delete;
  Genome(size_t, float,
         std::function<char(size_t)> f = [](size_t s)->char
           {return rand() % 255; } );
  Genome(const Genome &);
  Genome(Genome&&);
  double evaluate(std::function<double(char*,size_t)>) const;
  Genome get_begin(size_t) const;
  Genome get_end(size_t) const;
  int size() const {
    return length_;
  }
  char *serialize() const;
  Genome& append(const Genome&);
  char const *get_data();
  void mutation();
  ~Genome() {
    delete[] genome_;
  }
  Genome operator=(const Genome &);
  Genome& operator=(Genome &&);
  void dot_mutation(size_t);
 private:
  size_t length_;
  char *genome_;
  float mutation_rate_;
};
