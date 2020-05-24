//=========================
// Nina Pochernyaeva
//=========================

#include <iostream>
#include <cstdlib>
#include <climits>
#include <pthread.h>
#include <iomanip>
#include <cmath>

const int NUMBER = 4; //number of threads

static double RESULT = 0; //result of integration

//integrate from A to B with aertain GRid
static double A = 0;
static double B = 0;
static long   GRid = 0;

struct Arg
{
    int id;
    int time;
    pthread_mutex_t *mutex;
};

void* thread_routine(void *Arg); //function for each thread
double function(const double x); //function that will be integrated

int main(int Argc, char **Argv, char **env) {
    //check input Argument
    if(Argc != 1) {
        std::cout << "Invalid input: no Argument is needed!\n";
        return EXIT_FAILURE;
    }

    std::cout << "This program allows to calculate integral from A to B\n";
    std::cout << "Enter A = ";
    std::cin >> A;
    std::cout << "Enter B = ";
    std::cin >> B;
    std::cout << "Enter number of nodes (GRid > 1) = ";
    std::cin >> GRid;

    if((A >= B) || (GRid <= 1)) {
        std::cout << "Invalid numbers (input)\n";
        return EXIT_FAILURE;
    }

    //timer
    struct timespec BEGIN, END; //-lrt key is needed -> time_t tv_sec and long tv_nsec
    double TIMER;

    clock_gettime(CLOCK_REALTIME, &BEGIN);

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
    pthread_mutex_init(&mutex, NULL);

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
        int temp = pthread_join(thread[i], NULL);

        if(temp != 0) {
            std::cout << "Joining thread " << i << " failed\n";
            return EXIT_FAILURE;
        }
    }

    pthread_mutex_destroy(&mutex);

    clock_gettime(CLOCK_REALTIME, &END);

    TIMER = END.tv_sec - BEGIN.tv_sec; //seconds
    TIMER += (END.tv_nsec - BEGIN.tv_nsec) / 1000000000.0; //nanoseconds

  
    for(int i = 0; i < NUMBER; i++) {
        std::cout << "Time spent in thread #" << i << " equals to " << std::fixed << std::setprecision(10) << arg[i].time << " seconds\n";
    }
  

    std::cout << "Total spent time = " << std::fixed << std::setprecision(10) << TIMER << " seconds\n";

   
    std::cout << "Integral equals to " << RESULT << "\n";
   
    delete[] id;
    delete[] thread;
    delete[] arg;

    return 0;
}

void* thread_routine(void *Argument) {
    //timer
    struct timespec BEGIN, END; //-lrt key is needed -> time_t tv_sec and long tv_nsec
    double TIMER;

    clock_gettime(CLOCK_REALTIME, &BEGIN);

    Arg *arg = (Arg*) Argument  ;

    double LOCAL_RESULT = 0;
    long LOCAL_RANGE = (GRid - 1) / NUMBER;

    double h = (B - A) / ((double) (GRid - 1)); //step of integration

    for(long i = 0; i < LOCAL_RANGE; i++) {
        LOCAL_RESULT += (function(A + (LOCAL_RANGE * (arg->id) + i) * h) +
                         function(A + (LOCAL_RANGE * (arg->id) + (i + 1)) * h)) * 0.5 * h;
    }

    if((arg->id) == 0) {
        for(long i = 0; i + NUMBER * LOCAL_RANGE + 1 < GRid; i++) {
            LOCAL_RESULT += (function(B - i * h) +
                             function(B - (i + 1) * h)) * 0.5 * h;
        }
    }

    pthread_mutex_t mutex = (pthread_mutex_t) *(arg->mutex);

    pthread_mutex_lock(&mutex); //collisions avoidance

    RESULT += LOCAL_RESULT;

    pthread_mutex_unlock(&mutex); //collisions avoidance

    clock_gettime(CLOCK_REALTIME, &END);

    TIMER = END.tv_sec - BEGIN.tv_sec; //seconds
    TIMER += (END.tv_nsec - BEGIN.tv_nsec) / 1000000000.0; //nanoseconds

    arg->time = TIMER;

    pthread_exit(NULL);
}

double function(const double x) {
    return x * exp(x);
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
