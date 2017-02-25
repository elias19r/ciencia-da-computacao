/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 3: Smooth
 * Programa auxiliar para calcular as estatisticas
 * 
 * SSC0143 Programacao Concorrente
 * 
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * Vinicius Katata Biondo   6783972
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * Calcula e imprime estatisticas sobre um conjunto de dados
 */
void stats(double *data, int times)
{
	int i;
	double sum = 0.0, sum_sq = 0.0;
	double mean, stddev, confwidth, confmin, confmax;

	for (i = 0; i < times; i++) {
		sum += data[i]; // acumula para calcular a media e desvio padrao
		sum_sq += data[i] * data[i]; // acumula para calcular o desvio padrao
	}

	// Calculo da media e desvio padrao
	mean = sum / (double)times;
	stddev = sqrt((sum_sq + (double)times*mean*mean -2.0*mean*sum) / (double)times);

	// Calculo do intervalo de confianca
	confwidth = 1.96 * stddev / sqrt((double)times);
	confmin = mean - confwidth;
	confmax = mean + confwidth;
	
	// Impressao dos resultados
//	printf("Times   : %lf\n", times);
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
	double *data = NULL;
	
	// Le quantidade de vezes
	scanf("%d", &times);
	
	// Aloca memoria
	data = (double *)malloc(sizeof(double) * times);
	if (data == NULL) {
		printf("ERROR: could not allocate memory for stats\n");
		exit(EXIT_FAILURE);
	}
	
	// Le entradas dos tempos de execucao
	for (i = 0; i < times; i++) {
		scanf("%lf", data+i);
	}
	
	// Calcula e imprime estatisticas
	stats(data, times);
	
	// Libera memoria
	free(data);
	
	return EXIT_SUCCESS;
}
