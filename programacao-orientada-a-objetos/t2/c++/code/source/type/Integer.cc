#include "../../include/type/Integer.hh"

Integer::Integer() {
	this->value = 0;
}

Integer::Integer(int value) {
	this->set(value);
}

Integer::~Integer() {
	
}

void Integer::set(int value) {
	this->value = value;
}

int Integer::intValue() {
	return this->value;
}

void Integer::inc() {
	++this->value;
}

void Integer::dec() {
	--this->value;
}

String *Integer::toBinaryString(int num) {
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

String *Integer::toHexString(int num) {
	char *str = (char *) calloc(33, sizeof(char));
	
	sprintf(str, "%x", num);

	String *ret = new String(str);
	free(str);
	return ret;
}

String *Integer::toOctalString(int num) {
	char *str = (char *) calloc(33, sizeof(char));
	
	sprintf(str, "%o", num);

	String *ret = new String(str);
	free(str);
	return ret;	
}

int Integer::parseInt(String *string) {
	char *buffer = string->toCharArray();
	int ret = atoi(buffer);
	free(buffer);
	return ret;
}

int Integer::parseInt(char *str) {
	return atoi(str);
}

