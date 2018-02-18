
#include <pthread.h>
#include <stdint.h>
#include <stdatomic.h>
#include "countdown.h"

void countdown_init(countdown_t* countdown, size_t size) {
  countdown->count = size;
  pthread_mutex_init(&countdown->mutex, NULL);
  pthread_cond_init(&countdown->conditional, NULL);
}

void countdown_destroy(countdown_t* countdown) {
  pthread_mutex_destroy(&countdown->mutex);
  pthread_cond_destroy(&countdown->conditional);
}

void countdown_decrement(countdown_t* countdown, size_t amount) {
  int orgValue = atomic_fetch_sub(&countdown->count, 1);

  if (orgValue == 1) {
    pthread_cond_broadcast(&countdown->conditional);
  }
}

void countdown_wait(countdown_t* countdown) {
  pthread_mutex_lock(&countdown->mutex);
  int value = atomic_load(&countdown->count);
  
  while (value > 0) {
    pthread_cond_wait(&countdown->conditional, &countdown->mutex);
    value = atomic_load(&countdown->count);
  }

  pthread_mutex_unlock(&countdown->mutex);
}

