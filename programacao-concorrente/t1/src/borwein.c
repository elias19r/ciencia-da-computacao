/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 1: Calculo do Pi e Black-Scholes
 * Algoritmo Sequencial de Borwein (1984)
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

	mpf_t a0;      // a_{n}
	mpf_t a1;      // a_{n+1}
	mpf_t sqrt_a0; // raiz de a_{n}
	mpf_t b;       // sequencia b_{n}
	mpf_t p;       // sequencia p_{n}, que e' o resultado
	mpf_t tmp;     // auxiliar

	// Valores iniciais
	mpf_init(a1);
	mpf_init(a0); 
	mpf_sqrt_ui(a0, 2); 
	mpf_init(sqrt_a0); 
	mpf_init(b);
	mpf_init(p); 
	mpf_add_ui(p, a0, 2);
	mpf_init(tmp); 

	// Iteracao do algoritmo
	for (i = N; i > 0; i--) {

		// Primeira equacao
		mpf_sqrt(sqrt_a0, a0);
		mpf_set_d(a1, 1.0);
		mpf_div(a1, a1, sqrt_a0);
		mpf_add(a1, a1, sqrt_a0);
		mpf_div_ui(a1, a1, 2);

		// Segunda equacao
		mpf_add_ui(tmp, b, 1);
		mpf_mul(tmp, tmp, sqrt_a0);
		mpf_add(b, a0, b);
		mpf_div(b, tmp, b);

		// Terceira e quarta equacao
		mpf_add_ui(tmp, a1, 1);
		mpf_mul(tmp, tmp, p);
		mpf_mul(tmp, tmp, b);
		mpf_add_ui(p, b, 1);
		mpf_div(p, tmp, p);

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
	mpf_clear(tmp);

	return EXIT_SUCCESS;
}
