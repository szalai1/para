#include "image.h"
#include <iostream>

Image::~Image() {
  delete[] img_;
}

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

void Image::convolution(char *M) {
  char *new_pic = new char[dimx_*dimy_];
  for ( int i = 1; i < dimx_-1; ++i) {
    for (int j = 1; j < dimy_ -1; ++j) {
      for (int k=0; k<3; k++) {
	      for (int l=0; l<3; l++) {
          new_pic[i*dimy_ + j] += M[k*3 + l]*img_[(i-k+1)*dimy_ + j-l+1];
        }
      }   
    }
  }
  delete[] img_;
  img_=new_pic;
}

