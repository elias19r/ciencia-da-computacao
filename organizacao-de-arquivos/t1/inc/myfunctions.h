#ifndef _SCANSTR_H_
#define _SCANSTR_H_

#define ENTER 10

/**
 * Escaneia o teclado e pega uma string (caractere por caractere)
 * Usado no lugar de scanf, gets, fgets pra não ter problemas de leitura
 */
char *scan_str();

/**
 * Remove os espaços em branco de uma string
 */
void remove_spaces(char *str);

#endif
