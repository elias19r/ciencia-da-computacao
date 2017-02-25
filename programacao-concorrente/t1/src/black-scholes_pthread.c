/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 1: Calculo do Pi e Black-Scholes
 * Algoritmo Paralelo de Black-Scholes
 * 
 * SSC0143 Programacao Concorrente
 * 
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * Vinicius Katata Biondo   6783972
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

// Quantidade minima de threads: 2
#define _MIN_THREADS_ 2

// Variaveis globais
pthread_t *threads;
int *indices;

double S;
double E;
double r;
double sigma;
double T;
int M, m;
double *t;
double *sum;
double *sum_sq;

/**
 * Codigo para geracao de numero aleatorio dado
 * na especificacao do trabalho
 */
struct BoxMullerState {
	double x1, x2, w, y1, y2;
	int useLast;
	struct drand48_data random;
};

void initBoxMullerState(struct BoxMullerState *state)
{
	state->random.__init = 0;
	state->useLast = 0;

	struct timeval now;
	gettimeofday(&now, NULL);
	state->random.__x[0] = now.tv_usec;
}

double boxMullerRandom(struct BoxMullerState* state)
{
	double randomNumber;

	if (state->useLast) {
		state->y1 = state->y2;
		state->useLast = 0;
	} else {
		do {
			drand48_r(&state->random, &state->x1);
			state->x1 = 2.0 * state->x1 - 1.0;
			drand48_r(&state->random, &state->x2);
			state->x2 = 2.0 * state->x2 - 1.0;
			state->w = state->x1 * state->x1 + state->x2 * state->x2;
		} while (state->w >= 1.0);

		state->w = sqrt((-2.0 * log(state->w)) / state->w);
		state->y1 = state->x1 * state->w;
		state->y2 = state->x2 * state->w;
		state->useLast = 1;
	}

	return state->y1;
}

/**
 * Algoritmo de Black-Scholes
 */
void *blackScholes(void *arg)
{
	int i, k = *((int *)arg);
	struct BoxMullerState state;

	initBoxMullerState(&state);
	
	// Iteracao do algoritmo
	for (i = 0; i < m; i++) {
		t[k] = S * exp((r - 0.5 * sigma*sigma) * T + sigma * sqrt(T) * boxMullerRandom(&state));
		t[k] -= E;

		if (t[k] > 0.0) {
			t[k] = exp(-r * T) * t[k];
			sum[k] += t[k];           // acumula para calcular a media e desvio padrao
			sum_sq[k] += t[k] * t[k]; // acumula para calcular o desvio padrao
		}
	}

	pthread_exit(NULL);
}


int main(int argc, char **argv)
{
	int i, nthreads = _MIN_THREADS_;
	
	double mean;
	double stddev;
	double confwidth;
	double confmin;
	double confmax;

	double join_sum = 0.0;
	double join_sum_sq = 0.0;

	// Recebe a quantidade de threads
	if (argc > 1) {
		nthreads = atoi(argv[1]);
		if (nthreads < 1)
			nthreads = _MIN_THREADS_;
	}

	// Leitura dos dados de entrada
	scanf("%lf", &S);
	scanf("%lf", &E);
	scanf("%lf", &r);
	scanf("%lf", &sigma);
	scanf("%lf", &T);
	scanf("%d",  &M);
	
	// Calcula quantidade de iteracao por thread
	m = M / nthreads;
	
	threads = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
	indices = (int *)malloc(nthreads * sizeof(int));
	t = (double *)malloc(nthreads * sizeof(double));
	sum = (double *)malloc(nthreads * sizeof(double));
	sum_sq = (double *)malloc(nthreads * sizeof(double));
	
	// Iteracao do algoritmo em todas as threds
	for (i = 0; i < nthreads; i++) {
		indices[i] = i;
		pthread_create(&threads[i], NULL, blackScholes, &indices[i]);
	}

	// Espera todas threads terminarem e soma os valores calculados
	for (i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
		join_sum += sum[i];
		join_sum_sq += sum_sq[i];
	}

	// Calculo dos resultados
	mean = join_sum / (double)M;
	stddev = join_sum_sq + -2.0 * mean * join_sum + M * mean*mean;
	stddev = sqrt(stddev / (double)M);

	confwidth = 1.96 * stddev / sqrt((double)M);
	confmin = mean - confwidth;
	confmax = mean + confwidth;

	// Impressao dos resultados
	printf("%lf\n", S);
	printf("%lf\n", E);
	printf("%lf\n", r);
	printf("%lf\n", sigma);
	printf("%lf\n", T);
	printf("%d\n",  M);

	printf("%lf\n", confmin);
	printf("%lf\n", confmax); 

	free(threads);
	free(indices);
	free(t);
	free(sum);
	free(sum_sq);

	return EXIT_SUCCESS;
}
