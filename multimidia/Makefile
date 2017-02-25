#MUTE = @
PROGRAM  = zbm

CC     = gcc
#CFLAGS = -Wall -g

SRC = main.c
EXE = $(PROGRAM)
ZIP = $(PROGRAM).zip

all: clean
	$(MUTE)$(CC) -o $(EXE) $(SRC) $(CFLAGS)

run:
	$(MUTE)./$(EXE)

clean:
	$(MUTE)rm -f *~
	$(MUTE)rm -f $(EXE)
	$(MUTE)rm -f $(ZIP)
	$(MUTE)rm -f *_restored.bmp
	$(MUTE)rm -f *.bmpz

x: clean all run

zip: clean
	zip -r $(ZIP) *

memcheck: all
	valgrind --tool=memcheck --leak-check=full ./$(EXE)

