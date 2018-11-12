#ifndef LOCK_H
#define LOCK_H 1

#include "grid.h"


void lock_init(grid_t* gridPtr);

void lock_destroy(grid_t* gridPtr);

void lock_alloc(grid_t* gridPtr);

void lock_free(grid_t* gridPtr);

pthread_mutex_t* getMutex(grid_t* gridPtr, vector_t* pointVectorPtr, int index);

void lock_queue();

void unlock_queue();

void lock_insert();

void unlock_insert();


#endif
