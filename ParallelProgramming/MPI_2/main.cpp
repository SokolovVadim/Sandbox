//=========================
// Nina Pochernyaeva
//=========================

#include "mpi.h"
#include <climits>
#include <iostream>

long int ReadArg(char * str);
int CalculateSum(int N, int rank, int token);

int main(int argc, char* argv[])
{
	int proc_num(0), proc_rank(0);
	int N(0);
	MPI_Status status = {};

	// MPI code starts here
	int error = MPI_Init(&argc, &argv);
	if(error)
		std::cerr << "MPI_Init crashed" << std::endl;
	double start_time = MPI_Wtime();	
	MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	
		if(argc != 2){
			std::cerr << "Invalid number of arguments!\n";
			return EXIT_FAILURE;
		}
		N = ReadArg(argv[1]);
	

	int root = proc_num - 1; // last process

	int token(N / root);
	int token_sum(0);

	if(proc_rank == root)
	{
		int sum = CalculateSum(N, proc_rank, token);
		for(int i(0); i < root; ++i)
		{
			MPI_Recv(&token_sum, 1, MPI_INT, i, 10, MPI_COMM_WORLD, &status);
			sum += token_sum;
		}
		printf("sum = %d\n", sum);
	}
	else
	{
		int end = (proc_rank + 1) * token;
		for(int i(proc_rank * token + 1); i <= end; ++i)
		{
			token_sum += i;
		}
		MPI_Send(&token_sum, 1, MPI_INT, root, 10, MPI_COMM_WORLD);
	}

	double end_time = MPI_Wtime();
	double time_diff = end_time - start_time;
	if(proc_rank == root)
	{
		printf("total time spent: %lf\n", time_diff);
	}
	else
	{
		printf("proc[%d] time spent: %lf\n", proc_rank, time_diff);
	}


	MPI_Finalize();
}

int CalculateSum(int N, int rank, int token)
{
	//printf("token = %d\n", token);
	int sum(0);
	for(int i(rank * token + 1); i <= N; ++i)
		sum += i;
	// printf("rank * token + 1 = %d", rank * token + 1);
	return sum;
}

long int ReadArg(char * str)
{
    char* endptr;
    errno = 0;

    long int number = strtol(str, &endptr, 10);
    
    if ((errno == ERANGE && (number == LONG_MAX || number == LONG_MIN)) || (errno != 0 && number == 0)) 
    {
            perror("strtol");
            exit(EXIT_FAILURE);
    }

    if (endptr == str)
    {
            fprintf(stderr, "Error!\n");
            exit(EXIT_FAILURE);
    }
    if (*endptr != '\0')
    {
            fprintf(stderr, "Error!\n");
            exit(EXIT_FAILURE);
    }

    return number;
}