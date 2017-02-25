PROGRAM = t1-shortest-path
ZIP     = $(PROGRAM).zip

CC     = gcc
CFLAGS = -Wall -g

SRCEXT = .c
OBJEXT = .o

INC  = ./inc
OBJ  = ./obj
SRC  = ./src

SOURCES := $(wildcard $(SRC)/*$(SRCEXT))
OBJECTS := $(SOURCES:$(SRC)/%$(SRCEXT)=$(OBJ)/%$(OBJEXT))

all: $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS)

$(OBJECTS): $(OBJ)/%$(OBJEXT) : $(SRC)/%$(SRCEXT)
	$(CC) -c $< -o $@ $(CFLAGS) -I$(INC)

clean:
	rm -f *~
	rm -f $(PROGRAM)
	rm -f $(ZIP)
	rm -f $(INC)/*~
	rm -f $(OBJ)/*$(OBJEXT)
	rm -f $(SRC)/*~

zip: clean
	zip -r $(ZIP) *

