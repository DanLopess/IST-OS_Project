#ifndef LOCK_H
#define LOCK_H 1

#include "grid.h"


void lock_init(grid_t* gridPtr);

void lock_grid();

void unlock_grid();

void lock_queue();

void unlock_queue();

void lock_insert();

void unlock_insert();


#endif
