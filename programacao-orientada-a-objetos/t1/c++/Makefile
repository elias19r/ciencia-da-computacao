MUTE = @
PROGRAM  = t1
#INPUT    = < ./../input
#OUTPUT   = > ./../ouput

CC     = g++
#CFLAGS = -Wall -g

SRCEXT = .cpp
OBJEXT = .o

BIN  = ./bin
FILE = ./file
INC  = ./inc
OBJ  = ./obj
SRC  = ./src

ZIP = $(PROGRAM).zip
EXE = $(BIN)/$(PROGRAM)

SOURCES := $(wildcard $(SRC)/*$(SRCEXT))
OBJECTS := $(SOURCES:$(SRC)/%$(SRCEXT)=$(OBJ)/%$(OBJEXT))

all: clean $(OBJECTS)
	$(MUTE)$(CC) -o $(EXE) $(OBJECTS)

$(OBJECTS): $(OBJ)/%$(OBJEXT) : $(SRC)/%$(SRCEXT)
	$(MUTE)$(CC) -c $< -o $@ $(CFLAGS) -I$(INC)

run: clear_files
	$(MUTE)$(EXE) $(INPUT) $(OUTPUT)

clean: clear_files
	$(MUTE)rm -f *~
	$(MUTE)rm -f $(SRC)/*~
	$(MUTE)rm -f $(INC)/*~
	$(MUTE)rm -f $(OBJ)/*$(OBJEXT)
	$(MUTE)rm -f $(EXE)
	$(MUTE)rm -f $(ZIP)

clear_files:
	$(MUTE)rm -f $(FILE)/*

x: all run

zip: clean
	zip -r $(ZIP) *

memcheck: all
	valgrind --tool=memcheck --leak-check=full $(EXE) $(INPUT)
