
Author: Aaron Ogle
Project: Computing PI With Threads
Description: This program will explore the use of threads in an application that will
compute pi.  This program will utilize a Monte Carlo method in order to estimate the
value of pi.  The program will implement a timing system in order to keep track of
how long it takes for the Monte Carlo method to compute an estimate of pi based off
of different numbers of threads and different numbers of sample points.  

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

