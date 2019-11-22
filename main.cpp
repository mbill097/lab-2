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
//Method to calulate sum of a randomly populated array sequentially
long add_serial(const int *numbers) {
int sum=0;
    for (long i = 0; i < Array_size ; i++) {
            sum += numbers[i];
        }
    return sum;
}

//Method to calulate sum of a randomly populated array parallel
long int add_parallel(const int *numbers) {
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
    // Code to generate Ramdom number and store into array.
    int numbers[Array_size], i;
    for(i = 0; i <  Array_size; i++)
    {
        numbers[i] = (rand() % 100)+1;
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
