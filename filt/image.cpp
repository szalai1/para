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
  char *new_pic = new char[dimx_*dimy_];
  for ( int i = 1; i < dimx_-1; ++i) {
    for (int j = 1; j < dimy_ -1; ++j) {
      new_pic[i*dimy_ + j] += convolute_pixel(i, j, M);
    }
  }
  delete[] img_;
  img_=new_pic;
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

void Image::mpi_conv(char *M) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::cout << "###" << rank << " x: " << dimx_ << " y: " << dimy_ << std::endl;
  MPI_Bcast(&dimy_, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&dimx_, 1, MPI_INT, 0, MPI_COMM_WORLD);
  std::cout << size << " " << rank << " x: " << dimx_ << " y: " << dimy_ << std::endl;
  int rate = ((dimy_/size));
  int from = rank * rate ;
  int to = (rank + 1)*rate -1 ;
  if (rank == size -1) {
    to = dimy_;
  }
  std::cout << size << " " << rank << " f: " << from <<  " to: " << to << std::endl;
  char *tmp_img = new char[(to-from)*dimx_];
  MPI_Scatter(img_,
              (to-from)*dimx_,
              MPI_CHAR,
              tmp_img,
              (to-from)*dimx_,
              MPI_CHAR,
              0, MPI_COMM_WORLD);
  std::cout << rank << ": scatter done"  << std::endl;
  Image tmp(tmp_img, dimx_, to-from);
  char file_name[] = "pic_x.pgm";
  file_name[4] = '0' + rank;
  tmp.convolution(M);
  std::cout << rank << ": conv done \n";
  char *new_pic = NULL;
  if (rank == 0) {
    new_pic = new char[dimx_*dimy_];
  }
  MPI_Gather(tmp_img, 150*dimx_, //(to-from)*dimx_,
             MPI_CHAR,
             new_pic, (to-from)*dimx_,
             MPI_CHAR, 0,
             MPI_COMM_WORLD);
  std::cout << rank  << ": gather\n";
  if (rank == 0) {
    Image new_image(new_pic, dimx_, dimy_);
    new_image.save("yyy.pgm");
    for (int ii = 0; ii < size -1; ++ii) {
      for (int jj = 0; jj < dimx_; ++jj) {
        int x = ((dimy_/size) + 1)*ii;
        new_image.set(x, jj, convolute_pixel(x, jj, M) );
      }
    }
    img_ = new_pic;;
    new_image.img_ = NULL;
    }
}

