CC     = gcc
#CFLAGS = -Wall -g

all: sequential parallel stats

stats: 
	$(CC) -o ./bin/stats ./src/stats.c $(CFLAGS) -lm

parallel:
	$(CC) -o ./bin/parallel ./src/dijkstra.c ./src/floyd.c ./src/main.c -I./inc $(CFLAGS) -fopenmp

sequential:
	$(CC) -o ./bin/sequential ./src/dijkstra.c ./src/floyd.c ./src/main.c -I./inc $(CFLAGS)

clean:
	find -name "*~" | xargs rm -rf
	rm -f ./test/tmp_*
	rm -f ./bin/*

memcheck:
	valgrind --tool=memcheck --leak-check=full ./bin/sequential $(ALG) < input03

