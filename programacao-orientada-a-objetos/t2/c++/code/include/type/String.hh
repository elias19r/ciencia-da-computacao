#ifndef _STRING_HH_
#define _STRING_HH_
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../util/Object.hh"
#include "../util/Vector.hh"

class String : public Object {
	private:
		char *str;
		int len;
	public:
		String();
		String(const char *str);
		String(String *string);
		virtual ~String();
		void initialize();
		char charAt(int index);
		int compareTo(const char *str);
		int compareTo(String *string);
		void concat(const char *str);
		void concat(String *string);
		void set(const char *str);
		void set(String *string);
		char *toCharArray();
		bool equals(const char *str);
		bool equals(String *string);
		int length();
		void toLowerCase();
		void toUpperCase();
		void reverse();
		Vector<String *> *split(const char *separator);
		String *clone();
		char *parseStr(int num, int size);
		void replace(String *oldString, String *newString);
		void startsWith(String *string);
		void substring();
};

#endif
