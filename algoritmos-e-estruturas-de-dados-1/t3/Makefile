BIN=projeto03
SRC=projeto03.c
OBJ=projeto03.o
LIBNAME=pilha
LIBSRC=pilha.c
LIBOBJ=pilha.o
OBJECTS=$(OBJ) $(LIBOBJ)

all: $(OBJECTS)
	gcc -o $(BIN) $(OBJECTS)

$(OBJ):
	gcc -c $(SRC)

$(LIBOBJ):
	gcc -c $(LIBSRC)

clean:
	rm -rf $(BIN) $(OBJECTS) *~
