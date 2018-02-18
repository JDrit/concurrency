#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>

#include "semaphore.h"

void semaphore_init(semaphore_t* semaphore, size_t size) {
  pthread_mutex_init(&semaphore->lock, NULL);
  pthread_cond_init(&semaphore->cond, NULL);
  semaphore->currentSize = 0;
  semaphore->maxSize = size;
}

void semaphore_destroy(semaphore_t* semaphore) {
  pthread_mutex_destroy(&semaphore->lock);
  pthread_cond_destroy(&semaphore->cond);
}

void semaphore_acquire_single(semaphore_t* semaphore) {
  semaphore_acquire(semaphore, 1);
}

void semaphore_acquire(semaphore_t* semaphore, size_t amount) {
  pthread_mutex_lock(&semaphore->lock);

  while (semaphore->currentSize + amount >= semaphore->maxSize) {
    pthread_cond_wait(&semaphore->cond, &semaphore->lock);
  }
  semaphore->currentSize += amount;
  
  pthread_mutex_unlock(&semaphore->lock);
}

void semaphore_release_single(semaphore_t* semaphore) {
  semaphore_release(semaphore, 1);
}

void semaphore_release(semaphore_t* semaphore, size_t amount) {
  int orgValue = atomic_fetch_sub(&semaphore->currentSize, amount);

  if (orgValue - amount < semaphore->maxSize) {
    pthread_cond_broadcast(&semaphore->cond);
  }
}

size_t semaphore_get_current_usage(semaphore_t* semaphore) {
  return atomic_load(&semaphore->currentSize);
}

size_t semaphore_get_max_size(semaphore_t* semaphore) {
  return semaphore->maxSize;
}
