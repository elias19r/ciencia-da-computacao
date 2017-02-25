ALGORITHM = 1
#Algorithm
#	0 = FIFO
#	1 = Round-Robin
#	2 = Lottery
#	3 = Priority
MEMORY_ALGORITHM = 0
#Memory Algorithm
#	0 = First-Fit
#	1 = Next-Fit
#	2 = Best-Fit
#	3 = Worst-Fit

all: clean
	mkdir build
	javac ./src/br/usp/icmc/so/simulador/*.java -d ./build

run:
	java -cp ./build br.usp.icmc.so.simulador.Main $(ALGORITHM) $(MEMORY_ALGORITHM)

clean:
	rm -rf build
	rm -f ./src/br/usp/icmc/so/simulador/*~
	rm -f project02.zip
	rm -f project02.tar.gz

tar: clean
	tar -czvf project02.tar.gz *

zip: clean
	zip -r project02.zip *

x: clean all run

