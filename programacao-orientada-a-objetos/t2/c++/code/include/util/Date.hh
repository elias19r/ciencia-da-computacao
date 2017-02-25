#ifndef _DATE_HH_
#define _DATE_HH_

#include <ctime>
#include "../util/Object.hh"
#include "../type/Integer.hh"
#include "../type/String.hh"

class Date : public Object {
	private:
		int year;
		int month;
		int day;
		int hour;
		int min;
		int sec;
		char *str;

		bool setYear(int value);
		bool setMonth(int value);
		bool setDay(int value);
	public:	
		Date();
		Date(Date *date);
		Date(String *string);
		Date(const char *str);
		Date(int year, int month, int day);
		Date(int year, int month, int day, int hour, int min, int sec);
		virtual ~Date();
		bool set(int year, int month, int day);
		bool set(String *string);
		bool set(const char *str);
		bool setHour(int value);
		bool setMin(int value);
		bool setSec(int value);
		int getDay();
		int getMonth();
		int getYear();
		int getHour();
		int getMin();
		int getSec();
		bool leapYear();
		void printTime();
		bool equals(Date *date);
		int compareTo(Date *date);
		char *toCharArray();
		Date *clone();
};

#endif
