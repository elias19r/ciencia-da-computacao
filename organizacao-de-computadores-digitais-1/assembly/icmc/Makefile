PROGRAM = cmp
CHARMAP = charmap

all: clean
	montador $(PROGRAM).asm $(PROGRAM).mif
	simulador $(PROGRAM).mif $(CHARMAP).mif

clean:
	rm -f *~
