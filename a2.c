#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sched.h>

//this function calcuates the time difference between the two parameters in nanoseconds
unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}


//Predefined global variables
int number = 10; //number of experiments to be conducted
struct timespec start;//variable to store the starting time
struct timespec stop;//variable to store the stop time
int num = 0; //shared resource for threads
pthread_mutex_t lock;
pthread_mutex_t begin;
pthread_cond_t condition;


void sample_func(){}; //empty sample function used to calculate the time in function_call()



//This function measures the cost of a minimal function call in C
unsigned long long function_call(){

	int i; //incrementor variable
	unsigned long long average = 0; //variable used to store the average time
	unsigned long long result = 0; //variable used to store the cumulative sum of every experiment's time
	for(i=0; i<number; i++){
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start); //record the starting time
		sample_func(); //function call
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop); //record the stop time

		result += timespecDiff(&stop,&start); //calculate the time spend (in nanosec)

	}
	average = result/number; //the average time
	printf("Average minimal function call after %d trials time is: %llu \n", number, average);
	return average;
}


//This function measures the cost of a minimal system call in C
unsigned long long system_call(){

	int i; //incrementor variable
	unsigned long long average = 0; //variable used to store the average time
	unsigned long long result = 0; //variable used to store the cumulative sum of every experiment's time

	for(i=0; i<number; i++){
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start); //record the starting time
		getpid(); //system call
        	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop); //record the stop time
        
        	result += timespecDiff(&stop,&start); //calculate the time spend (in nanosec)
	}
	average = result/number; //the average time
	printf("Average minimal system call time after %d trials is: %llu \n", number, average);
	return average;
}


//This function measures the cost of a process switching
unsigned long long process_switch(){

	int i; //incrementor variable
	char a = 'a';//the message to be sent over the pipe
	pid_t Cpid; //child pid
	unsigned long long average = 0; //variable used to store the average time
	unsigned long long result = 0; //variable used to store the cumulative time

	//variables to set thread affinity to one processor
	cpu_set_t  mask;
	CPU_ZERO( &mask );
	unsigned int lenght = sizeof(mask);
	CPU_SET( 5, &mask );

  for(i=0; i<number; i++){
	int Pfd[2]; //pipe for parent to child communication
	pipe(Pfd); //initialize the pipe

	//fork a child process:
	if((Cpid = fork()) == -1){
		perror("fork");
		exit(1);
	}
	
	//set the code to only run on a single CPU
	if ((sched_setaffinity(0, lenght, &mask)) < 1){
		//child processs:
		if(Cpid == 0){
            close(Pfd[1]); //child closes write
			read(Pfd[0], &a, sizeof(a)); // Child reads the message
			exit(0);
		//parent process:
        	}else{
			close(Pfd[0]); //parent closes read
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);//start measuring time
			write(Pfd[1], &a, sizeof(a));
			wait(NULL); //wait for child t exit
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);//stop measuring time
			close(Pfd[1]);	//close the pipe 
		}
		//sum all trials' times		
		result += timespecDiff(&stop,&start);
	}

  }

	average = result/number; //the average time
	printf("Context switch time is: %llu \n", average);
	return average;
}


//attempt to measure thread switching
void* foo(void *arg)
{
    pthread_mutex_lock(&begin);
    pthread_mutex_unlock(&begin);

	pthread_mutex_lock(&lock);

	if(num>0){
		pthread_cond_signal(&condition);
	}
	
	for (;;) {
        num++;
        pthread_cond_wait(&condition, &lock);
        pthread_cond_signal(&condition);
	}

	pthread_mutex_unlock(&lock);
}

unsigned long long thread_switch(){

	pthread_t threads[2];

	//variables to set thread affinity to one processor
	cpu_set_t  mask;
	CPU_ZERO( &mask );
	unsigned int lenght = sizeof(mask);
	CPU_SET( 5, &mask );

	unsigned long long average = 0; //variable used to store the average time
	unsigned long long result = 0; //variable used to store the cumulative time

	//initialize mutex and condition variable
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&begin, NULL);
	pthread_cond_init (&condition, NULL);
  
	pthread_mutex_lock(&begin);
	num = 0;
if ((sched_setaffinity(0, lenght, &mask)) < 1){
	pthread_create(&threads[0], NULL, foo, NULL);
	pthread_create(&threads[1], NULL, foo, NULL);

	pthread_detach(threads[0]);
	pthread_detach(threads[1]);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);//start measuring time

	pthread_mutex_unlock(&begin);

	sleep(1);

	pthread_mutex_lock(&lock);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);//stop measuring time
}
	result = timespecDiff(&stop,&start);
	average = result/number; //the average time
	printf("Thread switch time is: %llu \n", average);
	return average;
}

//The main function with the calls to the cost measuring functions above:
int main(void){
	int i;
    unsigned long long func, sys, pr_switch;
    func = function_call();
    sys = system_call();
    pr_switch = process_switch();
	i = thread_switch();
    return 0;
}
