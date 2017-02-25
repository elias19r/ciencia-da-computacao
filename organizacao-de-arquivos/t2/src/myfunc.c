#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <myfunc.h>

/**
 * Escaneia o teclado, pega uma palavra e retorna
 */
char *scanw() {
	int count = 0;
	char c;
	char *str = NULL;
	
	while (1) {
		// Pega um caractere
		scanf("%c", &c);
		
		// Confere se é espaço
		if (isspace(c)) {
			// Se já inseriu algo: finaliza a string
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
		if (c == MYFUNC_ENTER) {
			// Se já inseriu algo: finaliza a string
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
		if (c == MYFUNC_ENTER) {
			// Se já inseriu algo: finaliza a string
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
char *fscanw(FILE *fp) {
	int count = 0;
	char c;
	char *str = NULL;
	
	while (1) {
		// Pega um caractere
		fscanf(fp, "%c", &c);
		
		// Confere se é espaço
		if (isspace(c)) {
			// Se já inseriu algo: finaliza a string
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
char *nextw(char *buffer) {
	static char *p;
	int count = 0;
	char *str = NULL;
	
	if (buffer != NULL)
		p = buffer;
	
	while (*p != '\0') {
		// Confere se é espaço
		if (isspace(*p)) {
			// Se já inseriu algo: finaliza a string
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
		if (*p == MYFUNC_ENTER) {
			// Se já inseriu algo: finaliza a string
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
 * Como se fosse o argc e argv do programa padrão
 * Obs: essa função pode retornar NULL
 */
char **getargs(const char *str, int *argc) {
	char *arg = NULL;
	char **argv = NULL;
	int i = 0, j = 0;
	
	*argc = 0;
	
	if (str != NULL) {
		while (str[i] != '\0') {
			// Se não for espaço, pega o caractere
			if (!isspace(str[i])) {
				arg = (char *) realloc(arg, sizeof(char) * (j + 1));
				arg[j++] = str[i];
			} else {
				
				// Se for espaço e já pegou alguma caractere (j > 0)
				// 	então adiciona essa string em argv e prepara
				// 	para pegar a próxima
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
		// Finaliza quando sair do laço e tiver pegado alguma coisa (j > 0)
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
 * Remove os espaços em branco de uma string
 */
void rmspaces(char *str) {
	char *p1 = str, *p2 = str;
	
	while (*p1 != '\0') {
		if (isspace(*p1)) {
			p1++;
		} else {
			*p2++ = *p1++;
		}
	}
	*p2 = '\0';
}

