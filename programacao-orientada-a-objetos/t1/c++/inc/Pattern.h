#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <stdio.h>
#include <regex.h>

#include <String.h>


/**
 * Classe Pattern
 * Confere padrões de entradas por meio de expressões regulares
 */
class Pattern {
	public:
		Pattern() {}
		~Pattern() {}

		/**
		 * Executa uma expressão regular
		 */

		static bool executeRegex(String *str, const char *regex) {
			bool flag = false;
			regex_t myRegex;
			
			regcomp(&myRegex, regex, REG_EXTENDED | REG_NOSUB);

			if (regexec(&myRegex, str->toCharArray(), 0, 0, 0) == 0) {
				flag = true;
			} else {
				flag = false;
			}
			
			regfree(&myRegex);
			return flag;
		}
		
		static bool executeRegex(char *str, const char *regex) {
			bool flag = false;
			regex_t myRegex;
			
			regcomp(&myRegex, regex, REG_EXTENDED | REG_NOSUB);

			if (regexec(&myRegex, str, 0, 0, 0) == 0) {
				flag = true;
			} else {
				flag = false;
			}
			
			regfree(&myRegex);
			return flag;
		}

		/**
		 * Confere se é um número real válido
		 */
		static bool number(String *str) {
			bool flag = false;
			regex_t myRegex;
			
			regcomp(&myRegex, "^[-+]{0,1}[0-9]+\\.{0,1}[0-9]*$", REG_EXTENDED | REG_NOSUB);

			if (regexec(&myRegex, str->toCharArray(), 0, 0, 0) == 0) {
				flag = true;
			} else {
				flag = false;
			}
			
			regfree(&myRegex);
			return flag;
		}

		/**
		 * Confere se é um e-mail válido
		 */
		static bool email(String *str) {
			bool flag = false;
			regex_t myRegex;
			
			regcomp(&myRegex, "^.+@.+$", REG_EXTENDED | REG_NOSUB);

			if (regexec(&myRegex, str->toCharArray(), 0, 0, 0) == 0) {
				flag = true;
			} else {
				flag = false;
			}
			
			regfree(&myRegex);
			return flag;
		}

		/**
		 * Confere se é uma data válida
		 */
		static bool date(String *str) {
			bool flag = false;
			regex_t myRegex;
			
			regcomp(&myRegex, "^(0?[1-9]|[12][0-9]|3[01])/(0?[1-9]|1[012])/([0-9]{4})$", REG_EXTENDED | REG_NOSUB);

			if (regexec(&myRegex, str->toCharArray(), 0, 0, 0) == 0) {
				flag = true;
			} else {
				flag = false;
			}
			
			regfree(&myRegex);
			return flag;
		}

		/**
		 * Confere se é uma senha válida
		 */
		static bool password(String *str, int minLength, int maxLength) {
			bool flag = false;
			regex_t myRegex;
			char regex[21];
			
			sprintf(regex, "^.{%d,%d}$", minLength, maxLength);

			regcomp(&myRegex, regex, REG_EXTENDED | REG_NOSUB);

			if (regexec(&myRegex, str->toCharArray(), 0, 0, 0) == 0) {
				flag = true;
			} else {
				flag = false;
			}
			
			regfree(&myRegex);
			return flag;
		}
};

#endif
