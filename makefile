all: raycast.o ppmrw.o vector.o parsing.o
	gcc raycast.o ppmrw.o vector.o parsing.o -o raycast

raycast.o: raycast.c raycast.h
	gcc -c raycast.c

ppmrw.o: ppmrw.c ppmrw.h
	gcc -c ppmrw.c

vector.o: vector.c vector.h
	gcc -c vector.c

parsing.o: parsing.c parsing.h
	gcc -c parsing.c

clean:
	rm -rf *.o *.stackdump *.exe 2>/dev/null || true
