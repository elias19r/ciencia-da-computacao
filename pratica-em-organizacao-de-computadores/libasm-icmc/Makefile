all: clean build-montador build-simulador

build-montador:
	gcc ./montador/src/*.c -o ./montador/bin/montador

build-simulador:
	g++ ./simulador/src/main.cpp ./simulador/src/View.cpp ./simulador/src/Controller.cpp ./simulador/src/Model.cpp -O3 -march=native -o ./simulador/bin/simulador `pkg-config --libs --cflags gtk+-2.0` -lcurses -pthread -lgthread-2.0

run:
	./montador/bin/montador $(asm).asm $(asm).mif
	./simulador/bin/simulador $(asm).mif charmap.mif

clean:
	rm -f ./*~
	rm -f ./montador/src/*~
	rm -f ./montador/src/*.o
	rm -f ./simulador/src/*~
	rm -f ./simulador/src/*.o
	rm -f ./montador/bin/montador	
	rm -f ./simulador/bin/simulador
	find . -maxdepth 1 ! -name "charmap.mif" -name "*.mif" -type f -delete

