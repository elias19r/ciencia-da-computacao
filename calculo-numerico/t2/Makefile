all:
	gcc -o t2_euler-mod main.c -lm -Wall -g
#	gcc -o t2_euler-mod main.c -lm

clean:
	rm -rf *~
	rm -rf *.o
	rm -rf t2_euler-mod

run:
	./t2_euler-mod

x: clean all run

memcheck: clean all
	valgrind --tool=memcheck --leak-check=full ./t2_euler-mod
