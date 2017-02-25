CC=gcc
CFLAGS=-Wall
objects = projeto01.o conjunto.o

projeto01 : $(objects)
	$(CC) -o projeto01 $(objects)

projeto01.o : conjunto.h
conjunto.o : conjunto.h	
clean :
	rm projeto01 $(objects)
