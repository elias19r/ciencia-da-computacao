/**
 * ICMC-USP
 * Implementacao de backtracking para colorir mapas
 * Elias Italiano Rodrigues, 7987251
 * Rodolfo Megiato de Lima,  7987286
 * 2013/09
 */

#ifndef _MYLIB_H_
#define _MYLIB_H_

#define _MYLIB_ENTER_ 10

/**
 * Remove caracteres (espacos) do comeco e do fim de uma string
 */
void trim(char **word);

/**
 * Escaneia o teclado, pega uma palavra e retorna
 */
char *scan();

/**
 * Escaneia o teclado, pega uma linha e retorna
 */
char *scanln();

/**
 * Escaneia uma string, pega uma palavra e retorna
 */
char *next(char *buffer);

#endif

