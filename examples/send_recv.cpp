#include <mpi.h>
#include <iostream>

int main(int argc, char **argv) { 
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Status stat;
  char a[] = "titkos";
  char b[7];
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  for( int ii = 1; ii < size && rank == 0; ++ii) {
     MPI_Send(a, 7, MPI_CHAR, ii, 1, MPI_COMM_WORLD);
     std::cout << a << " sent " << ii <<  std::endl;
  }
  if ( rank != 0 ) {
     MPI_Recv(b, 7, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &stat);
     std::cout << rank  << " " << b << std::endl;
  }
  MPI_Finalize();
}
