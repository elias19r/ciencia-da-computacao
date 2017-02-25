#include <stdlib.h>
#include <stdio.h>

int exist(int *v, int size, int key) {
	int i;
	
	for (i = 0; i < size; i++)
		if (v[i] == key)
			return 1;

	return 0;
}

int main(int argc, char *argv[]) {
	int i, count, read, *numbers = NULL;
	FILE *fp;

	if (argc < 2) {
		printf("Error: file not found\n");
		exit(0);
	}	
	fp = fopen(argv[1], "r");
	
	count = 0;
	while (!feof(fp)) {
		fscanf(fp, "%d", &read);
		if (!feof(fp)) {
			numbers = (int *) realloc(numbers, sizeof(int) * (count + 1));
			numbers[count++] = read;
		}
	}
	
	for (i = 0; i < 1200; i++) {
		printf("cenario(%d) <= '%s';\n", i, exist(numbers, count, i) ? "1" : "0");
	}
	
	fclose(fp);
	free(numbers);

	return 0;
}
