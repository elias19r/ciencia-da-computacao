ZIP = T2-G07-ELIAS.zip

all:
	gcc -c memory_manager.c -o memory_manager.o -g -Wall
	gcc -c main.c -o main.o -g -Wall
	gcc -o main main.o memory_manager.o

run:
	./main

clean:
	rm -f *~
	rm -f *.o
	rm -f main
	rm -f $(ZIP)

x: clean all run

memcheck:
	valgrind --tool=memcheck --leak-check=full ./main

zip: clean
	zip -r $(ZIP) *

