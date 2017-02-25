#ifndef _MYLIB_H_
#define _MYLIB_H_

#include <stdio.h>

#define MYLIB_ENTER 10

/**
 * Escaneia o teclado, pega uma palavra e retorna
 */
char *scan();

/**
 * Escaneia o teclado, pega uma linha e retorna
 */
char *scanln();

/**
 * Escaneia um arquivo, pega uma palavra e retorna
 */
char *fscan(FILE *fp);

/**
 * Escaneia um arquivo, pega uma linha e retorna
 */
char *fscanln(FILE *fp);

/**
 * Escaneia uma string, pega uma palavra e retorna
 */
char *next(char *buffer);

/**
 * Escaneia uma string, pega uma linha e retorna
 */
char *nextln(char *buffer);

/**
 * Recebe uma string e separada suas palavras, retornando um vetor de strings
 * e a quantidade de palavras encontradas
 * Como se fosse o argc e argv do programa padrao
 * Obs: essa funcao pode retornar NULL
 */
char **getargs(const char *str, int *argc);

/**
 * Remove os espacos em branco de uma string
 */
void rmspaces(char *str);

#endif
