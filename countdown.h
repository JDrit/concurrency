#ifndef __COUNTDOWN_LATCH
#define __COUNTDOWN_LATCH

#include <stdatomic.h>
#include <pthread.h>

typedef struct Countdown {
  atomic_int count;
  pthread_mutex_t mutex;
  pthread_cond_t conditional;
} countdown_t;

void countdown_init(countdown_t* countdown, size_t size);

void countdown_destroy(countdown_t* countdown);

void countdown_decrement(countdown_t* countdown, size_t amount);

void countdown_wait(countdown_t* countdown);

#endif
