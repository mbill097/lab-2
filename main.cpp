#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
// size of array
#define MAX 30
// maximum number of threads
#define MAX_THREAD 4
static const long Array_size = 3000;
static const long Num_To_Add = 1000000000;
static const double Scale = 10.0 / RAND_MAX;

//Method to calulate sum of a randomly populated array sequentially
long add_serial(const char *numbers) {
int sum=0;
    for (long i = 0; i < Array_size ; i++) {
            sum += numbers[i];
        }
    return sum;
}

//Method to calulate sum of a randomly populated array parallel
long int add_parallel(const char *numbers) {
    long int inntersum=0,sum = 0;
#pragma omp parallel num_threads(3)
    {
        int start, Psize= 3;
        int singleThread ,end;
        singleThread= Array_size/Psize;


#pragma omp for
        for (long start = 0; start < Array_size ; start++) {
            for(long end=0;end<singleThread;end++){
                inntersum += numbers[start];
                start=start+1;
            }
            sum+=inntersum;
            inntersum=0;
        }
    }
    return sum;
}
//Main metho
int main() {
     struct timeval start, end;

    char *numbers = (char*) malloc(sizeof(long) * Num_To_Add);
    long chunk_size = Num_To_Add / omp_get_max_threads();
#pragma omp parallel num_threads(omp_get_max_threads())
    {
        int p = omp_get_thread_num();
        unsigned int seed = (unsigned int) time(NULL) + (unsigned int) p;
        long chunk_start = p * chunk_size;
        long chunk_end = chunk_start + chunk_size;
        for (long i = chunk_start; i < chunk_end; i++) {
            numbers[i] = (char) (rand_r(&seed) * Scale);
        }
    }


    printf("Timing sequential...\n");
    gettimeofday(&start, NULL);
    long sum_s = add_serial(numbers);
    gettimeofday(&end, NULL);
    printf("Took  %.6f seconds\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    printf("Timing parallel...\n");
    gettimeofday(&start, NULL);
    long sum_p = add_parallel(numbers);
    gettimeofday(&end, NULL);
    printf("Took %.6f seconds\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    printf("Sum serial: %ld\nSum parallel: %ld", sum_s, sum_p);

    return 0;
}
