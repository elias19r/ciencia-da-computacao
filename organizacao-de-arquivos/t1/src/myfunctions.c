#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Inclui header
#include <myfunctions.h>

/**
 * Escaneia o teclado e pega uma string (caractere por caractere)
 * Usado no lugar de scanf, gets, fgets pra não ter problemas de leitura
 */
char *scan_str() {
	int count = 0;
	char c;
	char *str = NULL;
	
	while (1) {
		// Pega um caractere
		scanf("%c", &c);
		
		// Confere se teclou ENTER
		if (c == ENTER) {
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
 * Remove os espaços em branco de uma string
 */
void remove_spaces(char *str) {
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
