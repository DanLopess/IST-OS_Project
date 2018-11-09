#include "lock.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t grid_lock;
pthread_mutex_t queue_lock;
pthread_mutex_t insert_lock;

/*
*   lock_init - function that tries to initiate all the mutexes
*		if failed exits program
*/
void lock_init() {
	if(pthread_mutex_init(&grid_lock, NULL)!=0){
		fprintf(stderr, "Failed to initiate mutex.\n");
		exit(1);
	}
	if(pthread_mutex_init(&queue_lock, NULL)!=0){
		fprintf(stderr, "Failed to initiate mutex.\n");
		exit(1);
	}
	if(pthread_mutex_init(&insert_lock, NULL)!=0){
		fprintf(stderr, "Failed to initiate mutex.\n");
		exit(1);
	}
}

/*
*   lock_global - function that tries to lock a given mutex
*		if failed exits program
*/
void lock_global (pthread_mutex_t lock) {
	if (pthread_mutex_lock(&lock)!=0) {
		fprintf(stderr, "Failed to lock.\n");
		exit(1);
	}
}


/*
*   lock_init - function that tries to initiate all the mutexes
*		if failed exits program
*/
void unlock_global (pthread_mutex_t lock) {
	if (pthread_mutex_unlock(&lock)!=0) {
		fprintf(stderr, "Failed to unlock.\n");
		exit(1);
	}
}
