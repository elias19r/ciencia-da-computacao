#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
	int i, type;

	if (argc > 1) {

		type  = atoi(argv[1]);
		switch (type) {
			// nao temporal e nao espacial
			case 0:
				for (i = 0; i < 2978; i++) {
					printf("2 %x\n", (i * 11));
				}
				break;

			// espacial
			case 1:
				for (i = 0; i < 32768; i++) {
					printf("2 %x\n", i);
				}
				break;

			// temporal
			case 2: {
				int *address = (int *)malloc(sizeof(int) * 96);
				address[0] = 0;
				for (i = 1; i < 96; i++) {
					address[i] = address[i-1] + 11;
				}
				for (i = 0; i < 32768; i++) {
					printf("2 %x\n", address[i % 96]);
				}
				free(address);
				break;
			}
			// espacial e temporal
			case 3:
				for (i = 0; i < 32768; i++) {
					printf("2 %x\n", i % 320);
				}
				break;

			// aleatorio
			case 4:
				srand(time(NULL));
				for (i = 0; i < 32768; i++) {
					printf("2 %x\n", rand() % 192);
				}
				break;

			default:
				break;
		}

	} else {
		printf("como usar: trace_gen <type>\n\
\n\
<type>    eh o tipo de principios de localidade\n\
          0 nao temporal e nao espacial\n\
          1 espacial\n\
          2 temporal\n\
          3 temporal e espacial\n\
          4 aleatorio\n");
	}

	return EXIT_SUCCESS;
}