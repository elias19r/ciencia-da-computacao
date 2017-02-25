#define RELATIONSHIPS_DATA               "c++/file/relacionamentos.dat"
#define RELATIONSHIP_RECORD_SIZE 164
#define USER_EMAIL_SIZE          80

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	char *userEmail = (char *) calloc(USER_EMAIL_SIZE+1, sizeof(char));
	char *contactEmail = (char *) calloc(USER_EMAIL_SIZE+1, sizeof(char));
	int contactOffset;
	
	FILE *fpDAT = fopen(RELATIONSHIPS_DATA, "rb");
	printf("-----------------------\n");
	while (!feof(fpDAT)) {

		fread(userEmail, USER_EMAIL_SIZE * sizeof(char), 1, fpDAT);
		userEmail[USER_EMAIL_SIZE] = '\0';

		fread(contactEmail, USER_EMAIL_SIZE * sizeof(char), 1, fpDAT);
		contactEmail[USER_EMAIL_SIZE] = '\0';

		fread(&contactOffset, sizeof(int), 1, fpDAT);

		if (!feof(fpDAT)) {
			printf("%s\n", userEmail);
			printf("%s\n", contactEmail);
			printf("%d\n", contactOffset);
			printf("-----------------------\n");
		}
	}

	free(userEmail);			
	free(contactEmail);

	return 0;
}
