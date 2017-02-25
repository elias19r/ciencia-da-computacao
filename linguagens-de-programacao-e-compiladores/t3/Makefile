# Compiladores e flags
CC       = gcc
CFLAGS   = -g
#YACC     = yacc
YACC     = bison
LEX      = flex

# Curinga para pegar todos os .lalg do diretorio ./test/sem
TESTS_IN  := $(wildcard ./test/sem/*.lalg)
TESTS_OUT := $(TESTS_IN:./test/sem/%.lalg=./test/sem/%_out)

# Target principal
all: clean main

# Compila todo o programa
main:
	$(YACC) -dy lalg.y -Wall
	$(LEX) lalg.l
	$(CC) lex.yy.c y.tab.c -o main $(CFLAGS)

# Limpa arquivos temporarios e compilados
clean: clean_test
	find -name "*~" | xargs rm -rf
	rm -f main
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f lex.yy.c

# Limpa as saidas dos testes
clean_test:
	rm -f ./test/sem/*_out

# Executa para todos os arquivos .lalg dentro de ./test/sem
run: clean_test $(TESTS_OUT)

$(TESTS_OUT): ./test/sem/%_out : ./test/sem/%.lalg
	./main < $< 2> ./test/tmp_out
	cat $< > $@
	printf "\n\nERRORS:\n" >> $@
	cat ./test/tmp_out >> $@
	rm -f ./test/tmp_out

# Verifica vazao de memoria
memcheck:
	valgrind --tool=memcheck --leak-check=full ./main < test/sem/test1.lalg

