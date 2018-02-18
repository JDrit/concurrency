#define _GNU_SOURCE

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "countdown.h"
#include "barrier.h"

const size_t NUM_THREADS = 20;

struct ThreadInfo {
  int threadId;
  barrier_t* barrier;
  countdown_t* latch;
};

void* ThreadEntry(void *entry) {  
  const int myId = ((struct ThreadInfo*) entry)->threadId;
  countdown_t* latch = ((struct ThreadInfo*) entry)->latch;
  barrier_t* barrier = ((struct ThreadInfo*) entry)->barrier;

  // let all threads start at the same time
  barrier_wait(barrier);

  const int workLoops = 2;
  for (int i = 0 ; i < workLoops ; i++) {
    printf("[thread %02d] working (%d/%d)\n", myId, i, workLoops);
    sleep(1);
  }

  printf("[thread %02d] Signalling cond.\n", myId);
  countdown_decrement(latch, 1);
  
  return NULL;
}

int main(int argc, char **argv) {
  printf("[thread main] starting\n");

  barrier_t barrier;
  barrier_init(&barrier, NUM_THREADS);

  countdown_t countdown;
  countdown_init(&countdown, NUM_THREADS);
  
  pthread_t threads[NUM_THREADS];
  struct ThreadInfo infos[NUM_THREADS];
  
  for (int i = 0 ; i < NUM_THREADS ; i++) {
    infos[i].threadId = i;
    infos[i].latch = &countdown;
    infos[i].barrier = &barrier;
    
    int result = pthread_create(&threads[i], NULL, ThreadEntry, (void*) &infos[i]);
    assert(result == 0);
    pthread_detach(threads[i]);
  }

  // wait till all the threads are done
  countdown_wait(&countdown);

  // clean up all the state
  barrier_destroy(&barrier);
  countdown_destroy(&countdown);

  printf("[thread main] done = %d, finished\n", (int) NUM_THREADS);
  
}
