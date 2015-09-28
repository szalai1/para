#include <fstream>
#include <cstring>
#include <vector>
#include <mpi.h>

class Image {
 public:
  Image(char *file_name);
  Image(char *img, int dimx, int dimy): img_(img), dimx_(dimx), dimy_(dimy) {}
  ~Image();
  int get_sizex() const {return dimx_;}
  int get_sizey() const {return dimy_;}
  Image get_stripex(int from, int to) const;
  Image get_stripey(int from, int to) const;
  char get(int i, int j);
  void set(int i, int j, char pix);
  void save(const char *file_name);
  void convolution(char *M);
  char *convolute_lineX(int, char *);
  char convolute_pixel(int, int, char *);
  Image catenateX(std::vector<Image*>, int);
  Image catenateY(std::vector<Image*>);
  void mpi_conv(char *);
 protected:
  int dimx_, dimy_;
  char *img_;
 private:

};
