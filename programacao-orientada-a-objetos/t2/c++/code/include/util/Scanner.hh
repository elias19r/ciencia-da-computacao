#ifndef _SCANNER_HH_
#define _SCANNER_HH_

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "../util/Object.hh"
#include "../type/String.hh"
#include "../type/Integer.hh"

class Scanner : public Object {
	private:
		FILE *stream;
		int intValue;
		double d;
		char c;
		char *str;
	public:
		Scanner();
		Scanner(FILE *stream);
		virtual ~Scanner();
		void set(FILE *stream);
		bool nextChar();
		bool next();
		bool nextInt();
		bool nextDouble();
		char hasNextChar();
		char *hasNext();
		String *hasNextString();
		int hasNextInt();
		Integer *hasNextInteger();
		double hasNextDouble();
};

#endif
