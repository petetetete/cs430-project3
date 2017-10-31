CC = gcc
CFLAGS = -Wall -Wextra -c
LFLAGS = -Wall -Wextra -lm

all: raycast.o ppmrw.o vector.o parsing.o helpers.o
	$(CC) $(LFLAGS) raycast.o ppmrw.o vector.o parsing.o helpers.o -o raycast

raycast.o: raycast.c raycast.h
	$(CC) $(CFLAGS) raycast.c

ppmrw.o: ppmrw.c ppmrw.h
	$(CC) $(CFLAGS) ppmrw.c

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) vector.c

parsing.o: parsing.c parsing.h
	$(CC) $(CFLAGS) parsing.c

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) helpers.c

clean:
	rm -rf *.o *.stackdump *.exe 2>/dev/null || true
