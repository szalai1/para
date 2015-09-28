#include <mpi.h>
#include <iostream>


int main(int argc, char **argv) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  char a[] = "1234567890AB";
  char b[] = "............";
  MPI_Scatter(a, 3, MPI_CHAR, b+rank*3, 3, MPI_CHAR, 0, MPI_COMM_WORLD);
  std::cout << "n:" << rank << " N:" << size << " b = #" << b  << "#"<< std::endl;
  for( int i = 0; i < 12; ++i) {
    a[i]='.';
  }
  MPI_Gather(b+rank*3, 3, MPI_CHAR, a, 3, MPI_CHAR, 0, MPI_COMM_WORLD);
    std::cout << "    n:" << rank  << " a = #" << a  << "#"<< std::endl;
  MPI_Finalize();
}
