//=========================
// Nina Pochernyaeva
//=========================

#include <iostream>
#include <cstdlib>
#include <climits>
#include <pthread.h>
#include <iomanip>

static long RESULT = 0;
enum { DENOMINATOR = 1000000000 };

struct Arg
{
    int id;
    int part;
    double time;
    pthread_mutex_t *mutex;
};

void* thread_routine(void *arg);
long int ReadArg(char * str);

int main(int argc, char **argv, char **env) {
    //timer
    struct timespec begin, end; 
    double timer;

    clock_gettime(CLOCK_REALTIME, &begin);

    //check input argument
    if(argc != 3) {
        std::cout << "Programm requires number of threads and number to be summarized as arguments!\n";
        return EXIT_FAILURE;
    }
   
    long int number = ReadArg(argv[1]);
    long int N = ReadArg(argv[2]);

    int *id = new int[number];
    for(int i = 0; i < number; i++) {
        id[i] = i;
    }

    pthread_t *thread = new pthread_t[number];
    pthread_attr_t attr;

    //initialize and set the thread attributes
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    Arg *arg = new Arg[number];//= (ARG*) malloc(sizeof(ARG) * number); //arguments which sent to each thread

    for(long i = N - (N % number) + 1; i <= N; i++) { //sum of the rest of members
        RESULT += i;
    }

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    //creating threads
    for(int i = 0; i < number; i++) {
        arg[i].id = i; //ID of each thread
        arg[i].part = N / number; //number of members for each thread to sum
        arg[i].mutex = &mutex; //mutex which will help to avoid collisions
        int temp = pthread_create(&thread[i], &attr, thread_routine, (void*) &arg[i]);

        if(temp != 0) {
            std::cout << "Creating thread " << i << " failed!\n";
            return EXIT_FAILURE;
        }
    }

    //joining threads
    for(int i = 0; i < number; i++) {
        int temp = pthread_join(thread[i], nullptr);

        if(temp != 0) {
            std::cout << "Joining thread " << i << " failed\n";
            return EXIT_FAILURE;
        }
    }

    pthread_mutex_destroy(&mutex);

    clock_gettime(CLOCK_REALTIME, &end);

    timer = end.tv_sec - begin.tv_sec; // in seconds
    timer += (end.tv_nsec - begin.tv_nsec) / double(DENOMINATOR); // in nanoseconds

    for(int i = 0; i < number; i++) {
        std::cout << "Time spent in thread " << i << " is equal to " << std::fixed << std::setprecision(10) << arg[i].time << " seconds\n";
    }

    std::cout << "Total spent time = " << std::fixed << std::setprecision(10) << timer << " seconds\n";
    std::cout << "Sum of " << N << " numbers is equal to " << RESULT << "\n";

    delete[] id;
    delete[] thread;
    delete[] arg; 

    return 0;
}


void* thread_routine(void *argument) {
    //time measurement
    struct timespec begin = {}, end = {}; 
    double timer;

    clock_gettime(CLOCK_REALTIME, &begin);

    Arg *arg = (Arg*) argument;
    long int local_result(0);

    for(long int i = arg->id * arg->part + 1; i <= (arg->id + 1) * arg->part; i++) {
        local_result += i;
    }

    pthread_mutex_t mutex = (pthread_mutex_t) *(arg->mutex);
    pthread_mutex_lock(&mutex); //collisions avoidance

    RESULT += local_result;

    pthread_mutex_unlock(&mutex); //collisions avoidance
    clock_gettime(CLOCK_REALTIME, &end);

    timer = end.tv_sec - begin.tv_sec; //seconds
    timer += (end.tv_nsec - begin.tv_nsec) / double(DENOMINATOR); //nanoseconds
    arg->time = timer;

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
