//=========================
// Nina Pochernyaeva
//=========================

#include "mpi.h"
#include <iostream>

int main(int argc, char* argv[])
{
	int proc_num(0), proc_rank(0);
	// MPI code starts here
	int error = MPI_Init(&argc, &argv);
	if(error)
		std::cerr << "MPI_Init crashed" << std::endl;
	MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	if(proc_rank == 0)
		printf("size = %d\n", proc_num);
	printf("rank = %d\n", proc_rank);

	MPI_Finalize();
}