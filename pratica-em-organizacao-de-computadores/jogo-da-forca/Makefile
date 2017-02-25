PROGRAM = jogo-da-forca
CHARMAP = charmap

all: clean
	montador $(PROGRAM).asm $(PROGRAM).mif
	simulador $(PROGRAM).mif $(CHARMAP).mif

clean:
	rm -f *~
