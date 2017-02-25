#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <String.h>
#include <Integer.h>
#include <Double.h>

/**
 * Classe Scanner
 * Faz a leitura de dados
 */
class Scanner {
	private:
		FILE *stream; // stream, por padrão é stdin

		// Valores que podem ser lidos
		int intValue;
		double d;
		char c;
		char *str;

	public:
		/**
		 * Construtor
		 */
		Scanner(FILE *stream = stdin) {
			this->set(stream);
			
			this->intValue = 0;
			this->d = 0.0;
			this->c = '\0';
			this->str = NULL;
		}

		/**
		 * Destrutor
		 */
		~Scanner() { }
		
		/**
		 * Define o stream
		 */
		void set(FILE *stream) {
			this->stream = stream;
		}

		/**
		 * Métodos HASNEXT que leem o conteúdo de stream
		 */

		bool hasNextChar() {
			fscanf(this->stream, "%c", &(this->c));
			return true;
		}

		bool hasNext() {
			int count = 0;
			char c;
			bool quote = false;
			
			while (true) {
				fscanf(this->stream, "%c", &c);
				
				if (c == '"') {
					quote = !quote;
					if (!quote) {
						this->str = (char *) realloc(this->str, sizeof(char) * (count + 1));
						this->str[count] = '\0';
						break;
					}
				} else {
					if (isspace(c) && !quote) {
						if (count > 0) {
							this->str = (char *) realloc(this->str, sizeof(char) * (count + 1));
							this->str[count] = '\0';
							break;
						}
					} else {
						this->str = (char *) realloc(this->str, sizeof(char) * (count + 1));
						this->str[count++] = c;
					}
				}
			}
			return true;
		}

		bool hasNextInt() {
			fscanf(this->stream, "%d", &(this->intValue));
			return true;
		}		

		bool hasNextDouble() {
			fscanf(this->stream, "%lf", &(this->d));
			return true;
		}

		
		/**
		 * Métodos NEXT que retornam o conteúdo lido
		 */

		char nextChar() {
			char ret = this->c;
			this->c = '\0';
			return ret;
		}

		char *next() {
			char *ret = this->str;
			this->str = NULL;
			return ret;
		}

		String *nextString() {
			String *ret = new String(this->str);
			free(this->str);
			this->str = NULL;
			return ret;
		}		

		int nextInt() {
			int ret = this->intValue;
			this->intValue = 0;
			return ret;
		}

		Integer *nextInteger() {
			Integer *ret = new Integer(this->intValue);
			this->intValue = 0;
			return ret;
		}

		double nextDouble() {
			return this->d;
		}
	
		Double *nextDoubleObj() {
			Double *ret = new Double(this->d);
			this->d = 0.0;
			return ret;
		}
};

#endif
