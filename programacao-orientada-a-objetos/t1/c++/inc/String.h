#ifndef _STRING_H_
#define _STRING_H_

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <Vector.h>

/**
 * Classe String
 * Armazeda e manipula uma string
 */
class String {
	private:
		char *str; // o ponteiro de char
	
	public:
		/**
		 * Construtores
		 */

		String() {
			this->str = NULL;
		}

		String(const char *str) {
			this->str = NULL;
			this->set(str);
		}

		String(String *string) {
			this->str = NULL;
			this->set(string);
		}

		/**
		 * Destrutor
		 */
		~String() {
			if (this->str != NULL) free(this->str);
		}
		
		/**
		 * Retorna o charactere da posição index da string
		 */
		char charAt(int index) {
			return this->str[index];
		}
		
		/**
		 * Compara esta string a uma outra
		 */

		int compareTo(const char *str) {
			return strcmp(this->str, str);
		}

		int compareTo(String *string) {
			return strcmp(this->str, string->toCharArray());
		}

		/**
		 * Concatena esta string com uma outra
		 */

		void concat(char *str) {
			this->str = (char *) realloc(this->str, sizeof(char) * (this->length() + strlen(str) + 1));
			strcat(this->str, str);
		}

		void concat(String *string) {
			this->str = (char *) realloc(this->str, sizeof(char) * (this->length() + string->length() + 1));
			strcat(this->str, string->toCharArray());
		}

		/**
		 * Métodos SETTERs que definem a string
		 */

		void set(const char *str) {
			this->str = (char *) realloc(this->str, sizeof(char) * (strlen(str) + 1));
			strcpy(this->str, str);
		}		

		void set(String *string) {
			this->str = (char *) realloc(this->str, sizeof(char) * (string->length() + 1));
			strcpy(this->str, string->toCharArray());
		}
		
		/**
		 * Retorna o ponteiro de char desta string
		 */
		char *toCharArray() {
			return this->str;
		}
		
		/**
		 * Compara se esta string é igual a outra
		 */

		bool equals(String *string) {
			if (strcmp(this->str, string->toCharArray()) == 0)
				return true;
			else
				return false;
		}		

		bool equals(const char *str) {
			if (strcmp(this->str, str) == 0)
				return true;
			else
				return false;
		}
		
		/**
		 * Retorna o tamanho desta string
		 */
		int length() {
			return strlen(this->str);
		}
		
		/**
		 * Transforma esta string para lower-case
		 */
		void toLowerCase() {
			if (this->str != NULL) {
				int i = 0;
				while (this->str[i] != '\0') {
					this->str[i] = tolower(this->str[i]);
					i++;
				}
			}
		}

		/**
		 * Transforma esta string para upper-case
		 */		
		void toUpperCase() {
			if (this->str != NULL) {
				int i = 0;
				while (this->str[i] != '\0') {
					this->str[i] = toupper(this->str[i]);
					i++;
				}
			}
		}
		
		/**
		 * Inverte os caracteres desta string
		 */
		void reverse() {
			int i, j, thisLength = this->length();
			char *str = (char *) calloc(thisLength + 1, sizeof(char));
			
			for (i = 0, j = thisLength-1; i < thisLength; i++, j--) {
				str[j] = this->str[i];
			}
			str[i] = '\0';
			free(this->str);
			this->str = str;
		}
		
		/**
		 * Quebra esta string segundo um determinado 'token' e retorna em um vector de strings
		 */
		Vector<String *> *split(const char *separator) {
			char *str = (char *) calloc(this->length() + 1, sizeof(char));
			strcpy(str, this->toCharArray());

			Vector<String *> *v = new Vector<String *>();
			
			char *p = strtok(str, separator);
			while (p != NULL) {
				String *tok = new String(p);
				v->add(tok);
				p = strtok(NULL, separator);
			}
			
			free(str);
			if (v->size() == 0) {
				delete v;
				return NULL;
			} else
				return v;
		}

		/**
		 * Retorna um clone deste objeto
		 */
		String *clone() {
			return new String(this);
		}
		
		/**
		 * Faz parsing de um número inteiro e retorna um ponteiro de char
		 */
		static char *parseStr(int num, int size) {
			char *str = (char *) calloc(size + 1, sizeof(char));
			char format[31];
			sprintf(format, "%%%d", size);
			sprintf(str, format, num);
			return str;
		}

		// ---------- A FAZER ---------- //
		
		void replace(String *oldString, String *newString) {
		}
		
		void startsWith(String *string) {
		}
		
		void substring() {
		}
};

#endif
