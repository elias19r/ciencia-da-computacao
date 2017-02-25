/**
 * Elias Italiano Rodrigues, 7987251
 *
 * SME0104 Calculo Numerico: Trabalho 1
 * 2014/05/20
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * Calcula e retorna a norma infinita de um vetor v de tamanho n
 */
double norma_infinita(double *v, int n)
{
	int i;
	double abs, max = fabs(v[0]);

	for (i = 1; i < n; i++) {
		abs = fabs(v[i]);
		if (abs > max)
			max = abs;
	}
	return max;
}


/**
 * Resolve o sistema linear do tipo Ax = b de tamanho n x n
 * e precisao epsilon pelo metodo de Gauss-Seidel com no maximo itmax iteracoes
 */
int gauss_seidel(double **A, double *x, double *b, int n, double epsilon, int itmax)
{
	double *x_ = (double *)malloc(sizeof(double) * n); // vetor iteracao (k+1)
	double *r = (double *)malloc(sizeof(double) * n); // vetor residuo x(k+1) - x(k)
	double soma; // valor dos somatorios da formula
	int iteracoes = 0; // quantidade de iteracoes ate convergir
	int i, j; // variaveis de iteracao

	// Retorna valor dos somatorios da formula
	double somatorios()
	{
		soma = 0.0;
		for (j = 0; j < i; j++)
			soma += A[i][j] * x_[j];
		for (j = i + 1; j < n; j++)
			soma += A[i][j] * x[j];
		return soma;
	}

	do {
		// Conta e confere iteracoes
		iteracoes++;
		if (iteracoes > itmax) {
			iteracoes--;
			break;
		}

		// Aplica formula do metodo de Gauss
		for (i = 0; i < n; i++)
			x_[i] = (b[i] - somatorios()) / A[i][i];

		// Calcula vetor residuo r
		for (j = 0; j < n; j++)
			r[j] = x_[j] - x[j];

		// Copia valores de x_ para x para a proxima iteracao
		for (j = 0; j < n; j++)
			x[j] = x_[j];

	} while (norma_infinita(r, n) > epsilon);

	free(x_);
	free(r);
	return iteracoes;
}

/**
 * Programa principal
 */
int main(int argc, char **argv)
{
	int n;           // tamanho n do vetor b e matriz quadrada A
	double **A;      // matriz A
	double *x;       // vetor solucao x
	double *b;       // vetor b
	double epsilon;  // epsilon da precisao para o erro
	int itmax;       // numero maximo de iteracoes
	int iteracoes;   // quantidade de iteracoes para convergir
	int i, j, sinal; // variaveis auxiliares

	printf("SME0104 Calculo Numerico\nTrabalho 1: Metodo de Gauss-Seidel\n\n");

	// Leitura das entradas
	printf("n: "); scanf("%d", &n);
	printf("itmax: "); scanf("%d", &itmax);
	printf("epsilon: "); scanf("%lf", &epsilon);
	printf("\nvetor b (informe os n valores):\n");
	b = (double *)malloc(sizeof(double) * n);
	for (i = 0; i < n; i++) {
		printf("b[%d]: ", i + 1);
		scanf("%lf", b+i);
	}

	// Preparando o vetor x e construindo a matriz A segundo o enunciado do trabalho
	x = (double *)calloc(n, sizeof(double));
	A = (double **)malloc(sizeof(double *) * n);
	for (i = 0; i < n; i++)
		A[i] = (double *)calloc(n, sizeof(double));
	for (i = 0; i < n - 3; i++) {
		A[i][i] = 4.0;
		A[i][i+1] = A[i+1][i] = -1.0;
		A[i][i+3] = A[i+3][i] = -1.0;
	}
	for ( ; i < n - 1; i++) {
		A[i][i] = 4.0;
		A[i][i+1] = A[i+1][i] = -1.0;
	}
	for ( ; i < n; i++)
		A[i][i] = 4.0;

	printf("\nSistema a ser resolvido:\n\n");
	sinal = n / 2;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			printf("  %9lf ", A[i][j]);
		if (i == sinal)
			printf(" |  x[%d]  =  %9lf\n", i + 1, b[i]);
		else
			printf(" |  x[%d]     %9lf\n", i + 1, b[i]);
	}

	printf("\nResolvendo o sistema...\n");
	iteracoes = gauss_seidel(A, x, b, n, epsilon, itmax);

	printf("\nQuantidade de iteracoes: %d\n", iteracoes);

	printf("\nVetor solucao do sistema:\n");
	printf("x = (");
	for (i = 0; i < n; i++)
		printf(" %9lf", x[i]);
	printf("  )\n");

	free(b);
	free(x);
	while (--n >= 0) free(A[n]);
	free(A);

	return EXIT_SUCCESS;
}
