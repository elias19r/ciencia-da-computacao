#include "../../include/type/String.hh"

String::String() {
	this->initialize();
}

String::String(const char *str) {
	this->initialize();
	this->set(str);
}

String::String(String *string) {
	this->initialize();
	this->set(string);
}

String::~String() {
	free(this->str);
}

void String::initialize() {
	this->str = (char *) calloc(1, sizeof(char));
	this->len = 0;
}

char String::charAt(int index) {
	if (index > -1 && index < this->len)
		return this->str[index];

	return 0;
}

int String::compareTo(const char *str) {
	return strcmp(this->str, str);
}

int String::compareTo(String *string) {
	return strcmp(this->str, string->str);
}

void String::concat(const char *str) {
	this->len += strlen(str);
	this->str = (char *) realloc(this->str, sizeof(char) * (this->len + 1));
	strcat(this->str, str);
}

void String::concat(String *string) {
	this->len += string->len;
	this->str = (char *) realloc(this->str, sizeof(char) * (this->len + 1));
	strcat(this->str, string->str);
}

void String::set(const char *str) {
	this->len = strlen(str);
	this->str = (char *) realloc(this->str, sizeof(char) * (this->len + 1));
	strcpy(this->str, str);
}		

void String::set(String *string) {
	this->len = string->len;
	this->str = (char *) realloc(this->str, sizeof(char) * (this->len + 1));
	strcpy(this->str, string->str);
}

char *String::toCharArray() {
	char *ret = (char *) calloc(this->len + 1, sizeof(char));
	strcpy(ret, this->str);
	return ret;
}

bool String::equals(const char *str) {
	if (strcmp(this->str, str) == 0)
		return true;

	return false;
}

bool String::equals(String *string) {
	if (strcmp(this->str, string->str) == 0)
		return true;

	return false;	
}		

int String::length() {
	return this->len;
}

void String::toLowerCase() {
	int i = 0;
	while (this->str[i] != '\0') {
		this->str[i] = tolower(this->str[i]);
		++i;
	}
}

void String::toUpperCase() {
	int i = 0;
	while (this->str[i] != '\0') {
		this->str[i] = toupper(this->str[i]);
		++i;
	}
}

void String::reverse() {
	int i, j;
	char *str = (char *) calloc(this->len + 1, sizeof(char));
	
	for (i = 0, j = this->len-1; i < this->len; ++i, --j)
		str[j] = this->str[i];

	str[i] = '\0';
	free(this->str);
	this->str = str;
}

Vector<String *> *String::split(const char *separator) {
	char *str = (char *) calloc(this->len + 1, sizeof(char));
	strcpy(str, this->str);

	Vector<String *> *v = new Vector<String *>();
	
	char *p = strtok(str, separator);
	while (p != NULL) {
		String *tok = new String(p);
		v->add(tok);
		p = strtok(NULL, separator);
	}
	
	free(str);
	if (v->size() == 0) {
		delete v;
		return NULL;
	} else
		return v;
}

String *String::clone() {
	return new String(this->str);
}

char *String::parseStr(int num, int size) {
	char *str = (char *) calloc(size + 1, sizeof(char));
	char format[31];
	sprintf(format, "%%%d", size);
	sprintf(str, format, num);
	return str;
}

void String::replace(String *oldString, String *newString) {
	// TODO
}

void String::startsWith(String *string) {
	// TODO
}

void String::substring() {
	// TODO
}

