# Compiladores e flags
CC       = gcc
CFLAGS   = -g
LEX      = flex

# Curinga para pegar todos os .lalg do diretorio ./test
TESTS_IN  := $(wildcard ./test/*.lalg)
TESTS_OUT := $(TESTS_IN:./test/%.lalg=./test/%_out)

# Target principal
all: clean main

# Compila todo o programa
main:
	$(LEX) -o ./src/lalg.c ./src/lalg.l
	$(CC) -o ./bin/main ./src/main.c ./src/lalg.c $(CFLAGS)

# Limpa arquivos compilados e temporarios
clean: clean_test
	find -name "*~" | xargs rm -rf
	rm -f ./bin/*
	rm -f ./src/lalg.c

# Limpa as saidas dos testes
clean_test:
	rm -f ./test/*_out

# Executa para todos os arquivos .lalg dentro de ./test
run: clean_test $(TESTS_OUT)

$(TESTS_OUT): ./test/%_out : ./test/%.lalg
	./bin/main < $< > $@

# Verifica vazao de memoria
memcheck:
	valgrind --tool=memcheck --leak-check=full ./bin/main < test/programa1.lalg

