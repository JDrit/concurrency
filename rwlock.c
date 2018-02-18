
#include <stdatomic.h>
#include <pthread.h>
#include "rwlock.h"

void rw_lock_init(rw_lock_t* lock) {
  lock->readCount = 0;
  lock->writeCount = 0;
  lock->writeRequests = 0;
  pthread_mutex_init(&lock->lock, NULL);
  pthread_cond_init(&lock->cond, NULL);
}

void rw_lock_destroy(rw_lock_t* lock) {
  pthread_mutex_destroy(&lock->lock);
  pthread_cond_destroy(&lock->cond);
}

void rw_lock_read_lock(rw_lock_t* lock) {
  pthread_mutex_lock(&lock->lock);

  while (lock->writeCount > 0 || lock->writeRequests > 0) {
    pthread_cond_wait(&lock->cond, &lock->lock);
  }

  lock->readCount++;

  pthread_mutex_unlock(&lock->lock);  
}

void rw_lock_read_unlock(rw_lock_t* lock) {
  int orgValue = atomic_fetch_sub(&lock->readCount, 1);  

  if (orgValue == 1) {
    // lets the writers know that they can go
    pthread_cond_broadcast(&lock->cond);
  }
}

void rw_lock_write_lock(rw_lock_t* lock) {
  pthread_mutex_lock(&lock->lock);

  // let others know of the intent to write
  lock->writeRequests++;

  // writes cannot go when reads are still going on and when
  // there are other writes happening
  while (lock->readCount > 0 || lock->writeCount > 0) {
    pthread_cond_wait(&lock->cond, &lock->lock);
  }

  lock->writeRequests--;
  lock->writeCount++;

  pthread_mutex_unlock(&lock->lock);
}

void rw_lock_write_unlock(rw_lock_t* lock) {
  int orgValue = atomic_fetch_sub(&lock->writeCount, 1);

  if (orgValue == 1) {
    pthread_cond_broadcast(&lock->cond);
  }  
}
