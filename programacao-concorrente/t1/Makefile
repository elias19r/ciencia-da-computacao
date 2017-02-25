# Program name
PROGRAM = Trabalho1
ZIP     = $(PROGRAM).zip

# Compiler settings
CC     = gcc
CFLAGS = -O2
#CFLAGS = -Wall -g

all:
	$(CC) -o ./bin/gauss-legendre ./src/gauss-legendre.c -I$(INC) $(CFLAGS) -lgmp
	$(CC) -o ./bin/gauss-legendre_pthread ./src/gauss-legendre_pthread.c -I$(INC) $(CFLAGS) -lgmp -pthread
	$(CC) -o ./bin/borwein ./src/borwein.c -I$(INC) $(CFLAGS) -lgmp
	$(CC) -o ./bin/borwein_pthread ./src/borwein_pthread.c -I$(INC) $(CFLAGS) -lgmp -pthread
	$(CC) -o ./bin/monte-carlo ./src/monte-carlo.c -I$(INC) $(CFLAGS) -lm
	$(CC) -o ./bin/monte-carlo_pthread ./src/monte-carlo_pthread.c -I$(INC) $(CFLAGS) -pthread -lm
	$(CC) -o ./bin/black-scholes ./src/black-scholes.c -I$(INC) $(CFLAGS) -lm
	$(CC) -o ./bin/black-scholes_pthread ./src/black-scholes_pthread.c -I$(INC) $(CFLAGS) -lm -pthread

clean:
	find -name "*~" | xargs rm -rf
	rm -f ./bin/*

clean-test:
	find -name "*~" | xargs rm -rf
	find -name "tempo*" | xargs rm -rf
	find -name "pi*" | xargs rm -rf
	find -name "intconf*" | xargs rm -rf

run:
	/usr/bin/time -f "%e" ./bin/$(EXE) < ./entrada_pi.txt > ./saida_pi.txt

run-bs:
	/usr/bin/time -f "%e" ./bin/black-scholes < ./entrada_blackscholes.txt > saida_blackscholes.txt

run-bsp:
	/usr/bin/time -f "%e" ./bin/black-scholes_pthread < ./entrada_blackscholes.txt > saida_blackscholes.txt

run-test:
	bash run-test_gl-b.sh
	bash run-test_mc.sh
	bash run-test_bs.sh

zip: clean
	zip -r $(ZIP) *

memcheck:
	valgrind --tool=memcheck --leak-check=full ./bin/$(EXE)

