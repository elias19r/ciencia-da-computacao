#ifndef _DATE_H_
#define _DATE_H_

#include <ctime>

#include <Integer.h>
#include <String.h>

/**
 * Classe Date
 * Armazena e manipula informações de uma data
 */
class Date {
	private:
		int year;
		int month;
		int day;
		int hour;
		int min;
		int sec;
		
		char *str;
	
	public:	
		/**
		 * Construtores
		 */
		 
		Date() {
			this->year  = 0;
			this->month = 0;
			this->day   = 0;
			this->hour = 0;
			this->min  = 0;
			this->sec  = 0;
			this->str = NULL;
		}
		
		Date(String *string) {
			this->year  = 0;
			this->month = 0;
			this->day   = 0;
			this->hour = 0;
			this->min  = 0;
			this->sec  = 0;
			this->str = NULL;
			this->set(string);
		}
		
		Date(const char *str) {
			this->year  = 0;
			this->month = 0;
			this->day   = 0;
			this->hour = 0;
			this->min  = 0;
			this->sec  = 0;
			this->str = NULL;
			this->set(str);
		}

		Date(int year, int month, int day) {
			this->hour = 0;
			this->min  = 0;
			this->sec  = 0;
			this->str = NULL;
			this->set(year, month, day);
		}

		Date(int year, int month, int day, int hour, int min, int sec) {
			this->setYear(year);
			this->setMonth(month);
			this->setDay(day);
			this->setHour(hour);
			this->setMin(min);
			this->setSec(sec);
			this->str = NULL;
		}
		
		// Destrutor
		~Date() {
			if (this->str != NULL) free(this->str);
		}
		
		/**
		 * Métodos SETTERS públicos
		 */
		 
		 bool set(int year, int month, int day) {
			if (this->setYear(year) && this->setMonth(month) && this->setDay(day))
				return true;

			return false;
		 }
		 
		bool set(String *string) {
			bool flag = false;
			Vector<String *> *dateVector = string->split("/");

			if (
				this->setYear(Integer::parseInt(dateVector->elementAt(2))) &&
				this->setMonth(Integer::parseInt(dateVector->elementAt(1))) &&
				this->setDay(Integer::parseInt(dateVector->elementAt(0)))
			)
				flag = true;

			for (int i = 0, size = dateVector->size(); i < size; i++) {
				delete dateVector->elementAt(i);
			}
			delete dateVector;
			
			return flag;
		}

		// Define a data a partir de um char* no formato "diamesano". Exemplo: 15032013
		bool set(const char *str) {
			char day[3] = {str[0], str[1], '\0'};
			char month[3] = {str[2], str[3], '\0'};
			char year[5] = {str[4], str[5], str[6], str[7], '\0'};
			
			if (this->setYear(atoi(year)) && this->setMonth(atoi(month)) && this->setDay(atoi(day)))
				return true;

			return false;
		}
		
		bool setHour(int value) {
			if (value >= 0 && value < 24) {
				this->hour = value;
			} else
				return false;
		}

		bool setMin(int value) {
			if (value >= 0 && value < 59) {
				this->min = value;
				return true;
			} else
				return false;
		}
		
		bool setSec(int value) {
			if (value >= 0 && value < 59) {
				this->sec = value;
				return true;
			} else
				return false;
		}
		
	private:

		/**
		 * Métodos SETTERS privados
		 */

		bool setYear(int value) {
			time_t rawTime;
			struct tm *timeInfo = NULL;
			bool flag = false;
			
			time(&rawTime);
			timeInfo = localtime(&rawTime);

			if (value >= 1900 && value <= timeInfo->tm_year + 1900) {
				this->year = value;
				flag = true;
			}

			return flag;
		}
		
		bool setMonth(int value) {
			if (value > 0 && value < 13) {
				this->month = value;
				return true;
			} else
				return false;
		}

		bool setDay(int value) {
			if (this->month > 0) {	
				if (value > 0 && value < 32) {
					if (this->month == 2) {
						if (this->leapYear()) {
							if (value < 30) {
								this->day = value;
								return true;
							} else
								return false;
						} else {
							if (value < 29) {
								this->day = value;
								return true;
							} else
								return false;
						}
					} else {
						if (this->month == 4 || this->month == 6 || this->month == 9 || this->month == 11) {
							if (value < 31) {
								this->day = value;
								return true;
							} else
								return false;
						} else {
							this->day = value;
							return true;
						}
					}
				} else
					return false;
			} else
				return false;
		}
		
	public:

		/**
		 * Métodos GETTERS
		 */

		int getDay()   { return this->day;   }
		int getMonth() { return this->month; }
		int getYear()  { return this->year;  }
		int getHour()  { return this->hour;  }
		int getMin()   { return this->min;   }
		int getSec()   { return this->sec;   }

		/**
		 * Confere se um ano é bissexto
		 */
		bool leapYear() {
			if ((!(this->year % 4) && (this->year % 100)) || !(this->year % 400))
				return true;
			else
				return false;
		}

		void printTime() {
			printf("%2d:%2d:%4d\n", this->hour, this->min, this->sec);
		}

		/**
		 * Confere se esta data é igual a uma outra
		 */		
		bool equals(Date *date) {
			if (date->year == this->year)
				if (date->month == this->month)
					if (date->day == this->day)
						if (date->hour == this->hour)
							if (date->min == this->min)
								if (date->sec == this->sec)
									return true;
				
			return false;
		}
		
		/**
		 * Faz comparação entre esta data e uma outra
		 */
		int compareTo(Date *date) {
			if (!this->equals(date)) {
				if (this->year  > date->year)  return 1; else return -1;
				if (this->month > date->month) return 1; else return -1;
				if (this->day   > date->day)   return 1; else return -1;
				if (this->hour  > date->hour)  return 1; else return -1;
				if (this->min   > date->min)   return 1; else return -1;
				if (this->sec   > date->sec)   return 1; else return -1;				
			} else
				return 0;
		}
		
		/**
		 * Define um char* no formato "diamesano". Por exemplo: "15032013"
		 * Isso facilita para salvar a data no arquivo .dat
		 */
		char *toCharArray() {
			int at = 0;
			
			if (this->str != NULL) free(this->str);
			this->str = (char *) calloc(9, sizeof(char));

			if (this->day < 10) {
				this->str[0] = '0';
				at++;
				sprintf(this->str+at, "%d", this->day);
				at++;
			} else {
				at += sprintf(this->str+at, "%d", this->day);
			}
			if (this->month < 10) {
				this->str[at] = '0';
				at++;
				sprintf(this->str+at, "%d", this->month);
				at++;
			} else {
				at += sprintf(this->str+at, "%d", this->month);
			}
			sprintf(this->str+at, "%d", this->year);
			return this->str;
		}
};

#endif	
