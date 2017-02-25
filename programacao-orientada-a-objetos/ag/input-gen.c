#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

	if (argc < 7) {
		printf("Error: incorrect params.\n");
		printf("Usage: times n size crossover mutation maxdistance seed\n");
		exit(0);
	}

	int i, j;
	double d;

	int times = atoi(argv[1]);
	int n = atoi(argv[2]);
	int size = atoi(argv[3]);
	double crossover = atof(argv[4]);
	double mutation = atof(argv[5]);
	int maxdistance = atoi(argv[6]);
	int seed = atoi(argv[7]);
	
	printf("%d\n", times);
	printf("%d\n", n);
	printf("%d\n", size);
	printf("%.2lf\n", crossover);
	printf("%.2lf\n", mutation);

	srand(seed);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			if (i != j)
				d = 0.01 * (double ) (rand() % (maxdistance * 100));
			else
				d = 0.0;

			printf("%d %d %.2lf\n", i, j, d);
		}
	
	return 0;
}
