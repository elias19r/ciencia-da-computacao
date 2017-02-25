#define USERS_DATA         "c++/file/usuarios.dat"
#define USER_RECORD_SIZE   338
#define USER_EMAIL_SIZE    80
#define USER_PASSWORD_SIZE 30
#define USER_FULLNAME_SIZE 80
#define USER_NICKNAME_SIZE 30
#define USER_CITY_SIZE     30
#define USER_STATE_SIZE    30
#define USER_COUNTRY_SIZE  50
#define USER_BIRTHDAY_SIZE 8

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	char *email = (char *) calloc(USER_EMAIL_SIZE+1, sizeof(char));
	char *password = (char *) calloc(USER_PASSWORD_SIZE+1, sizeof(char));
	char *fullName = (char *) calloc(USER_FULLNAME_SIZE+1, sizeof(char));
	char *nickName = (char *) calloc(USER_NICKNAME_SIZE+1, sizeof(char));
	char *city = (char *) calloc(USER_CITY_SIZE+1, sizeof(char));
	char *state = (char *) calloc(USER_STATE_SIZE+1, sizeof(char));
	char *country = (char *) calloc(USER_COUNTRY_SIZE+1, sizeof(char));
	char *birthday = (char *) calloc(USER_BIRTHDAY_SIZE+1, sizeof(char));
	
	FILE *fpDAT = fopen(USERS_DATA, "rb");
	printf("-----------------------\n");
	while (!feof(fpDAT)) {

		fread(email, USER_EMAIL_SIZE * sizeof(char), 1, fpDAT);
		email[USER_EMAIL_SIZE] = '\0';

		fread(password, USER_PASSWORD_SIZE * sizeof(char), 1, fpDAT);
		password[USER_PASSWORD_SIZE] = '\0';

		fread(fullName, USER_FULLNAME_SIZE * sizeof(char), 1, fpDAT);
		fullName[USER_FULLNAME_SIZE] = '\0';

		fread(nickName, USER_NICKNAME_SIZE * sizeof(char), 1, fpDAT);
		nickName[USER_NICKNAME_SIZE] = '\0';

		fread(city, USER_CITY_SIZE * sizeof(char), 1, fpDAT);
		city[USER_CITY_SIZE] = '\0';

		fread(state, USER_STATE_SIZE * sizeof(char), 1, fpDAT);
		state[USER_STATE_SIZE] = '\0';

		fread(country, USER_COUNTRY_SIZE * sizeof(char), 1, fpDAT);
		country[USER_COUNTRY_SIZE] = '\0';

		fread(birthday, USER_BIRTHDAY_SIZE * sizeof(char), 1, fpDAT);
		birthday[USER_BIRTHDAY_SIZE] = '\0';
		
		if (!feof(fpDAT)) {
			printf("%s\n", email);
			printf("%s\n", password);
			printf("%s\n", fullName);
			printf("%s\n", nickName);
			printf("%s\n", city);
			printf("%s\n", state);
			printf("%s\n", country);
			printf("%s\n", birthday);
			printf("-----------------------\n");
		}
	}

	free(email);
	free(password);
	free(fullName);
	free(nickName);
	free(city);
	free(state);
	free(country);
	free(birthday);			

	return 0;
}
