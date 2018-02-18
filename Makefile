
CC=clang
LIBS=-lpthread
CFLAGS=-g -Wall -std=c11
TARGET=cond

all: $(TARGET)

barrier.o: barrier.c
	$(CC) $(CFLAGS) -c barrier.c

countdown.o: countdown.c
	$(CC) $(CFLAGS) -c countdown.c

$(TARGET): cond.c countdown.h countdown.o barrier.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c countdown.o barrier.o $(LIBS)

clean:
	rm $(TARGET)
