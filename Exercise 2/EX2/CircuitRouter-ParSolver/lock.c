#include "lock.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "grid.h"


pthread_mutex_t grid_lock;
pthread_mutex_t queue_lock;
pthread_mutex_t insert_lock;
/*
* lock_init Initializes all mutexes used in the program
*/
void lock_init(grid_t* gridPtr) { /* 1 lock per coordinate */
	int gridSize = gridPtr->width*gridPtr->height*gridPtr->depth; /* Number of coordinates*/	
	int i;
	// maybe need to alloc mutexes[i] because only mutexes (global) has been allocated
	for (i = 0; i < gridSize; i++) {
		if(pthread_mutex_init(gridPtr->mutexes[i], NULL)!=0){
			fprintf(stderr, "Failed to initiate mutex.\n");
			exit(1);
		}
	}

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

/* ======= Grid mutexes ====== */
void lock_alloc(grid_t* gridPtr) {
	int gridSize = gridPtr->width*gridPtr->height*gridPtr->depth; /* Number of coordinates*/	
	int i;

	for(i = 0; i < gridSize; i++) {
		gridPtr->mutexes[i] = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	}
		
}
void lock_free(grid_t* gridPtr) {
	int gridSize = gridPtr->width*gridPtr->height*gridPtr->depth; /* Number of coordinates*/	
	int i;

	for(i = 0; i < gridSize; i++) {
		free(gridPtr->mutexes[i]);
	}
}

/* ======= Specialized mutexes ====== */

void lock_queue() {
	if (pthread_mutex_lock(&queue_lock)!=0) {
		fprintf(stderr, "Failed to lock.\n");
		exit(1);
	}
}

void unlock_queue() {
	if (pthread_mutex_unlock(&queue_lock)!=0) {
		fprintf(stderr, "Failed to unlock.\n");
		exit(1);
	}
}

void lock_insert() {
	if (pthread_mutex_lock(&insert_lock)!=0) {
		fprintf(stderr, "Failed to lock.\n");
		exit(1);
	}
}

void unlock_insert() {
	if (pthread_mutex_unlock(&insert_lock)!=0) {
		fprintf(stderr, "Failed to unlock.\n");
		exit(1);
	}
}
