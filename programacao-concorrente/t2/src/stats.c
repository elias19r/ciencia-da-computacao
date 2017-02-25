#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * Calcula e imprime estatisticas sobre um conjunto de resultados
 */
void stats(double *result, int times)
{
	int k;
	double sum = 0.0, sum_sq = 0.0;
	double mean, stddev, confwidth, confmin, confmax;

	for (k = 0; k < times; k++) {
		sum += result[k]; // acumula para calcular a media e desvio padrao
		sum_sq += result[k] * result[k]; // acumula para calcular o desvio padrao
	}

	// Calculo da media e desvio padrao
	mean = sum / (double)times;
	stddev = sum_sq + -2.0 * mean * sum + (double)times * mean*mean;
	stddev = sqrt(stddev / (double)times);

	// Calculo do intervalo de confianca
	confwidth = 1.96 * stddev / sqrt((double)times);
	confmin = mean - confwidth;
	confmax = mean + confwidth;
	
	// Impressao dos resultados
	printf("Mean    : %lf\n", mean);
	printf("Stddev  : %lf\n", stddev);
	printf("ConfInt : [%lf, %lf]\n", confmin, confmax);
}

/**
 * Programa principal
 */
int main(int argc, char **argv)
{
	int i, times;
	double *result = NULL;
	
	// Le quantidade de vezes
	scanf("%d", &times);
	
	// Aloca memoria
	result = (double *)malloc(sizeof(double) * times);
	if (result == NULL) {
		printf("ERROR: could not allocate memory\n");
		return EXIT_FAILURE;
	}
	
	// Le entradas dos tempos de execucao
	for (i = 0; i < times; i++) {
		scanf("%lf", result+i);
	}
	
	// Calcula e imprime estatisticas
	stats(result, times);
	
	// Libera memoria
	free(result);
	
	return EXIT_SUCCESS;
}
