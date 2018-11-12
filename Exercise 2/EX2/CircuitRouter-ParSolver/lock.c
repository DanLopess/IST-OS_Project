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

void lock_destroy(grid_t* gridPtr) { /* 1 lock per coordinate */
	int gridSize = gridPtr->width*gridPtr->height*gridPtr->depth; /* Number of coordinates*/
	int i;

	for (i = 0; i < gridSize; i++) {
		if(pthread_mutex_destroy(gridPtr->mutexes[i])!=0){
			fprintf(stderr, "Failed to destroy mutex.\n");
			exit(1);
		}
	}

	if(pthread_mutex_destroy(&grid_lock)!=0){
		fprintf(stderr, "Failed to destroy mutex.\n");
		exit(1);
	}
	if(pthread_mutex_destroy(&queue_lock)!=0){
		fprintf(stderr, "Failed to destroy mutex.\n");
		exit(1);
	}
	if(pthread_mutex_destroy(&insert_lock)!=0){
		fprintf(stderr, "Failed to destroy mutex.\n");
		exit(1);
	}
}

/* ======= Grid mutexes ====== */
/* allocated all mutexes for grid locking */
void lock_alloc(grid_t* gridPtr) {
	int gridSize = gridPtr->width*gridPtr->height*gridPtr->depth; /* Number of coordinates*/
	gridPtr->mutexes = (pthread_mutex_t**) malloc(sizeof(pthread_mutex_t*)*gridSize);
	int i;

	for(i = 0; i < gridSize; i++) {
		gridPtr->mutexes[i] = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	}

}
/* frees all mutexes for grid locking */
void lock_free(grid_t* gridPtr) {
	int gridSize = gridPtr->width*gridPtr->height*gridPtr->depth; /* Number of coordinates*/
	int i;

	for(i = 0; i < gridSize; i++) {
		free(gridPtr->mutexes[i]);
	}
	free(gridPtr->mutexes);
}

/* returns a given mutex based on a point in the index of pointVectorPtr  */
pthread_mutex_t* getMutex(grid_t* gridPtr, vector_t* pointVectorPtr, int index) {
	long* xPtr = (long*) malloc(sizeof(long));
	long* yPtr = (long*) malloc(sizeof(long));
	long* zPtr = (long*) malloc(sizeof(long));
	long* gridPointPtr = (long*)vector_at(pointVectorPtr, index);
	pthread_mutex_t* lock;

	grid_getPointIndices (gridPtr, gridPointPtr, xPtr, yPtr, zPtr);
	lock = grid_getMutexRef(gridPtr, *xPtr,*yPtr,*zPtr);
	free(xPtr);
	free(yPtr);
	free(zPtr);

	return (lock);
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
