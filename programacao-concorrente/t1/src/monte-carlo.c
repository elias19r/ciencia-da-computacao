/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 1: Calculo do Pi e Black-Scholes
 * Algoritmo Sequencial de Monte Carlo
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

// Quantidade padrao minima de iteracoes: 10^9
#define _MIN_ITERACAO_ 1000000000

/**
 * Codigo para geracao de numero aleatorio dado
 */
void initBoxMullerState(struct drand48_data *random)
{
	struct timeval now;

	random->__init = 0;
	gettimeofday(&now, NULL);
	random->__x[0] = now.tv_usec;
}

double boxMullerRandom(struct drand48_data *random)
{
	double randomNumber;
	drand48_r(random, &randomNumber);

	return randomNumber;
}

int main(int argc, char **argv)
{
	double randomx, randomy; // valores de um ponto (x, y) no plano
	unsigned long long i, N = _MIN_ITERACAO_; // long long pois o numero de iteracoes pode ser tao grande quanto se queira
	unsigned long long circleArea = 0;
	struct drand48_data random;

	// Recebe a quantidade de iteracoes a serem calculadas
	if (argc > 1) {
		N = atoi(argv[1]);
		if (N < 0)
			N = _MIN_ITERACAO_;
	}

	initBoxMullerState(&random);

	// Iteracao do algoritmo
	for (i = N; i > 0; i--) {
		randomx = boxMullerRandom(&random);
		randomy = boxMullerRandom(&random);

		if ((randomx*randomx + randomy*randomx) <= 1.0) {
			circleArea++;
		}
	}

	// Resultado
	printf("%.8lf\n", 4.0 * ((double)circleArea / (double)N));	

	return EXIT_SUCCESS;
}

