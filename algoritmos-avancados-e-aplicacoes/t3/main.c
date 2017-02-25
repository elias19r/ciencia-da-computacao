#include <stdlib.h>
#include <stdio.h>

// Tamanho das matrizes quadradas para o Fibonacci
#define SIZE 2

/**
 * Multiplica matrizes quadradas A x B e retorna resultado em uma nova matriz R
 * Aplica modulo nos valores calculados de acordo com o valor da variavel mod
 */
unsigned long long int **matrix_mul(unsigned long long int **A, unsigned long long int **B, unsigned long long int mod) {
	int i, j, k;
	unsigned long long int **R;

	R = (unsigned long long int **) malloc(sizeof(unsigned long long int *) * SIZE);
	for (i = 0; i < SIZE; i++)
		R[i] = (unsigned long long int *) calloc(SIZE, sizeof(unsigned long long int));

	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			for (k = 0; k < SIZE; k++) {
				R[i][j] += (A[i][k] % mod) * (B[k][j] % mod);
				R[i][j] %= mod;
			}

	return R;
}

/**
 * Calcula recursivamente a n-esima potencia de uma matriz quadrada M e retorna
 * o resultado em uma nova matriz R
 *
 * Aplica modulo nos valores calculados de acordo com o valor da variavel mod
 */
unsigned long long int **matrix_pow(unsigned long long int **M, unsigned long long int n, unsigned long long int mod) {
	unsigned long long int **R = NULL;

	// Caso base, apenas faz uma copia da matriz M e a retorna
	if (n == 1) {
		int i, j;

		R = (unsigned long long int **) malloc(sizeof(unsigned long long int *) * SIZE);
		for (i = 0; i < SIZE; i++) {
			R[i] = (unsigned long long int *) malloc(sizeof(unsigned long long int) * SIZE);
			for (j = 0; j < SIZE; j++)
				R[i][j] = M[i][j];
		}

		return R;

	// Se n par
	} else if (n % 2 == 0) {
		unsigned long long int **tmp;
		int i;

		tmp = matrix_pow(M, n/2, mod);
		R = matrix_mul(tmp, tmp, mod);

		for (i = 0; i < SIZE; i++)
			free(tmp[i]);
		free(tmp);

		return R;

	// Se n impar
	} else {
		unsigned long long int **tmp1, **tmp2;
		int i;

		tmp1 = matrix_pow(M, n/2, mod);
		tmp2 = matrix_mul(tmp1, tmp1, mod);
		R = matrix_mul(tmp2, M, mod);

		for (i = 0; i < SIZE; i++) {
			free(tmp1[i]);
			free(tmp2[i]);
		}
		free(tmp1);
		free(tmp2);

		return R;
	}

	return R;
}

/**
 * Retorna os ultimos digitos do n-esimo termo da sequencia de Fibonacci
 * de acordo com o valor da variavel mod
 */
unsigned long long int F(unsigned long long int n, unsigned long long int mod) {
	unsigned long long int **A, **tmp, ret = 0;
	int i;

	if (n == 0 || n == 1)
		return 1 % mod;

	// Aloca e define a matriz dos coeficientes da base
	A = (unsigned long long int **) malloc(sizeof(unsigned long long int *) * SIZE);
	for (i = 0; i < SIZE; i++)
		A[i] = (unsigned long long int *) malloc(sizeof(unsigned long long int) * SIZE);
	A[0][0] = 1;
	A[0][1] = 1;
	A[1][0] = 1;
	A[1][1] = 0;

	// Aplica formula fechada utilizando potencia e multiplicacao de matrizes
	tmp = matrix_pow(A, n-1, mod);
	for (i = 0; i < SIZE; i++) {
		ret += tmp[0][i] % mod;
		ret %= mod;
	}

	// Desalocando memoria
	for (i = 0; i < SIZE; i++) {
		free(tmp[i]);
		free(A[i]);
	}
	free(tmp);
	free(A);

	return ret;
}

/**
 * Funcao principal
 */
int main(int argc, char *argv[]) {
	unsigned long long int n;

	// Recebe valor 0 <= n <= 10^11
	scanf("%llu", &n);

	// Imprime os ultimos 6 digitos do n-esimo termo da sequencia de Fibonacci
	printf("%llu\n", F(n, 1000000));

	return 0;
}

