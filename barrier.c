#include "countdown.h"
#include "barrier.h"

void barrier_init(barrier_t* barrier, size_t size) {
  countdown_init(&barrier->countdown, size);
}

void barrier_destroy(barrier_t* barrier) {
  countdown_destroy(&barrier->countdown);
}

void barrier_wait(barrier_t* barrier) {
  countdown_decrement(&barrier->countdown, 1);
  countdown_wait(&barrier->countdown);
}
