ALGORITHM = 0
#Algorithm
#	0 = FIFO
#	1 = Round-Robin
#	2 = Lottery
#	3 = Priority

all:
	mkdir build
	javac ./src/br/usp/icmc/so/simulador/*.java -d ./build

run:
	java -cp ./build br.usp.icmc.so.simulador.Main $(ALGORITHM)

clean:
	rm -rf build
	rm -f *~
	rm -f ./src/br/usp/icmc/so/simulador/*~
	rm -f 7987251_4321350_7987286.tar.gz

tar: clean
	tar -czvf 7987251_4321350_7987286.tar.gz *

x: clean all run
