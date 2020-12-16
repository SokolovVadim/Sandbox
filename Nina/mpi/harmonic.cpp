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

	long number = ReadArg(argv[1]);
	std::cout << "Number entered: " << number << std::endl;

	double sum = 0.0;
	#pragma omp parallel reduction (+: sum)
	#pragma omp for
	for(long i = 0; i < number; i++)
		sum += 1.0 / (i + 1);
	std::cout << "sum: " << sum << std::endl;
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

