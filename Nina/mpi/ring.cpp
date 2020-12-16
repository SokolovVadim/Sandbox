#include <omp.h>
#include <iostream>
#include <climits>

long int ReadArg(char * str);

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cerr << "Invalid input!\n Enter a number";
		return EXIT_FAILURE;
	}

	long thread_num = ReadArg(argv[1]);
	std::cout << "Number entered: " << thread_num << std::endl;
	int shared(0);
	omp_set_num_threads(thread_num);
	#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		printf("entering critical section ...\nshared variable: %d\n", shared);
		#pragma omp critical
		{
			shared += tid; // change the variable in each thread
			printf("tid: %d, shared variable: %d\n", tid, shared);
		}
		printf("Exiting critical section ...\nshared variable: %d\n", shared);
	}
	printf("shared variable final value: %d\n", shared);

	return 0;
}

// strtol routine is safe in comparison to atoi

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