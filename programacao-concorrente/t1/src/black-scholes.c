/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 1: Calculo do Pi e Black-Scholes
 * Algoritmo Sequencial de Black-Scholes
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

int main(int argc, char **argv)
{
	int i;

	double S;
	double E;
	double r;
	double sigma;
	double T;
	int M;
	
	double t;         // var temporaria
	double sum = 0.0; // auxilia stddev e mean
	double sum_sq = 0.0; // auxilia stddev
	double mean;
	double stddev = 0.0;
	double confwidth;
	double confmin;
	double confmax;

	struct BoxMullerState state;

	// Leitura dos dados de entrada
	scanf("%lf", &S);
	scanf("%lf", &E);
	scanf("%lf", &r);
	scanf("%lf", &sigma);
	scanf("%lf", &T);
	scanf("%d",  &M);

	initBoxMullerState(&state);

	// Iteracao do algoritmo
	for (i = 0; i < M; i++) {
		t = S * exp((r - 0.5 * sigma*sigma) * T + sigma * sqrt(T) * boxMullerRandom(&state));
		t -= E;

		if (t > 0.0) {
			t = exp(-r * T) * t;
			sum += t;        // acumula para calcular a media e desvio padrao
			sum_sq += t * t; // acumula para calcular o desvio padrao
		}
	}

	// Calculo dos resultados
	mean = sum / (double)M;
	stddev = sum_sq + -2.0 * mean * sum + M * mean*mean;
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

	return EXIT_SUCCESS;
}

