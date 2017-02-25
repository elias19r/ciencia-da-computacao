INPUT  = < ./test/input0.data
#OUTPUT = > ./test/mello.out

CASENAME    = ./test/02.in
TIMES       = 1000
N           = 20
SIZE        = 100
CROSSOVER   = 50.0
MUTATION    = 10.0
MAXDISTANCE = 1500
SEED        = 43563719

all:
	javac -cp ./build:. -d ./build -Xlint src/*.java

clean:
	rm -rf ./build
	mkdir build
	find -name "*~" | xargs rm -rf
	rm -f poo-ag.zip

run:
	java -cp ./build:. Program $(INPUT) $(OUTPUT)

input-gen:
	gcc -o input-gen input-gen.c -g -Wall
	./input-gen $(TIMES) $(N) $(SIZE) $(CROSSOVER) $(MUTATION) $(MAXDISTANCE) $(SEED) > $(CASENAME)

zip: clean
	zip -r poo-ag.zip *

x: clean all run
