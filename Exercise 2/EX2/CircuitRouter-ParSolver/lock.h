#ifndef LOCK_H
#define LOCK_H 1

pthread_mutex_t grid_lock, queue_lock, insert_lock;

void lock_init();

void grid_lock();

void grid_unlock();

void queue_lock();

void queue_unlock();

void insert_lock();

void insert_unlock();


#endif
