all:
#	gcc -o t1_gauss-seidel main.c -lm -Wall -g
	gcc -o t1_gauss-seidel main.c -lm

clean:
	rm -rf *~
	rm -rf *.o
	rm -rf t1_gauss-seidel

run:
	./t1_gauss-seidel

x: clean all run

memcheck: clean all
	valgrind --tool=memcheck --leak-check=full ./t1_gauss-seidel < caso_c > out_caso_c
