#include "stdio.h"
#include "mpi.h"


int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int buf[7] = {10, 20, 30, 40, 50, 60, 70};
	int recv_buf[3] = {0, 0, 0};
	MPI_Scatter(buf, 2, MPI_INT, recv_buf, 3, MPI_INT, 0, MPI_COMM_WORLD);
	printf("rank: %d %d %d %d\n", rank, recv_buf[0], recv_buf[1], recv_buf[2]);
	return 0;
}
