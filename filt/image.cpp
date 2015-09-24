#include "image.h"

Image::Image(char *file) {
  std::ifstream ifs(file, std::fstream::in|std::fstream::binary);
  ifs.ignore(256, '\n');
  int num;
  char c;
  ifs >> dimx_ >> dimy_ >> num >> c;
  size_t  size = dimx_*dimy_;
  img_ = new char[size + 1];
  ifs.read(img_, size);
  img_[size] = '\0';
  ifs.close();
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

void Image::save(const char *file_name) {
  std::fstream ofs(file_name, std::fstream::out|std::fstream::binary);
  ofs << "P5" << std::endl << dimx_ << " " << dimy_ << std::endl
      << 255 << std::endl;
  ofs.write(img_, dimx_*dimy_);
  ofs.close();
}
