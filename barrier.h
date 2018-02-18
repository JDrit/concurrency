#ifndef __BARRIER
#define __BARRIER

#include "countdown.h"

typedef struct Barrier {
  countdown_t countdown;
} barrier_t;

void barrier_init(barrier_t* barrier, size_t size);

void barrier_destroy(barrier_t* barrier);

void barrier_wait(barrier_t* barrier);

#endif
