#ifndef __RW_LOCK
#define __RW_LOCK

#include <stdatomic.h>
#include <pthread.h>

/**
 * This is a multi-reader / single-writer lock that gives favor to incoming 
 * write requests over new read requests. Read requests will block when there
 * are any write requests that still need processing.
 */
typedef struct RWLOCK {
  atomic_int readCount;  // the amount of active readers
  atomic_int writeCount; // the amount of active writers, should be zero or one
  size_t writeRequests;  // if there are any blocking writes
  pthread_mutex_t lock;  // lock used internally
  pthread_cond_t cond;   // used to have threads wait on the locks
} rw_lock_t;

void rw_lock_init(rw_lock_t* lock);

void rw_lock_destroy(rw_lock_t* lock);

/**
 * Gets a read lock from the given lock input. Allows multiple read 
 * operations to go one at the same time. Blocks if there are any 
 * blocking writes or any active write requests.
 */
void rw_lock_read_lock(rw_lock_t* lock);

/**
 * Releases a read lock from the given input lock.
 */
void rw_lock_read_unlock(rw_lock_t* lock);

/**
 * Gets the single write lock for the given input lock. This will block
 * if there are any other active requests going on.
 */
void rw_lock_write_lock(rw_lock_t* lock);

/**
 * Releases the single write lock from the given input. This will allow
 * other writes and reads to continue.
 */
void rw_lock_write_unlock(rw_lock_t* lock);

#endif
