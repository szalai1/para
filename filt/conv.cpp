#include <iostream>
//#include <mpi.h>
#include <vector>
#include "image.h"

char M[]= {	0, 1, 0,  1, -4, 1,  0, 1, 0};

int main(int argc, char *argv[]) {
  //  MPI_Init(&argc, &argv);
  int size , rank;
  //MPI_Comm_size(MPI_COMM_WORLD, &size);
  //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	Image img(argv[1]);
  std::cout << img.get_sizex() << " " << img.get_sizey() << std::endl;
  Image partI  = img.get_stripex(0,100);
  std::cout << "X" << std::endl;
  Image partII = img.get_stripex(100,200);
  Image partIII = img.get_stripex(200,300);
  Image partIV = img.get_stripex(300,400);
  std::vector<Image *> x(3);
  x[0] = &partII;
  x[1] = &partIII;
  x[2] = &partIV;
  Image II = partI.catenateX(x, 3);
  //img.convolute_lineX(150, M);
  //img.save("part2.pgm");
  II.save("partII.pgm");

}

