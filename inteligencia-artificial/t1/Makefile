#CFLAGS = -Wall -g
CFLAGS = -O2

all:
	gcc -o n_queens n_queens.c $(CFLAGS)

clean:
	rm -f *~
	rm -f n_queens

x: clean all

memcheck:
	valgrind --tool=memcheck --leak-check=full ./n_queens 2 8
