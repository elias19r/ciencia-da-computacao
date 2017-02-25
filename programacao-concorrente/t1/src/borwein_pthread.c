/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 1: Calculo do Pi e Black-Scholes
 * Algoritmo Paralelo de Borwein (1984)
 * 
 * SSC0143 Programacao Concorrente
 * 
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * Vinicius Katata Biondo   6783972
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <gmp.h>

// Checa se houve erro em uma thread
#define pthread_fatal(func, ret) \
	do { if (ret != 0) { \
		printf("ERROR %d on pthread_%s()\n", ret, func); \
		return EXIT_FAILURE; \
	}} while(0)

// Variaveis definidas globais
mpf_t a0;      // a_{n}
mpf_t a1;      // a_{n+1}
mpf_t sqrt_a0; // raiz de a_{n}
mpf_t b;       // b_{n} e b_{n+1}
mpf_t p;       // p_{n} e p_{n+1}
mpf_t tmp1;    // auxiliar
mpf_t tmp2;    // auxiliar
mpf_t tmp3;    // auxiliar

/**
 * Calcula (a_{n} + b_{n})
 */
void *T1(void *arg)
{
	mpf_add(tmp3, a0, b);
	pthread_exit(NULL);
}

/**
 * Calcula (1 + b_{n}))
 */
void *T2(void *arg)
{
	mpf_add_ui(tmp2, b, 1);
	pthread_exit(NULL);
}


/**
 * Calcula sqrt(a_{n})
 */
void *T3(void *arg)
{
	mpf_sqrt(sqrt_a0, a0);
	pthread_exit(NULL);
}


/**
 * Calcula (a_{n} + 1)
 */
void *T4(void *arg)
{
	mpf_add_ui(a1, a0, 1);
	pthread_exit(NULL);
}

/**
 * Calcula (2 * sqrt(a_{n}))
 */
void *T5(void *arg)
{
	mpf_mul_ui(tmp1, sqrt_a0, 2);
	pthread_exit(NULL);
}

/**
 * Calcula (1 + b_{n}) * sqrt(a_{n})
 */
void *T6(void *arg)
{
	mpf_mul(tmp2, tmp2, sqrt_a0);
	pthread_exit(NULL);
}

/**
 * Calcula a_{n+1}
 */
void *T7(void *arg)
{
	mpf_div(a1, a1, tmp1);
	pthread_exit(NULL);
}

/**
 * Calcula b_{n+1}
 */
void *T8(void *arg)
{
	mpf_div(b, tmp2, tmp3);
	pthread_exit(NULL);
}

/**
 * Calcula (p_{n} * b_{n+1})
 */
void *T9(void *arg)
{
	mpf_mul(tmp2, p, b);
	pthread_exit(NULL);
}

/**
 * Calcula (1 + a_{n+1})
 */
void *T10(void *arg)
{
	mpf_add_ui(tmp1, a1, 1);
	pthread_exit(NULL);
}

/**
 * Calcula (1 + b_{n+1})
 */
void *T11(void *arg)
{
	mpf_add_ui(tmp3, b, 1);
	pthread_exit(NULL);
}

/**
 * Calcula (1 + a_{n+1}) * p_{n} * b_{n+1}
 */
void *T12(void *arg)
{
	mpf_mul(tmp2, tmp1, tmp2);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int ret, i, N = 1, digitos = 6, precisao;
	char format[50], *buffer;

	// Threads a serem usadas
	pthread_t P1;
	pthread_t P2;
	pthread_t P3;
	pthread_t P4;
	
	// Recebe a quantidade de digitos decimais a serem calculados
	if (argc > 1) {
		digitos = atoi(argv[1]);
		if (digitos < 0)
			digitos = 6;
	}

	// Determina a precisao necessaria
	i = digitos;
	while (i >>= 1)
		N++;
	precisao = N * digitos;

	// Define formato da string que imprime o resultado
	sprintf(format, "%%.%dFf", digitos + 1);

	// Define precisao para os big numbers
	mpf_set_default_prec(precisao);

	// Valores iniciais
	mpf_init(a1);
	mpf_init(a0); 
	mpf_sqrt_ui(a0, 2); 
	mpf_init(sqrt_a0); 
	mpf_init(b);
	mpf_init(p); 
	mpf_add_ui(p, a0, 2);
	mpf_init(tmp1);
	mpf_init(tmp2);
	mpf_init(tmp3);

	// Iteracao do algoritmo
	for (i = N; i > 0; i--) {

		ret = pthread_create(&P1, NULL, T3, NULL);
		pthread_fatal("create", ret);

		ret = pthread_create(&P2, NULL, T2, NULL);
		pthread_fatal("create", ret);

		ret = pthread_create(&P3, NULL, T1, NULL);
		pthread_fatal("create", ret);

		ret = pthread_create(&P4, NULL, T4, NULL);
		pthread_fatal("create", ret);

			ret = pthread_join(P1, NULL);
			pthread_fatal("join", ret);
 
		ret = pthread_create(&P1, NULL, T5, NULL);
		pthread_fatal("create", ret);

			ret = pthread_join(P2, NULL);
			pthread_fatal("join", ret);

		ret = pthread_create(&P2, NULL, T6, NULL);
		pthread_fatal("create", ret);

			ret = pthread_join(P4, NULL);
			pthread_fatal("join", ret);

			ret = pthread_join(P1, NULL);
			pthread_fatal("join", ret);

		ret = pthread_create(&P1, NULL, T7, NULL);
		pthread_fatal("create", ret);

			ret = pthread_join(P3, NULL);
			pthread_fatal("join", ret);

			ret = pthread_join(P2, NULL);
			pthread_fatal("join", ret);

		ret = pthread_create(&P2, NULL, T8, NULL);
		pthread_fatal("create", ret);

			ret = pthread_join(P1, NULL);
			pthread_fatal("join", ret);

			ret = pthread_join(P2, NULL);
			pthread_fatal("join", ret);

		ret = pthread_create(&P1, NULL, T10, NULL);
		pthread_fatal("create", ret);

		ret = pthread_create(&P2, NULL, T9, NULL);
		pthread_fatal("create", ret);

		ret = pthread_create(&P3, NULL, T11, NULL);
		pthread_fatal("create", ret);

			ret = pthread_join(P1, NULL);
			pthread_fatal("join", ret);

			ret = pthread_join(P2, NULL);
			pthread_fatal("join", ret);

		ret = pthread_create(&P1, NULL, T12, NULL);
		pthread_fatal("create", ret);

			ret = pthread_join(P3, NULL);
			pthread_fatal("join", ret);

			ret = pthread_join(P1, NULL);
			pthread_fatal("join", ret);

		// T13: Calcula p_{n+1} 
		mpf_div(p, tmp2, tmp3);

		mpf_set(a0, a1); // atualiza valores da sequencia a_{n} para a proxima iteracao
	}
	
	// Resultado extenso (nao arredondado)
	buffer = (char *)malloc(digitos + 4);
	gmp_sprintf(buffer, format, p);
	buffer[digitos + 2] = '\0';
	printf("%s\n", buffer);
	free(buffer);

	// Liberacao de memoria
	mpf_clear(a0);
	mpf_clear(a1);
	mpf_clear(sqrt_a0);
	mpf_clear(b);
	mpf_clear(p);
	mpf_clear(tmp1);
	mpf_clear(tmp2);
	mpf_clear(tmp3);

	return EXIT_SUCCESS;
}
