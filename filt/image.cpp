#include "image.h"

Image::Image(char *file) {
  std::ifstream ifs(file, std::fstream::in|std::fstream::binary);
  ifs.ignore(256, '\n');
  ifs >> dimx_ >> dimy_;
  size_t  size = dimx_*dimy_;
  img_ = new char[size];
  ifs.read(img_, size);
}

char Image::get(int i, int j) {
  if (i < dimx_ and j < dimy_) {
    return img_[i*dimx_ + j];
  }
  return 0;
}

void Image::set(int i, int j, char pix) {
  if ( i < dimx_ and j < dimy_) {
    img_[i*dimx_ + j] = pix;
  }
}
