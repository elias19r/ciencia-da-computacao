# Program name
PROGRAM = projeto02
ZIP     = $(PROGRAM).zip

# Project directories
DATA = ./data
OBJ  = ./obj
SRC  = ./src

# File extensions
SRCEXT = .c
OBJEXT = .o

# Compiler settings
CC     = gcc
CFLAGS = -lm -O3
#CFLAGS = -Wall -g -lm -O3

# Wildcard to compile every source-file into a respective object-file
SOURCES := $(wildcard $(SRC)/*$(SRCEXT))
OBJECTS := $(SOURCES:$(SRC)/%$(SRCEXT)=$(OBJ)/%$(OBJEXT))

# Main target
all: $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS)

$(OBJECTS): $(OBJ)/%$(OBJEXT) : $(SRC)/%$(SRCEXT)
	$(CC) -c $< -o $@ $(CFLAGS)

# Just run it :)
run:
	./$(PROGRAM)

# Trashing files
clean:
	rm -f *~
	rm -f $(PROGRAM)
	rm -f $(ZIP)
	rm -f $(OBJ)/*$(OBJEXT)
	rm -f $(SRC)/*~

# Compress into .zip
zip: clean
	zip -r $(ZIP) *

# All-in-one target ^_^
x: clean all run

# Memory checking
memcheck: all
	valgrind --tool=memcheck --leak-check=full ./$(PROGRAM)

