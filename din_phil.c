#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "mt.h"

#define THINKMIN 1
#define THINKMAX 20
#define EATMIN 2
#define EATMAX 9

pthread_t philosopher[5];
pthread_mutex_t eating_fork[5];

int forks[5] = {0,0,0,0,0};

int get_rand(int min, int max){
	return genrand_int32 () % (max-min) + min;
}

/* 
 * Prints the available forks. 0 for unused, and the philosopher's number for the used.
 */
void print_forks(int tab){
	int i = 0;
	if(tab)
		printf("\t\t");
	printf("Forks in use: {");
	for(i=0; i<5; ++i){
		printf(" %d ", forks[i]);
	}
	printf("}\n");
}

/*
 * Each philosopher is in an endless loop of eating and thinking.
 * They need two forks, which are always the same forks. 
 * Because we grab the forks simultaneously, they cannot deadlock.
 * @param n: philosopher number
 */
void *phil(int n){
	while(1){
		// Think
		int thinking_time = get_rand(THINKMIN,THINKMAX);
		printf("Philosopher %d is thinking for %d seconds.\n", n+1, thinking_time);
		sleep(thinking_time);

		// Get forks
		pthread_mutex_lock(&eating_fork[n]);
		pthread_mutex_lock(&eating_fork[(n+1)%5]);
		forks[n] = n+1;
		forks[(n+1)%5] = n+1;

		// Eat
		int eating_time = get_rand(EATMIN,EATMAX);
		printf("\t\tPhilosopher %d is eating for %d seconds.\n", n+1, eating_time);
		print_forks(1);
		sleep(eating_time);
		
		// Return Forks
		forks[n] = 0;
		forks[(n+1)%5] = 0;
		pthread_mutex_unlock(&eating_fork[n]);
		pthread_mutex_unlock(&eating_fork[(n+1)%5]);
	}
}

int main(){
	int i;
	for(i=0; i<5; ++i){
		pthread_mutex_init(&eating_fork[i], NULL);
	}
	for(i=0; i<5; ++i){
		pthread_create(&philosopher[i], NULL, (void *) phil, (void*)i);
	}

	//this should never happen...
	for(i=0; i<5; ++i){
		pthread_join(philosopher[i], NULL);
	}
	for(i=0; i<5; ++i){
		pthread_mutex_destroy(&eating_fork[i]);
	}

	return 0;
}
