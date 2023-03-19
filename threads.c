/*
Author: Aaron Ogle
Project: Computing PI With Threads
Description: This program will explore the use of threads in an application that will
compute pi.  This program will utilize a Monte Carlo method in order to estimate the
value of pi.  The program will implement a timing system in order to keep track of
how long it takes for the Monte Carlo method to compute an estimate of pi based off
of different numbers of threads and different numbers of sample points.  Note that
the code below has been modified from code provided to us for this assignment.

Note additionally that in order to compile and run the program the following commands must be utilized:

cc osproj3a.c -o pi -lpthread
./a.out <# of sample points> <# of threads>

ex: ./pi 80000 8

meaning 80000 sample points with 8 threads

ex input/output:

aogle@DESKTOP-0EPMK2E:~/CSC660/Projects/Project3$ cc osproj3a.c -o pi -lpthread
aogle@DESKTOP-0EPMK2E:~/CSC660/Projects/Project3$ ./pi 80000 8
Computed PI = 3.148750
Elapsed time: 1 milliseconds
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define MAX_THREADS 512

// Function prototype
void *compute_pi(void *);

/* Global variables */
long sample_points;
long total_hits;
long total_misses;
long hits[MAX_THREADS];
long sample_points_per_thread;
int num_threads;

int main(int argc, char *argv[])
{
    /* local variables */
    int ii;
    int retval;
    pthread_t p_threads[MAX_THREADS];
    pthread_attr_t attr;
    double computed_pi;

    /* initialize local variables */
    retval = 0;

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    /* parse command line arguments into sample points and number of threads */
    /* there is no error checking here!!!!! */
    sample_points = atol(argv[1]);
    num_threads = atoi(argv[2]);

    /* uncomment this block if you want interactive input!!!! */
    /* if so...comment out the two statements above */
    /*
  printf( "Enter number of sample points: " );
  scanf( "%d", &sample_points );
  printf( "Enter number of threads: " );
  scanf( "%d%", &num_threads );
  */
    // Timing method based off of what was provided in timer_example.c
    // This portion starts our timer
    // timer
    struct timeval start, end;
    long mtime, seconds, useconds;
    gettimeofday(&start, NULL); // timer

    total_hits = 0;
    sample_points_per_thread = sample_points / num_threads;

    for (ii = 0; ii < num_threads; ii++)
    {
        hits[ii] = ii;
        pthread_create(&p_threads[ii], &attr, compute_pi, (void *)&hits[ii]);
    }

    for (ii = 0; ii < num_threads; ii++)
    {
        pthread_join(p_threads[ii], NULL);
        total_hits += hits[ii];
    }

    computed_pi = 4.0 * (double)total_hits / ((double)(sample_points));

    printf("Computed PI = %lf\n", computed_pi);

    // Timing method based off of what was provided in timer_example.c
    // This portion gets the "end" time
    gettimeofday(&end, NULL); // timer
    // Calculate the total elapsed time, and format it
    // timer
    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds)*1000 + useconds / 1000.0) + 0.5;
    printf("Elapsed time: %ld milliseconds\n", mtime);

    /* return to calling environment */
    return (retval);
}

/*
################################################################################################
Compute PI (Monte Carlo Method) Estimate

This function will estimate the value of pi based off of a monte carlo
method.  The function works by generating random points:

rand_no_x = (double)(rand_r(&seed)) / (double)RAND_MAX;
rand_no_y = (double)(rand_r(&seed)) / (double)RAND_MAX;

and checking to see if they are within a circle of radius 0.5, hence
the use of subtracting 0.5 from the random number that is generated; we
then find the ratio of the number of points that are inside the circle
and total number of points generated.  Using this method provides a way
to estimate the value of pi based off of the below equation:

pi ~ 4 * (# of points in circle / total # of points generated)

Return Value
-------------------------------------------------------------------------
None - void function so it does not return a value, instead it keeps track
of "local_hits" by utilizing a pointer - *hit_pointer

Pointer Parameters
-------------------------------------------------------------------------
s           void pointer         number of sample points

-------------------------------------------------------------------------
################################################################################################
*/

void *compute_pi(void *s)
{
    int seed;
    long ii;
    long *hit_pointer;
    long local_hits;
    double rand_no_x;
    double rand_no_y;

    hit_pointer = (long *)s;
    seed = *hit_pointer;
    local_hits = 0;

    for (ii = 0; ii < sample_points_per_thread; ii++)
    {
        rand_no_x = (double)(rand_r(&seed)) / (double)RAND_MAX;
        rand_no_y = (double)(rand_r(&seed)) / (double)RAND_MAX;
        if (((rand_no_x - 0.5) * (rand_no_x - 0.5) +
             (rand_no_y - 0.5) * (rand_no_y - 0.5)) <= 0.25)
            local_hits++;
        seed *= ii;
    }

    *hit_pointer = local_hits;
    pthread_exit(0);
}