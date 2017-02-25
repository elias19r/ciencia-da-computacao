PROGRAM = fibonacci_logn
SRC     = main.c
ZIP     = $(PROGRAM).zip

CC     = gcc
CFLAGS = -O3

all:
	$(CC) -o $(PROGRAM) $(SRC) $(CFLAGS)

run:
	./$(PROGRAM)

clean:
	rm -f *~
	rm -f $(PROGRAM)
	rm -f $(ZIP)

zip: clean
	zip -r $(ZIP) *

x: clean all run

memcheck: all
	valgrind --tool=memcheck --leak-check=full ./$(PROGRAM)

