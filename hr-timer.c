#include <stdio.h>
#include <stdint.h>
#include <time.h>

//this function calcuates the time difference between the two parameters in nanoseconds
unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}


int main()
{
struct timespec start; //variable used to record the starting time of the clock
struct timespec stop; //variable used to record the stop time of the clock
unsigned long long result; //64 bit integer used to record the resulting time calculated by the clock 

////////////////Clock 1//////////////////
// CLOCK_REALTIME is a system-wide realtime clock and represents the machine's best guess as to the current wall-clock. This clock can jump forwards and backwards if the system time-of-day clock is changed. It should be used if one wants to write a program which is supposed to work with the real-world time.
clock_gettime(CLOCK_REALTIME, &start); //record the starting time for clock 1
sleep(1); //delay for specified amount of time
clock_gettime(CLOCK_REALTIME, &stop); //record the stop time for clock 1

result=timespecDiff(&stop,&start); //calculate the time spend using clock 1 (in nanosec)

printf("CLOCK_REALTIME Measured: %llu\n",result); //print out the result

////////////////Clock 2//////////////////
//CLOCK_MONOTONIC represents the total elapsed time since some unspecified starting point and isn't affected by changes in the system time-of-day clock. It should be used if one wants to know the relative time and to include the time spent waiting for I/O and the slowdowns caused by other processes getting scheduled. 
clock_gettime(CLOCK_MONOTONIC, &start); //record the starting time for clock 2
sleep(1); //delay for specified amount of time
clock_gettime(CLOCK_MONOTONIC, &stop); //record the stop time for clock 2

result=timespecDiff(&stop,&start); //calculate the time spend using clock 2 (in nanosec)

printf("CLOCK_MONOTONIC Measured: %llu\n",result); //print out the result

////////////////Clock 3//////////////////
//CLOCK_PROCESS_CPUTIME_ID is a high-resolution per-process timer from the CPU and is used for measuring the amount of CPU time consumed by the process. It will only count the actual clock cycles spent executing on your process's behalf, but not the time spent sleeping. It should be used if one wants to know how much CPU time has passed since the process started.
clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start); //record the starting time for clock 3
sleep(1); //delay for specified amount of time
clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop); //record the stop time for clock 3

result=timespecDiff(&stop,&start); //calculate the time spend using clock 3 (in nanosec)

printf("CLOCK_PROCESS_CPUTIME_ID Measured: %llu\n",result); //print out the result

////////////////Clock 4//////////////////
//CLOCK_THREAD_CPUTIME_ID is a thread-specific CPU-time clock and is used for measuring the amount of CPU time consumed by the thread. 
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start); //record the starting time for clock 4
sleep(1); //delay for specified amount of time
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop); //record the stop time for clock 3

result=timespecDiff(&stop,&start); //calculate the time spend using clock 4 (in nanosec)

printf("CLOCK_THREAD_CPUTIME_ID Measured: %llu\n",result); //print out the result


}
