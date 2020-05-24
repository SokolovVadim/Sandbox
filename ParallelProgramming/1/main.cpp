//=========================
// Nina Pochernyaeva
//=========================

#include <iostream>
#include <cstdlib>
#include <climits>
#include <pthread.h>

void* thread_routine(void* arg);
long int ReadArg(char * str);

int main(int argc, char** argv, char** env) {
    if(argc != 2) {
        std::cout << "Programm requires number of threads as an argument!\n";
        return EXIT_FAILURE;
    }

    int N = ReadArg(argv[1]);
    int* arg = new int[N];

    for(int i = 0; i < N; i++) {
        arg[i] = i;
    }

    pthread_t* thread = new pthread_t[N];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    for(int i = 0; i < N; i++) {
        int temp = pthread_create(&thread[i], &attr, thread_routine, (void*) &arg[i]);

        if(temp != 0) {
            std::cout << "Creating thread " << i << " failed!\n";
            return EXIT_FAILURE;
        }
    }

    for(int i = 0; i < N; i++) {
        int temp = pthread_join(thread[i], nullptr);
        if(temp != 0) {
            std::cout << "Joining thread " << i << " failed\n";
            return EXIT_FAILURE;
        }
    }

    delete[] arg;
    delete[] thread;

    return 0;
}

void* thread_routine(void* arg) 
{
    int ID = *((int*)arg);

    printf("Thread %d\n", ID);
    fflush(stdout);

    pthread_exit(nullptr);
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