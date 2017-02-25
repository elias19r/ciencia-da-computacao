#ifndef _MYFUNC_H_
#define _MYFUNC_H_

#define MYFUNC_ENTER 10

/**
 * Escaneia o teclado, pega uma palavra e retorna
 */
char *scanw();

/**
 * Escaneia o teclado, pega uma linha e retorna
 */
char *scanln();

/**
 * Escaneia um arquivo, pega uma linha e retorna
 */
char *fscanln(FILE *fp);

/**
 * Escaneia um arquivo, pega uma palavra e retorna
 */
char *fscanw(FILE *fp);

/**
 * Escaneia uma string, pega uma palavra e retorna
 */
char *nextw(char *buffer);

/**
 * Escaneia uma string, pega uma linha e retorna
 */
char *nextln(char *buffer);

/**
 * Recebe uma string e separada suas palavras, retornando um vetor de strings
 * e a quantidade de palavras encontradas
 * Como se fosse o argc e argv do programa padrão
 * Obs: essa função pode retornar NULL
 */
char **getargs(const char *str, int *argc);

/**
 * Remove os espaços em branco de uma string
 */
void rmspaces(char *str);

#endif
