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
    img_[j*dimx_ + i] = pix;
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
  #ifdef OMPI_MPI_H
  void Image::mpi_conv(char *M);
  #else
  char *new_pic = new char[dimx_*dimy_];
  for ( int i = 1; i < dimx_-1; ++i) {
    for (int j = 1; j < dimy_ -1; ++j) {
      new_pic[i*dimy_ + j] += convolute_pixel(i, j, M);
    }
  }
  delete[] img_;
  img_=new_pic;
  #endif
}

Image Image::get_stripex(int from, int to) const {
  if ( from >= 0 and to < dimx_) {
    int len = to - from;
    char *new_img = new char[len * dimy_];
    char *act = new_img;
    for(int ii = 0; ii < dimy_; ++ii) {
      memcpy(act, img_ + ii*dimx_ + from, len);
      act += len;
    }
    return Image(new_img, len, dimy_);
  }
}

Image Image::get_stripey(int from, int to) const {
  if ( from >= 0 and to < dimy_) {
    int len = to - from;
    char *new_img  = new char[len * dimx_];
    memcpy(new_img, img_ + from*dimx_, len*dimx_);
    return Image(new_img, dimx_, len );
  }
}

char *Image::convolute_lineX(int line, char *M) {
  if ( line < dimx_ and line > 0) {
    char *tmp = new char[dimy_];
    for (int ii = 1; ii < dimy_; ++ii) {
      tmp[ii] = convolute_pixel(line, ii, M);
    }
    return tmp;
  }
}

char Image::convolute_pixel(int x, int y, char *M) {
  if (x > 0 and x < dimx_ and y > 0 and y < dimy_) {
    char c = 0;
    for (int k=0; k<3; k++) {
      for (int l=0; l<3; l++) {
        c += M[k*3 + l]*img_[(y-k+1)*dimx_ + x-l+1];
      }
    }
    return c;
  }
  return 0;
}

Image Image::catenateX(std::vector<Image*> imgs, int num) {
  int new_dimx = dimx_;
  for(int ii = 0; ii < num; ++ii) {
    new_dimx += imgs[ii]->get_sizex();
    if (dimy_ < imgs[ii]->get_sizey()) {
      return Image(NULL, 0 ,0);
    }
  }
  char *new_img = new char[new_dimx*dimy_];
  char *act = new_img;
  for(int ii = 0; ii < dimy_; ++ii) {
    memcpy(act, img_ + ii*dimx_, dimx_);
    act += dimx_;
    for(int jj = 0; jj < num; ++jj) {
      char *act_img = imgs[jj]->img_;
      int dimx = imgs[jj]->dimx_;
      memcpy(act, act_img + ii*dimx, dimx);
      act += dimx;
    }
  }
  return Image(new_img, new_dimx, dimy_);
}

#ifdef OMPI_MPI_H
void Image::mpi_conv(char *) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int rate = ((dimy_/size) + 1);
  int from = rank * rate;
  int to = (rank + 1)*rate -1;
  if (rank == size -1) {
    to = dimy_;
  }
  int len = to - from;
  char *new_pic = new char[len*dimy_];
  for ( int i =  1; i < dimx_-1; ++i) {
    for (int j = from + 1; j < to -1; ++j) {
      new_pic[i*len + j] += convolute_pixel(i, j, M);
    }
  }
  delete img_;
  img_=new_pic;
  
}
#endif
