#include <iostream>
//#include <mpi.h>
#include "image.h"

char M[]= {	0, 1, 0,  1, -4, 1,  0, 1, 0};



int main(int argc, char *argv[]) {
  //  MPI_Init(&argc, &argv);
  int size , rank;
  //MPI_Comm_size(MPI_COMM_WORLD, &size);
  //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	Image img(argv[1]);
  std::cout << img.get_sizex() << " " << img.get_sizey() << std::endl;
  //  Image partI  = img.get_stripex(100,400);
  img.convolute_lineX(150, M);
  img.save("part2.pgm");
}

