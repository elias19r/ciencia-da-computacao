#include "../../include/util/Pattern.hh"

Pattern::Pattern() {
	
}

Pattern::~Pattern() {
	
}

bool Pattern::executeRegex(String *string, const char *regex) {
	char *buffer = string->toCharArray();
	bool flag = false;
	regex_t myRegex;
	
	regcomp(&myRegex, regex, REG_EXTENDED | REG_NOSUB);

	if (regexec(&myRegex, buffer, 0, 0, 0) == 0)
		flag = true;
	
	regfree(&myRegex);
	free(buffer);
	return flag;
}

bool Pattern::executeRegex(const char *str, const char *regex) {
	bool flag = false;
	regex_t myRegex;
	
	regcomp(&myRegex, regex, REG_EXTENDED | REG_NOSUB);

	if (regexec(&myRegex, str, 0, 0, 0) == 0)
		flag = true;
	
	regfree(&myRegex);
	return flag;
}

bool Pattern::number(String *string) {
	char *buffer = string->toCharArray();
	bool flag = false;
	regex_t myRegex;
	
	regcomp(&myRegex, "^[-+]{0,1}[0-9]+\\.{0,1}[0-9]*$", REG_EXTENDED | REG_NOSUB);

	if (regexec(&myRegex, buffer, 0, 0, 0) == 0)
		flag = true;
	
	regfree(&myRegex);
	free(buffer);
	return flag;
}

bool Pattern::email(String *string) {
	char *buffer = string->toCharArray();
	bool flag = false;
	regex_t myRegex;
	
	regcomp(&myRegex, "^.+@.+$", REG_EXTENDED | REG_NOSUB);

	if (regexec(&myRegex, buffer, 0, 0, 0) == 0)
		flag = true;
	
	regfree(&myRegex);
	free(buffer);
	return flag;
}

bool Pattern::date(String *string) {
	char *buffer = string->toCharArray();
	bool flag = false;
	regex_t myRegex;
	
	regcomp(&myRegex, "^(0?[1-9]|[12][0-9]|3[01])/(0?[1-9]|1[012])/([0-9]{4})$", REG_EXTENDED | REG_NOSUB);

	if (regexec(&myRegex, buffer, 0, 0, 0) == 0)
		flag = true;
	
	regfree(&myRegex);
	free(buffer);
	return flag;
}

bool Pattern::password(String *string, int minLength, int maxLength) {
	char *buffer = string->toCharArray();
	bool flag = false;
	regex_t myRegex;
	char regex[21];
	
	sprintf(regex, "^.{%d,%d}$", minLength, maxLength);

	regcomp(&myRegex, regex, REG_EXTENDED | REG_NOSUB);

	if (regexec(&myRegex, buffer, 0, 0, 0) == 0)
		flag = true;

	regfree(&myRegex);
	free(buffer);
	return flag;
}

