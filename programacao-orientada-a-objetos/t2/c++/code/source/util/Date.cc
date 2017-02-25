#include "../../include/util/Date.hh"

Date::Date() {
	this->year  = 0;
	this->month = 0;
	this->day   = 0;
	this->hour = 0;
	this->min  = 0;
	this->sec  = 0;
	this->str = (char *) calloc(1, sizeof(char));
}

Date::Date(Date *date) {
	this->year  = date->year;
	this->month = date->month;
	this->day   = date->day;
	this->hour  = date->hour;
	this->min   = date->min;
	this->sec   = date->sec;
	this->str   = (char *) calloc(strlen(date->str) + 1, sizeof(char));
	strcpy(this->str, date->str);
}

Date::Date(String *string) {
	this->year  = 0;
	this->month = 0;
	this->day   = 0;
	this->hour = 0;
	this->min  = 0;
	this->sec  = 0;
	this->str = (char *) calloc(1, sizeof(char));
	this->set(string);
}

Date::Date(const char *str) {
	this->year  = 0;
	this->month = 0;
	this->day   = 0;
	this->hour = 0;
	this->min  = 0;
	this->sec  = 0;
	this->str = (char *) calloc(1, sizeof(char));
	this->set(str);
}

Date::Date(int year, int month, int day) {
	this->hour = 0;
	this->min  = 0;
	this->sec  = 0;
	this->str = (char *) calloc(1, sizeof(char));
	this->set(year, month, day);
}

Date::Date(int year, int month, int day, int hour, int min, int sec) {
	this->setYear(year);
	this->setMonth(month);
	this->setDay(day);
	this->setHour(hour);
	this->setMin(min);
	this->setSec(sec);
	this->str = (char *) calloc(1, sizeof(char));
}

Date::~Date() {
	free(this->str);
}

bool Date::set(int year, int month, int day) {
	if (this->setYear(year) && this->setMonth(month) && this->setDay(day))
		return true;

	return false;
}
 
bool Date::set(String *string) {
	bool flag = false;
	Vector<String *> *dateVector = string->split("/");
	Integer *integer = new Integer();

	if (
		this->setYear(integer->parseInt(dateVector->get(2))) &&
		this->setMonth(integer->parseInt(dateVector->get(1))) &&
		this->setDay(integer->parseInt(dateVector->get(0)))
	)
		flag = true;

	for (int i = 0, count = dateVector->size(); i < count; ++i)
		delete dateVector->get(i);

	delete dateVector;
	delete integer;
	
	return flag;
}

bool Date::set(const char *str) {
	char day[3] = {str[0], str[1], '\0'};
	char month[3] = {str[2], str[3], '\0'};
	char year[5] = {str[4], str[5], str[6], str[7], '\0'};
	
	if (this->setYear(atoi(year)) && this->setMonth(atoi(month)) && this->setDay(atoi(day)))
		return true;

	return false;
}

bool Date::setHour(int value) {
	if (value >= 0 && value < 24) {
		this->hour = value;
		return true;
	}
	return false;
}

bool Date::setMin(int value) {
	if (value >= 0 && value < 59) {
		this->min = value;
		return true;
	}
	return false;
}

bool Date::setSec(int value) {
	if (value >= 0 && value < 59) {
		this->sec = value;
		return true;
	}
	return false;
}

bool Date::setYear(int value) {
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

bool Date::setMonth(int value) {
	if (value > 0 && value < 13) {
		this->month = value;
		return true;
	}
	return false;
}

bool Date::setDay(int value) {
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

int Date::getDay() {
	return this->day;
}

int Date::getMonth() {
	return this->month;
}

int Date::getYear() {
	return this->year;
}

int Date::getHour() {
	return this->hour;
}

int Date::getMin() {
	return this->min;
}

int Date::getSec() {
	return this->sec;
}

bool Date::leapYear() {
	if ((!(this->year % 4) && (this->year % 100)) || !(this->year % 400))
		return true;
	else
		return false;
}

void Date::printTime() {
	printf("%2d:%2d:%4d\n", this->hour, this->min, this->sec);
}

bool Date::equals(Date *date) {
	if (date->year == this->year)
		if (date->month == this->month)
			if (date->day == this->day)
				if (date->hour == this->hour)
					if (date->min == this->min)
						if (date->sec == this->sec)
							return true;
		
	return false;
}

int Date::compareTo(Date *date) {
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

char *Date::toCharArray() {
	int at = 0;
	
	free(this->str);
	this->str = (char *) calloc(9, sizeof(char));

	if (this->day < 10) {
		this->str[0] = '0';
		++at;
		sprintf(this->str+at, "%d", this->day);
		++at;
	} else
		at += sprintf(this->str+at, "%d", this->day);

	if (this->month < 10) {
		this->str[at++] = '0';
		sprintf(this->str+at, "%d", this->month);
		++at;
	} else
		at += sprintf(this->str+at, "%d", this->month);

	sprintf(this->str+at, "%d", this->year);
	
	char *ret = (char *) calloc(strlen(this->str) + 1, sizeof(char));
	strcpy(ret, this->str);
	
	return ret;
}

Date *Date::clone() {
	return new Date(this);
}

