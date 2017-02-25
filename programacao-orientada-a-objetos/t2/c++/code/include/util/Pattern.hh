#ifndef _PATTERN_HH_
#define _PATTERN_HH_

#include <stdio.h>
#include <regex.h>
#include "../util/Object.hh"
#include "../type/String.hh"

class Pattern : public Object {
	public:
		Pattern();
		virtual ~Pattern();
		bool executeRegex(String *string, const char *regex);
		bool executeRegex(const char *str, const char *regex);
		bool number(String *string);
		bool email(String *string);
		bool date(String *string);
		bool password(String *string, int minLength, int maxLength);
};

#endif
