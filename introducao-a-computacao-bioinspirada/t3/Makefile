CC     = gcc
CFLAGS = -g -Wall
LFLAGS = -lm -lrt -pthread

all: clean main

# Compile using -O2 flag
done: clean
	$(CC) -o aco-tsp main.c -O2 $(LFLAGS)

main:
	$(CC) -o aco-tsp main.c $(CFLAGS) $(LFLAGS)

clean:
	find -name "*~" | xargs rm -rf
	rm -f ./aco-tsp

run:
	./aco-tsp

memcheck:
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./aco-tsp -t 2 -n 10 -k 10

