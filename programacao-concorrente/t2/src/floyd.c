#include <floyd.h>

/**
 * Algoritmo Floyd-Warshall
 * 
 * Retorna a menor distancia entre duas juncoes mais distantes
 */
int floyd(int **town, int n, int nthreads)
{
	int *maxVector;
	int i, j, k, min = INT_MAX, auxMax = 0;

#ifdef _OPENMP
	if (nthreads > 0) {
		omp_set_num_threads(nthreads);
	}
	#pragma omp parallel for private(k, i, j)
#endif
	for (k = 0; k < n; k++) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (town[i][k] != INT_MAX && town[k][j] != INT_MAX && town[i][j] > town[i][k] + town[k][j]) {
				   town[i][j] = town[i][k] + town[k][j];
				}
			}
		}	
	}

	// Vetor que guarda os maximos para cada juncao
	maxVector = (int *)malloc(sizeof(int) * n);

	if (maxVector == NULL) {
		printf("ERROR: could not allocate memory for Floyd-Warshall algorithm\n");
		return EXIT_FAILURE;
	}

	// Encontra o maximo para cada juncao e menor desses maximos
	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++) {
			if (town[j][i] > auxMax) {
				auxMax = town[j][i];
			}
		}
		maxVector[j] = auxMax;
		if (maxVector[j] < min) {
			min = maxVector[j];
		}
		auxMax = 0;
	}

	free(maxVector);

	return min;
}

