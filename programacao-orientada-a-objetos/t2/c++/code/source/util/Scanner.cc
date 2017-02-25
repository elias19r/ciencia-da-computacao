#include "../../include/util/Scanner.hh"

Scanner::Scanner() {
	this->set(stdin);
	this->intValue = 0;
	this->d = 0.0;
	this->c = '\0';
	this->str = NULL;
}

Scanner::Scanner(FILE *stream) {
	this->set(stream);
	this->intValue = 0;
	this->d = 0.0;
	this->c = '\0';
	this->str = NULL;
}

Scanner::~Scanner() {
	if (this->str != NULL)
		free(this->str);
}

void Scanner::set(FILE *stream) {
	this->stream = stream;
}

bool Scanner::nextChar() {
	fscanf(this->stream, "%c", &(this->c));
	return true;
}

bool Scanner::next() {
	int count = 0;
	char c;
	bool quote = false;
	
	while (true) {
		fscanf(this->stream, "%c", &c);
		
		if (c == '"') {
			quote = !quote;
			if (!quote) {
				this->str = (char *) realloc(this->str, sizeof(char) * (count + 1));
				this->str[count] = '\0';
				break;
			}
		} else {
			if (isspace(c) && !quote) {
				if (count > 0) {
					this->str = (char *) realloc(this->str, sizeof(char) * (count + 1));
					this->str[count] = '\0';
					break;
				}
			} else {
				this->str = (char *) realloc(this->str, sizeof(char) * (count + 1));
				this->str[count++] = c;
			}
		}
	}

	return true;
}

bool Scanner::nextInt() {
	fscanf(this->stream, "%d", &(this->intValue));
	return true;
}		

bool Scanner::nextDouble() {
	fscanf(this->stream, "%lf", &(this->d));
	return true;
}


char Scanner::hasNextChar() {
	char ret = this->c;
	this->c = '\0';
	return ret;
}

char *Scanner::hasNext() {
	char *ret = this->str;
	this->str = NULL;
	return ret;
}

String *Scanner::hasNextString() {
	String *ret = new String(this->str);
	free(this->str);
	this->str = NULL;
	return ret;
}		

int Scanner::hasNextInt() {
	int ret = this->intValue;
	this->intValue = 0;
	return ret;
}

Integer *Scanner::hasNextInteger() {
	Integer *ret = new Integer(this->intValue);
	this->intValue = 0;
	return ret;
}

double Scanner::hasNextDouble() {
	return this->d;
}

