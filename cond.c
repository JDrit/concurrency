#define _GNU_SOURCE

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "barrier.h"
#include "countdown.h"
#include "semaphore.h"

const size_t NUM_THREADS = 15;
const size_t SEMA_SIZE = 5;

struct ThreadInfo {
  int threadId;
  barrier_t* barrier;
  semaphore_t* semaphore;
  countdown_t* latch;
};

void* ThreadEntry(void *entry) {  
  const int myId = ((struct ThreadInfo*) entry)->threadId;
  countdown_t* latch = ((struct ThreadInfo*) entry)->latch;
  barrier_t* barrier = ((struct ThreadInfo*) entry)->barrier;
  semaphore_t* semaphore = ((struct ThreadInfo*) entry)->semaphore;

  // let all threads start at the same time
  barrier_wait(barrier);

  // only a couple of threads should be able to work at any
  // given time
  semaphore_acquire_single(semaphore);
  
  const int workLoops = 2;
  for (int i = 0 ; i < workLoops ; i++) {
    printf("[thread %02d] working (%d/%d)\n", myId, i, workLoops);
    sleep(1);
  }

  semaphore_release_single(semaphore);

  printf("[thread %02d] Signalling cond.\n", myId);
  countdown_decrement(latch, 1);
  
  return NULL;
}

int main() {
  printf("[thread main] starting\n");

  barrier_t barrier;
  barrier_init(&barrier, NUM_THREADS);

  countdown_t countdown;
  countdown_init(&countdown, NUM_THREADS);

  semaphore_t semaphore;
  semaphore_init(&semaphore, SEMA_SIZE);
  
  pthread_t threads[NUM_THREADS];
  struct ThreadInfo infos[NUM_THREADS];
  
  for (size_t i = 0 ; i < NUM_THREADS ; i++) {
    infos[i].threadId = i;
    infos[i].latch = &countdown;
    infos[i].barrier = &barrier;
    infos[i].semaphore = &semaphore;
    
    int result = pthread_create(&threads[i], NULL, ThreadEntry, (void*) &infos[i]);
    assert(result == 0);
    pthread_detach(threads[i]);
  }

  // wait till all the threads are done
  countdown_wait(&countdown);

  // clean up all the state
  barrier_destroy(&barrier);
  countdown_destroy(&countdown);
  semaphore_destroy(&semaphore);
  
  printf("[thread main] done = %d, finished\n", (int) NUM_THREADS);
  
}
