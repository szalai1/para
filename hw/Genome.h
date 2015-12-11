

class Genome {
 public:
  Genome() = delete;
  Genome(size_t lg, float m);
  Genome(Genome&&);
  Genome &operator=(Genome &);
  Genome &operator=(Genome &&);
  Genome get_begin(size_t) const;
  Genome get_end(size_t) const;
  int size() const {
    return length_;
  }
  float mutation_rate() {
    return mutation_rate_;
  }
  char *serialize() const;
  Genome append(Genome&);
  void deserialize(const char *);
  char const *get_data();
  void mutation();
  void dot_mutatuin(size_t);
  char *serilize() const;
  ~Genome() {
    delete[] genome_;
  }
 private:
  size_t length_;
  char *genome_;
  float mutation_rate_;
};
