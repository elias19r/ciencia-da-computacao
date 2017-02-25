/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 1: Calculo do Pi e Black-Scholes
 * Algoritmo Paralelo de Monte Carlo
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
#include <pthread.h>

// Quantidade padrao minima de iteracoes: 10^9
#define _MIN_ITERACAO_ 1000000000

// Quantidade minima de threads: 2
#define _MIN_THREADS_ 2

// Variaveis globais
pthread_t *threads;
int *indices;
unsigned long long *sums;
unsigned long long n;

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

/**
 * Faz a simulacao sorteando pares (x, y) aleatorios
 * e conferindo se estao dentro do setor
 */
void *func(void *arg)
{
	double randomx, randomy; // valores de um ponto (x, y) no plano
	unsigned long long i;
	int k = *((int *)arg);
	struct drand48_data random;

	initBoxMullerState(&random);

	for (i = n; i > 0; i--) {
		randomx = boxMullerRandom(&random);
		randomy = boxMullerRandom(&random);

		if ((randomx*randomx + randomy*randomy) <= 1.0) {
			sums[k]++;
		}
	}

	pthread_exit(NULL);
}


int main(int argc, char **argv)
{
	int i, nthreads = _MIN_THREADS_;
	unsigned long long N = _MIN_ITERACAO_;
	unsigned long long circleArea = 0;

	// Recebe a quantidade de iteracoes
	if (argc > 1) {
		N = atoi(argv[1]);
		if (N < 0)
			N = _MIN_ITERACAO_;
	}

	// Recebe a quantidade de threads
	if (argc > 2) {
		nthreads = atoi(argv[2]);
		if (nthreads < 1)
			nthreads = _MIN_THREADS_;
	}

	// Calcula quantidade de iteracao por thread
	n = N / nthreads;

	// Iteracao do algoritmo em todas as threads
	threads = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
	indices = (int *)malloc(nthreads * sizeof(int));
	sums = (unsigned long long *)malloc(nthreads * sizeof(unsigned long long));

	for (i = 0; i < nthreads; i++) {
		indices[i] = i;
		sums[i] = 0;
		pthread_create(&threads[i], NULL, func, &indices[i]);
	}

	// Espera todas threads terminarem e soma os valores
	for (i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
		circleArea += sums[i];
	}

	free(threads);
	free(indices);
	free(sums);

	// Resultado
	printf("%.8lf\n", 4.0 * ((double)circleArea / (double)N));

	return EXIT_SUCCESS;
}
