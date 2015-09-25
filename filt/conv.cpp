#include <iostream>
#include <mpi.h>
#include "image.h"

char M[]= {	5, 2, 0,  1,-1, 2,  -1, 1 ,1  };



int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int size , rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::cout << size << " " << rank << std::endl;
	Image img(argv[1]);
  img.convolution(M);
  img.save("proba.pgm");
}

