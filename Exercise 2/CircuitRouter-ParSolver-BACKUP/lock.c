#include "lock.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t grid_lock;
pthread_mutex_t queue_lock;
pthread_mutex_t insert_lock;

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

void lock_grid() {
	if (pthread_mutex_lock(&grid_lock)!=0) {
		fprintf(stderr, "Failed to lock.\n");
		exit(1);
	}
}

void unlock_grid() {
	if (pthread_mutex_unlock(&grid_lock)!=0) {
		fprintf(stderr, "Failed to lock.\n");
		exit(1);
	}
}

void lock_queue() {
	if (pthread_mutex_lock(&queue_lock)!=0) {
		fprintf(stderr, "Failed to lock.\n");
		exit(1);
	}
}

void unlock_queue() {
	if (pthread_mutex_unlock(&queue_lock)!=0) {
		fprintf(stderr, "Failed to lock.\n");
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
		fprintf(stderr, "Failed to lock.\n");
		exit(1);
	}
}
