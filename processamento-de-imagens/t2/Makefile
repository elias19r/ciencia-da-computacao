CC      = gcc
CFLAGS  = -g

all: clean main

main:
	$(CC) `pkg-config --cflags opencv` -o ./bin/main ./src/main.c `pkg-config --libs opencv` $(CFLAGS)

clean_project: clean
	find -name ".tags*" | xargs rm -rf
	find -name "*.sublime*" | xargs rm -rf

clean:
	find -name "*~" | xargs rm -rf
	rm -f ./bin/*

run:
	./bin/main -i lena.jpg -g -l --log

memcheck:
	valgrind --tool=memcheck --leak-check=full ./bin/main -i lena.jpg -g -l --log

