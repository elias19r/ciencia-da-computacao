CC      = gcc
MPICC   = mpicc
NVCC    = nvcc
CFLAGS  = -Wall -g

all: stats sequential parallel cuda

stats: 
	$(CC) -o ./bin/stats ./src/stats.c $(CFLAGS) -lm

sequential:
	$(CC) -o ./bin/sequential ./src/sequential.c ./src/ppm_p3.c ./src/smooth.c $(CFLAGS)

parallel:
	$(MPICC) -o ./bin/parallel ./src/parallel.c ./src/ppm_p3.c ./src/smooth.c $(CFLAGS)

cuda:
	$(NVCC) -o ./bin/cuda ./src/cuda.cu

clean:
	find -name "*~" | xargs rm -rf
	rm -f ./test/tmp_*
	rm -f ./bin/*

memcheck:
	valgrind --tool=memcheck --leak-check=full ./bin/sequential -n < in.ppm

