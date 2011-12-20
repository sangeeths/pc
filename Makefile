
CC=gcc

SOURCES=pc.c pc-cmd.c q.c

CFLAGS=-lpthread -Wall

all: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -o pc

clean:
	rm -rf pc *.o

