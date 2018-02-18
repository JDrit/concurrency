#ifndef __SEMAPHORE
#define __SEMAPHORE

#include <stdatomic.h>
#include <stdint.h>
#include <pthread.h>

typedef struct Semaphore {
  pthread_mutex_t lock;
  pthread_cond_t cond;
  atomic_int currentSize;
  size_t maxSize;
} semaphore_t;

/**
 * Initilizes the semaphore with the given amount of locks equal to `size`.
 */
void semaphore_init(semaphore_t* semaphore, size_t size);

void semaphore_destroy(semaphore_t* semaphore);

/**
 * A helper function when the amount that is being acquired is always
 * one.
 */
void semaphore_acquire_single(semaphore_t* semaphore);

/**
 * Tries to get the specified amount of locks from the semaphore. This will 
 * block if there is not enough avialable. If the amount being asked for is
 * greater than the total amount in the semaphore than this will block forever.
 * The code will block here if there are not currently enough locks to fullfill
 * the request.
 * 
 * It is undefinied behavior if the amount being passed in is negative or zero.
 */
void semaphore_acquire(semaphore_t* semaphore, size_t amount);

/**
 * A helper function when the amount that is being released is always one.
 */
void semaphore_release_single(semaphore_t* semaphore);

/**
 * Let's go of the given amount of locks from the semaphore and potentially
 * notifies other threads that they can go. It is undefined what happens if
 * the number passed in is negative.
 */
void semaphore_release(semaphore_t* semaphore, size_t amount);

/**
 * Atomically gets the current number of used locks.
 */
size_t semaphore_get_currentUsage(semaphore_t* semaphore);

/**
 * Gets the max amount of locks that this semaphore can hand out.
 */
size_t semaphore_get_max_size(semaphore_t* semaphore);

#endif
