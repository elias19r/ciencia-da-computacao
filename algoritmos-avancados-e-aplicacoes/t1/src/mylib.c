/**
 * ICMC-USP
 * Implementacao de backtracking para colorir mapas
 * Elias Italiano Rodrigues, 7987251
 * Rodolfo Megiato de Lima,  7987286
 * 2013/09
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <mylib.h>

/**
 * Remove caracteres (espacos) do comeco e do fim de uma string
 */
void trim(char **word) {
	char *p = *word, *aux;
	int len;

//	while (isspace(*p))
	while (isspace(*p) || *p == ',' || *p == '.')
		p++;

	len = strlen(p) - 1;
	while (len != 0) {
//		if (isspace(p[len]))
		if (isspace(p[len]) || p[len] == ',' || p[len] == '.')
			p[len] = '\0';
		else
			break;
		len--;
	}

	aux = (char *) malloc(sizeof(char) * (strlen(p) + 1));
	strcpy(aux, p);

	free(*word);
	*word = aux;
}

/**
 * Escaneia o teclado, pega uma palavra e retorna
 */
char *scan() {
	int count = 0;
	char c;
	char *str = NULL;

	while (1) {
		// Pega um caractere
		scanf("%c", &c);

		// Confere se eh espaco
//		if (isspace(c)) {
		if (c == ':' || c == ',') {
			// Se ja inseriu algo: finaliza a string
			if (count != 0) {
				str = (char *) realloc(str, sizeof(char) * (count + 1));
				str[count] = '\0';
				break;
			}
		// Copia caractere digitado para str
		} else {
			str = (char *) realloc(str, sizeof(char) * (count + 1));
			str[count++] = c;
		}
	}

	return str;
}

/**
 * Escaneia o teclado, pega uma linha e retorna
 */
char *scanln() {
	int count = 0;
	char c;
	char *str = NULL;

	while (1) {
		// Pega um caractere
		scanf("%c", &c);

		// Confere se teclou ENTER
		if (c == _MYLIB_ENTER_) {
			// Se ja inseriu algo: finaliza a string
			if (count != 0) {
				str = (char *) realloc(str, sizeof(char) * (count + 1));
				str[count] = '\0';
				break;
			}
		// Copia caractere digitado para str
		} else {
			str = (char *) realloc(str, sizeof(char) * (count + 1));
			str[count++] = c;
		}
	}

	return str;
}

/**
 * Escaneia uma string, pega uma palavra e retorna
 */
char *next(char *buffer) {
	static char *p;
	int count = 0;
	char *str = NULL;

	if (buffer != NULL)
		p = buffer;

	while (*p != '\0') {
		// Confere se eh espaco
//		if (isspace(*p)) {
		if (*p == ',' || *p == '.') {
			// Se ja inseriu algo: finaliza a string
			if (count != 0) {
				break;
			}
		// Copia caractere digitado para str
		} else {
			str = (char *) realloc(str, sizeof(char) * (count + 1));
			str[count++] = *p;
		}

		p++;
	}

	str = (char *) realloc(str, sizeof(char) * (count + 1));
	str[count] = '\0';

	return str;
}

