CASE   = usa
# values: usa, brasil
METHOD = 3
# 0: normal backtracking
# 1: backtracking with forward-cheking
# 2: backtracking with forward-cheking, and MRV
# 3: backtracking with forward-cheking, MRV, and degree

# Program name
PROGRAM = color-map
ZIP     = $(PROGRAM).zip

# Project directories
DATA = ./data
INC  = ./inc
LIB  = ./lib
OBJ  = ./obj
SRC  = ./src

# File extensions
SRCEXT = .c
OBJEXT = .o

# Input and output files
INPUT  = < $(DATA)/$(CASE).in
OUTPUT = > $(DATA)/$(CASE).out

# Compiler settings
CC     = gcc
CFLAGS = -Wall

# Wildcard to compile every source-file into a respective object-file
SOURCES := $(wildcard $(SRC)/*$(SRCEXT))
OBJECTS := $(SOURCES:$(SRC)/%$(SRCEXT)=$(OBJ)/%$(OBJEXT))

# Main target
all: $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS)

$(OBJECTS): $(OBJ)/%$(OBJEXT) : $(SRC)/%$(SRCEXT)
	$(CC) -c $< -o $@ $(CFLAGS) -I$(INC)

# Just run it :)
run:
	./$(PROGRAM) $(METHOD) $(INPUT) $(OUTPUT)
	java -cp $(LIB)/map-viewer $(CASE) $(DATA)/$(CASE).out $(DATA)
	eog $(DATA)/$(CASE)_colored.svg

# Trashing files
clean:
	rm -f *~
	rm -f $(PROGRAM)
	rm -f $(ZIP)
	rm -f $(INC)/*~
	rm -f $(OBJ)/*$(OBJEXT)
	rm -f $(SRC)/*~
	rm -f $(DATA)/*.svg
	rm -f $(DATA)/*.out

# Compress into .zip
zip: clean
	zip -r $(ZIP) *

# All-in-one target ^_^
x: clean all run

# Memory checking
memcheck: all
	valgrind --tool=memcheck --leak-check=full ./$(PROGRAM) $(METHOD) $(INPUT)

