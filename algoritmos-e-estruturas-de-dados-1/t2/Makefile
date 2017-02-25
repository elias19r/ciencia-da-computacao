CC=gcc
CFLAGS=-Wall
objects = projeto02.o listaestatica.o

projeto02 : $(objects)
	$(CC) -o projeto02 $(objects)

projeto02.o : listaestatica.h
conjunto.o : listaestatica.h	
clean :
	rm projeto02 $(objects)
