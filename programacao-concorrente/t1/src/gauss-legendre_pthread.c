/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 1: Calculo do Pi e Black-Scholes
 * Algoritmo Paralelo de Gauss-Legendre
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
mpf_t a0;   // a_{n}	
mpf_t a1;   // a_{n+1}
mpf_t b0;   // b_{n}
mpf_t b1;   // b_{n+1}
mpf_t t0;   // t_{n}
mpf_t t1;   // t_{n+1}
mpf_t p0;   // p_{n}
mpf_t p1;   // p_{n+1}
mpf_t tmp1; // auxiliar
mpf_t tmp2; // auxiliar
mpf_t tmp3; // auxiliar

/**
 * Calcula a_{n+1} = ((a_{n} + b_{n}) / 2)
 */
void *T1(void *arg)
{
	mpf_add(a1, a0, b0);
	mpf_div_ui(a1, a1, 2);
	pthread_exit(NULL);
}

/*
 * Calcula p_{n+1} = 2 * p_{n}
 */
void *T2(void *arg)
{
	mpf_mul_ui(p1, p0, 2);
	pthread_exit(NULL);
}

/**
 * Calcula b_{n+1} = sqrt{a_{n} * b_{n}}
 */
void *T3(void *arg)
{
	mpf_mul(b1, a0, b0);
	mpf_sqrt(b1, b1);
	pthread_exit(NULL);
}

/**
 * Calcula t_{n} - p_{n} * a_{n}^2 
 */
void *T4(void *arg)
{
	mpf_pow_ui(tmp1, a0, 2);
	mpf_mul(tmp1, tmp1, p0);
	mpf_sub(tmp1, t0, tmp1);
	pthread_exit(NULL);
}

/**
 * Calcula p_{n} * 2 * a_{n}
 */
void *T5(void *arg)
{
	mpf_mul_ui(tmp2, p0, 2);
	mpf_mul(tmp2, tmp2, a0);
	pthread_exit(NULL);
}

/**
 * Calcula p_{n} * a_{n+1}^2
 */
void *T6(void *arg)
{
	mpf_pow_ui(tmp3, a1, 2);
	mpf_mul(tmp3, tmp3, p0);
	pthread_exit(NULL);
}

/**
 * Calcula p_{n} * 2 * a_{n} * a_{n+1}
 */
void *T7(void *arg)
{
	mpf_mul(tmp2, tmp2, a1);
	pthread_exit(NULL);
}

/**
 * Calcula t_{n+1} = t_{n} - p_{n} * a_{n}^2 + p_{n} * 2 * a_{n} * a_{n+1} - p_{n} * a_{n+1}^2
 */
void *T8(void *arg)
{
	mpf_add(t1, tmp1, tmp2);
	mpf_sub(t1, t1, tmp3);
	pthread_exit(NULL);
}

/**
 * Calcula a_{n+1}^2
 */
void *T9(void *arg)
{
	mpf_pow_ui(tmp1, a1, 2);
	pthread_exit(NULL);
}

/**
 * Calcula 2 * a_{n+1} * b_{n+1}
 */
void *T10(void *arg)
{
	mpf_mul_ui(tmp2, a1, 2);
	mpf_mul(tmp2, tmp2, b1);
	pthread_exit(NULL);
}

/**
 * Calcula b_{n+1}^2
 */
void *T11(void *arg)
{
	mpf_pow_ui(tmp3, b1, 2);
	pthread_exit(NULL);
}

/**
 * Calcula 4 * t_{n+1}
 */
void *T12(void *arg)
{
	mpf_mul_ui(t1, t1, 4);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int i, ret, N = 1, digitos = 6, precisao;
	char format[50], *buffer;

	// Threads a serem usadas
	pthread_t P1;
	pthread_t P2;
	pthread_t P3;
	pthread_t P4;
	pthread_t P5;
	
	//Recebe a quantidade de digitos decimais a serem calculados
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

	mpf_init(tmp1);
	mpf_sqrt_ui(tmp1, 2);
	mpf_init(tmp2);
	mpf_init(tmp3);

	mpf_init_set_ui(a0, 1);
	mpf_init(a1);

	mpf_init_set_ui(b0, 1);
	mpf_div(b0, b0, tmp1);
	mpf_init(b1);

	mpf_init_set_d(t0, 0.25);
	mpf_init(t1);

	mpf_init_set_ui(p0, 1);
	mpf_init(p1);

	// Interacao do algoritmo
	for (i = N; i > 0; i--) {
		
		ret = pthread_create(&P1, NULL, T1, NULL);
		pthread_fatal("create", ret);
		
		ret = pthread_create(&P2, NULL, T2, NULL);
		pthread_fatal("create", ret);

		ret = pthread_create(&P3, NULL, T3, NULL);
		pthread_fatal("create", ret);

		ret = pthread_create(&P4, NULL, T4, NULL);
		pthread_fatal("create", ret);

		ret = pthread_create(&P5, NULL, T5, NULL);
		pthread_fatal("create", ret);

			ret = pthread_join(P1, NULL);	
			pthread_fatal("join", ret);

		ret = pthread_create(&P1, NULL, T6, NULL);	
		pthread_fatal("create", ret);

			ret = pthread_join(P5, NULL);	
			pthread_fatal("join", ret);

		ret = pthread_create(&P5, NULL, T7, NULL);	
		pthread_fatal("create", ret);

			ret = pthread_join(P1, NULL);	
			pthread_fatal("join", ret);

			ret = pthread_join(P4, NULL);	
			pthread_fatal("join", ret);

			ret = pthread_join(P5, NULL);	
			pthread_fatal("join", ret);
		
		ret = pthread_create(&P1, NULL, T8, NULL);	
		pthread_fatal("create", ret);
		
			ret = pthread_join(P1, NULL);	
			pthread_fatal("join", ret);

			ret = pthread_join(P2, NULL);	
			pthread_fatal("join", ret);

			ret = pthread_join(P3, NULL);	
			pthread_fatal("join", ret);
	
		// Atualiza valores da sequencia
		mpf_set(a0, a1);
		mpf_set(b0, b1);
		mpf_set(t0, t1);
		mpf_set(p0, p1);
	}
		
	ret = pthread_create(&P1, NULL, T9, NULL);
	pthread_fatal("create", ret);

	ret = pthread_create(&P2, NULL, T10, NULL);
	pthread_fatal("create", ret);

	ret = pthread_create(&P3, NULL, T11, NULL);
	pthread_fatal("create", ret);

	ret = pthread_create(&P4, NULL, T12, NULL);
	pthread_fatal("create", ret);

		ret = pthread_join(P1, NULL);	
		pthread_fatal("join", ret);

		ret = pthread_join(P2, NULL);	
		pthread_fatal("join", ret);

		ret = pthread_join(P3, NULL);	
		pthread_fatal("join", ret);

		ret = pthread_join(P4, NULL);
		pthread_fatal("join", ret);

	// T13: Calcula pi
	mpf_add(p1, tmp1, tmp2);
	mpf_add(p1, p1, tmp3);
	mpf_div(p1, p1, t1);

	// Resultado extenso (nao arredondado)
	buffer = (char *)malloc(digitos + 4);
	gmp_sprintf(buffer, format, p1);
	buffer[digitos + 2] = '\0';
	printf("%s\n", buffer);
	free(buffer);

	// Liberacao de memoria	
	mpf_clear(a0);
	mpf_clear(a1);
	mpf_clear(b0);
	mpf_clear(b1);
	mpf_clear(t0);
	mpf_clear(t1);
	mpf_clear(p0);
	mpf_clear(p1);
	mpf_clear(tmp1); 
	mpf_clear(tmp2); 
	mpf_clear(tmp3); 

	return EXIT_SUCCESS;
}
