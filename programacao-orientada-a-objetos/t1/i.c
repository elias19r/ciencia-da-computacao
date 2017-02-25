#define USERS_INDEX     "c++/file/usuarios.idx"
#define USER_EMAIL_SIZE 80

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	char *email = (char *) calloc(USER_EMAIL_SIZE+1, sizeof(char));
	int offset;
	
	FILE *fpDAT = fopen(USERS_INDEX, "rb");
	printf("-----------------------\n");
	while (!feof(fpDAT)) {

		fread(email, USER_EMAIL_SIZE * sizeof(char), 1, fpDAT);
		email[USER_EMAIL_SIZE] = '\0';

		fread(&offset, sizeof(int), 1, fpDAT);

		if (!feof(fpDAT)) {
			printf("%s\n", email);
			printf("%d\n", offset);
			printf("-----------------------\n");
		}
	}
	free(email);			

	return 0;
}
