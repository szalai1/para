#include <iostream>
#include <vector>
#include "image.h"
#include  <mpi.h>


char M[]= {	0, 1, 0,  1, -4, 1,  0, 1, 0};

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Image img(NULL, 0, 0);
  if (rank == 0) {
    img = Image(argv[1]);
  }
  img.mpi_conv(M);
  img.save("mpi_conv.pgm");
  MPI_Finalize();
}

