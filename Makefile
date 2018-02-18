
CC=clang
LIBS=-lpthread
CFLAGS=-g -Wall -Wextra -std=c11 -O3
TARGET=cond

all: $(TARGET)

rwlock.o: rwlock.c rwlock.h
	$(CC) $(CFLAGS) -c rwlock.c

semaphore.o: semaphore.c semaphore.h
	$(CC) $(CFLAGS) -c semaphore.c

barrier.o: barrier.c barrier.h
	$(CC) $(CFLAGS) -c barrier.c

countdown.o: countdown.c countdown.h
	$(CC) $(CFLAGS) -c countdown.c

$(TARGET): cond.c countdown.h countdown.o barrier.h barrier.o semaphore.h semaphore.o rwlock.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c countdown.o barrier.o semaphore.o rwlock.o $(LIBS)

clean:
	rm $(TARGET) *.o
