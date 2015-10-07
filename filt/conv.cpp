#include <iostream>
#include <vector>
#include "image.h"
<<<<<<< HEAD
#include  <mpi.h>

=======
#include <mpi.h>
>>>>>>> 8b772a8431f37d21e2de4e4e2ea6ce07ac414ce2

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
<<<<<<< HEAD
  img.save("mpi_conv.pgm");
=======
  if (rank == 0) {
    img.save("mpi_conv.pgm");
    }
>>>>>>> 8b772a8431f37d21e2de4e4e2ea6ce07ac414ce2
  MPI_Finalize();
}

