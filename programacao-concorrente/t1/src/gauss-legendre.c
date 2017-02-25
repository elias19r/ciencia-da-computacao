/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 1: Calculo do Pi e Black-Scholes
 * Algoritmo Sequencial de Gauss-Legendre
 * 
 * SSC0143 Programacao Concorrente
 * 
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * Vinicius Katata Biondo   6783972
 */

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

int main(int argc, char **argv)
{
	int i, N = 1, digitos = 6, precisao;
	char format[50], *buffer;

	// Recebe a quantidade de digitos a serem calculados
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

	mpf_t a0;  // a_{n}
	mpf_t a1;  // a_{n+1}
	mpf_t b;   // b_{n} e b_{n+1}
	mpf_t t;   // t_{n} e t_{n+1}
	mpf_t p;   // p_{n} e p_{n+1}
	mpf_t tmp; // auxiliar

	// Valores iniciais
	mpf_init(a1);
	mpf_init(tmp);
	mpf_sqrt_ui(tmp, 2);
	mpf_init_set_d(b,  1.0);
	mpf_div(b, b, tmp);
	mpf_init_set_d(a0, 1.0); 
	mpf_init_set_d(t,  0.25);
	mpf_init_set_d(p,  1.0);

	// Iteracao do algoritmo
	for (i = N; i > 0; i--) {

		// Primeira equacao
		mpf_add(a1, a0, b);
		mpf_div_ui(a1, a1, 2);

		// Segunda equacao
		mpf_mul(b, a0, b);
		mpf_sqrt(b, b);

		// Terceira equacao
		mpf_sub(tmp, a0, a1);
		mpf_pow_ui(tmp, tmp, 2);
		mpf_mul(tmp, tmp, p);
		mpf_sub(t, t, tmp);

		// Quarta equacao
		mpf_mul_ui(p, p, 2);

		mpf_set(a0, a1); // atualiza valores da sequencia a_{n} para a proxima iteracao
	}

	// Cacula pi
	mpf_add(tmp, a1, b);
	mpf_pow_ui(tmp, tmp, 2);
	mpf_div_ui(tmp, tmp, 4);
	mpf_div(tmp, tmp, t);

	// Resultado extenso (nao arredondado)
	buffer = (char *)malloc(digitos + 4);
	gmp_sprintf(buffer, format, tmp);
	buffer[digitos + 2] = '\0';
	printf("%s\n", buffer);
	free(buffer);

	// Liberacao de memoria
	mpf_clear(a0);
	mpf_clear(a1);
	mpf_clear(b);
	mpf_clear(t);
	mpf_clear(p);
	mpf_clear(tmp);

	return EXIT_SUCCESS;
}

