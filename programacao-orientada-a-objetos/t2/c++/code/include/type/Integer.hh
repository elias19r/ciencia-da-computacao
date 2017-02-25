#ifndef _INTEGER_HH_
#define _INTEGER_HH_

#include "../util/Object.hh"
#include "../type/String.hh"

class Integer : public Object {
	private:
		int value;
	public:
		Integer();
		Integer(int value);
		virtual ~Integer();
		void set(int value);
		int intValue();
		void inc();
		void dec();
		String *toBinaryString(int num);
		String *toHexString(int num);
		String *toOctalString(int num);
		int parseInt(String *string);
		int parseInt(char *str);
};

#endif
