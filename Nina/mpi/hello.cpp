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
	omp_set_num_threads(thread_num);
	#pragma omp parallel
	{
		printf("Hello world! Number of threads: %ld, my thread id is: %d\n", thread_num, omp_get_thread_num());
	}

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
