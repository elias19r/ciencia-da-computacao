#ifndef _INTEGER_H_
#define _INTEGER_H_

#include <String.h>

/**
 * Classe Integer
 * Armazena a manipula um número inteiro
 */
class Integer {
	private:
		int num;
	
	public:
		/**
		 * Construtores
		 */
		 
		Integer() {
			this->num = 0;
		}

		Integer(int value) {
			this->num = value;
		}

		/**
		 * Destrutor
		 */
		~Integer() {}
		
		void set(int value) {
			this->num = value;
		}

		/**
		 * Retorna o valor na variável primitiva
		 */		
		int intValue() {
			return this->num;
		}
		
		/**
		 * Incrementa o inteiro
		 */		
		void inc() {
			this->num++;
		}

		/**
		 * Decrementa o inteiro
		 */		
		void dec() {
			this->num--;
		}
		
		/**
		 * Converte um número inteiro para uma String representativa do número na base binária
		 */
		static String *toBinaryString(int num) {
			char *str = (char *) calloc(33, sizeof(char));
			char digit[1];
			
			while (num > 0) {
				sprintf(digit, "%1d", num % 2);
				strcat(str, digit);
				num /= 2;
			}		
			String *ret = new String(str);
			free(str);
			ret->reverse();
			return ret;			
		}

		/**
		 * Converte um número inteiro para uma String representativa do número na base Hexadecimal
		 */
		static String *toHexString(int num) {
			char *str = (char *) calloc(33, sizeof(char));
			
			sprintf(str, "%x", num);

			String *ret = new String(str);
			free(str);
			return ret;	
		}

		/**
		 * Converte um número inteiro para uma String representativa do número na base octal
		 */
		static String *toOctalString(int num) {
			char *str = (char *) calloc(33, sizeof(char));
			
			sprintf(str, "%o", num);

			String *ret = new String(str);
			free(str);
			return ret;	
		}

		/**
		 * Converte coverte uma string para um inteiro primitivo
		 */		
		
		static int parseInt(String *str) {
			return atoi(str->toCharArray());
		}

		static int parseInt(char *str) {
			return atoi(str);
		}
};

#endif
