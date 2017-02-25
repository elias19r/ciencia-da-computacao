#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../../include/lib/mylib.h"

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
		
		// Confere se eh espaço
		if (isspace(c)) {
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
 * Escaneia um arquivo, pega uma palavra e retorna
 */
char *fscan(FILE *fp) {
	int count = 0;
	char c;
	char *str = NULL;
	
	while (1) {
		// Pega um caractere
		fscanf(fp, "%c", &c);
		
		// Confere se eh espaço
		if (isspace(c)) {
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
 * Escaneia um arquivo, pega uma linha e retorna
 */
char *fscanln(FILE *fp) {
	int count = 0;
	char c;
	char *str = NULL;
	
	while (1) {
		// Pega um caractere
		fscanf(fp, "%c", &c);
		
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
		// Confere se eh espaço
		if (isspace(*p)) {
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

/**
 * Escaneia uma string, pega uma linha e retorna
 */
char *nextln(char *buffer) {
	static char *p;
	int count = 0;
	char *str = NULL;
	
	if (buffer != NULL)
		p = buffer;
	
	while (*p != '\0') {
		// Confere se teclou ENTER
		if (*p == _MYLIB_ENTER_) {
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

/**
 * Recebe uma string e separada suas palavras, retornando um vetor de strings
 * e a quantidade de palavras encontradas
 * Como se fosse o argc e argv do programa padrao
 * Obs: essa funcao pode retornar NULL
 */
char **getargs(const char *str, int *argc) {
	char *arg = NULL;
	char **argv = NULL;
	int i = 0, j = 0;
	
	*argc = 0;
	
	if (str != NULL) {
		while (str[i] != '\0') {
			// Se nao for espaco, pega o caractere
			if (!isspace(str[i])) {
				arg = (char *) realloc(arg, sizeof(char) * (j + 1));
				arg[j++] = str[i];
			} else {
				
				// Se for espaço e ja pegou alguma caractere (j > 0)
				// 	entao adiciona essa string em argv e prepara
				// 	para pegar a proxima
				if (j > 0) {
					arg = (char *) realloc(arg, sizeof(char) * (j + 1));
					arg[j] = '\0';
					argv = (char **) realloc(argv, sizeof(char *) * (*argc + 1));
					argv[(*argc)++] = arg;
					arg = NULL;
					j = 0;
				}
			}
			i++;
		}
		// Finaliza quando sair do laco e tiver pegado alguma coisa (j > 0)
		if (j > 0) {
			arg = (char *) realloc(arg, sizeof(char) * (j + 1));
			arg[j] = '\0';
			argv = (char **) realloc(argv, sizeof(char *) * (*argc + 1));
			argv[(*argc)++] = arg;
		}
	}
	
	return argv;
}

/**
 * Remove os espacos em branco de uma string
 */
void rmspaces(char *str) {
	char *p1 = str, *p2 = str;
	
	while (*p1 != '\0') {
		if (isspace(*p1))
			p1++;
		else
			*p2++ = *p1++;
	}
	*p2 = '\0';
}

