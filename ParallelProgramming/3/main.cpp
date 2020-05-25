//=========================
// Nina Pochernyaeva
//=========================

#include <iostream>
#include <cstdlib>
#include <climits>
#include <pthread.h>
#include <iomanip>
#include <cmath>

enum COMPONENTS
{
    NUMBER = 4, // thread num
    DENOMINATOR = 1000000000 // sec -> nanosec
}; 

static double RESULT = 0; //result of integration

//integrate from A to B with aertain Grid
static double A = 0;
static double B = 0;
static long   Grid = 0;

struct Arg
{
    int id;
    double time;
    pthread_mutex_t *mutex;
};

void*  thread_routine(void *Arg);
double function(const double x); //function to be integrated

int main(int Argc, char **Argv, char **env) {
    // Input check

    if(Argc != 1) {
        std::cout << "Invalid input: no Argument is needed!\n";
        return EXIT_FAILURE;
    }

    std::cout << "This program allows to calculate integral from A to B\n";
    std::cout << "Enter A = ";
    std::cin >> A;
    std::cout << "Enter B = ";
    std::cin >> B;
    std::cout << "Enter NUMBER of nodes (Grid > 1) = ";
    std::cin >> Grid;

    if((A >= B) || (Grid <= 1)) {
        std::cout << "Invalid NUMBERs (input)\n";
        return EXIT_FAILURE;
    }

    //timer
    struct timespec begin, end; 
    double timer;

    clock_gettime(CLOCK_REALTIME, &begin);

    //id = [0, 1, 2, ..., NUMBER - 1]
    int *id = new int[NUMBER];
    for(int i = 0; i < NUMBER; i++) {
        id[i] = i;
    }

    pthread_t *thread = new pthread_t[NUMBER];
    pthread_attr_t attr;

    //initialize and set the thread attributes
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    Arg *arg = new Arg[NUMBER]; //arg which sent to each thread

    //mutex to avoid collisions
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    //creating threads
    for(int i = 0; i < NUMBER; i++) {
        arg[i].id = i; //id of each thread
        arg[i].mutex = &mutex; //mutex which will help to avoid collisions
        int temp = pthread_create(&thread[i], &attr, thread_routine, (void*) &arg[i]);

        if(temp != 0) {
            std::cout << "Creating thread " << i << " failed!\n";
            return EXIT_FAILURE;
        }
    }

    //joining threads
    for(int i = 0; i < NUMBER; i++) {
        int temp = pthread_join(thread[i], nullptr);

        if(temp != 0) {
            std::cout << "Joining thread " << i << " failed\n";
            return EXIT_FAILURE;
        }
    }

    pthread_mutex_destroy(&mutex);
    clock_gettime(CLOCK_REALTIME, &end);

    timer = end.tv_sec - begin.tv_sec; //seconds
    timer += (end.tv_nsec - begin.tv_nsec) / double(DENOMINATOR); //nanoseconds

    for(int i = 0; i < NUMBER; i++) {
        std::cout << "Time spent in thread " << i << " equals to " << std::fixed << std::setprecision(10) << arg[i].time << " seconds\n";
    }
  
    std::cout << "Total spent time = " << std::fixed << std::setprecision(10) << timer << " seconds\n";
    std::cout << "Integral equals to " << RESULT << "\n";
   
    delete[] id;
    delete[] thread;
    delete[] arg;

    return 0;
}

// function to be integrated
double function(const double x) {
    return x * exp(-x);
}

void* thread_routine(void *Argument) {
    //timer
    struct timespec begin = {}, end = {}; 
    double timer(0.0);

    clock_gettime(CLOCK_REALTIME, &begin);

    Arg *arg = (Arg*) Argument  ;

    double local_result(0.0);
    long local_range = (Grid - 1) / NUMBER;

    double h = (B - A) / ((double) (Grid - 1)); //step of integration

    for(long i = 0; i < local_range; i++) {
        local_result += (function(A + (local_range * (arg->id) + i) * h) +
                         function(A + (local_range * (arg->id) + (i + 1)) * h)) * 0.5 * h;
    }

    if((arg->id) == 0) {
        for(long i = 0; i + NUMBER * local_range + 1 < Grid; i++) {
            local_result += (function(B - i * h) +
                             function(B - (i + 1) * h)) * 0.5 * h;
        }
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
