#include <fstream>

class Image {
 public:
  Image(char *file_name);
  Image(char *pic, int dimx, int dimy);
  char get(int i, int j);
  void set(int i, int j, char pix);
  void save(const char *file_name);
 private:
  int dimx_, dimy_;
  char *img_;
};
