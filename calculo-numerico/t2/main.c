/**
 * Elias Italiano Rodrigues, 7987251
 *
 * SME0104 Calculo Numerico: Trabalho 2
 * 2014/06/24
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Vetores do PVI
double   y[2] = {1.0, 0.0}; // solucao
double   F[2] = {0.0, 0.0}; // F
double  _F[2] = {0.0, 0.0}; // F calculado com y barra
double  _y[2] = {0.0, 0.0}; // y barra

// Erros
double erros[4][3] = {
// resultado, soma do numerador, soma do denominador
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0}
};

/**
 * Resposta da solucao de Euler Modificado para um dado x
 * e considerando intervalo de tamanho h
 */
double euler_mod(double x, double h)
{
	// Passo 1
	F[0] = y[1];
	F[1] = y[0] + exp(x - h);
	
	// Passo 2
	_y[0] = y[0] + h * F[0];
	_y[1] = y[1] + h * F[1];
	
	// Passo 3
	_F[0] = _y[1];
	_F[1] = _y[0] + exp(x);
	
	// Passo 4
	y[0] = y[0] + (h * (F[0] + _F[0])) / 2.0;
	y[1] = y[1] + (h * (F[1] + _F[1])) / 2.0;
	
	return y[0];
}

/**
 * Resposta da solucao analitica para um dado x
 */
double analitica(double x)
{
	return 0.25 * (exp(x) * (1.0 + 2.0 * x) + 3.0 * exp(-x));
}

/**
 * Efetua todos os calculos iterativos chamando as devidas funcoes
 * e imprime os resultados na tela
 */
void calcular(int k)
{
	int j;
	double h = 0.2 / pow(2.0, (double)k); // calcula h
	int N = (int)(2.0 / h); // calcula N
	double x, res1, res2;
	
	// Reseta os valores
	y[0] = 1.0;
	y[1] = 0.0;
	
	printf("\n Resultados para k = %d\n", k);
	printf("---------------------------------\n");
	printf(" x          Euler      Analitica\n");
	printf("---------------------------------\n");
	// Iterando em cada um dos pontos
	for (j = 0; j <  N + 2; j++) {
		// Obtem valores
		x = (double)j * h;
		res1 = j > 0 ? euler_mod(x, h) : y[0];
		res2 = analitica(x);

		// Imprime		
		printf(" %lf   %lf   %lf\n", x, res1, res2);
		
		// Acumula erros
		erros[k-1][1] += pow(res2 - res1, 2.0);
		erros[k-1][2] += pow(res2, 2.0);
	}
	// Calcula e imprime o erro relativo
	erros[k-1][0] = sqrt(erros[k-1][1] / erros[k-1][2]);
	printf("---------------------------------\n");
	printf(" Erro relativo: %lf\n", erros[k-1][0]);
	printf("---------------------------------\n");
}

/**
 * Programa principal
 */
int main(int argc, char **argv)
{
	int k;
	
	// Imprimindo informacoes do problema
	printf("SME0104 Calculo Numerico\nTrabalho 2: Metodo de Euler Modificado\n\n");
	printf(" EDO alvo : y'' = y + e^x\n");
	printf("Intervalo : x em [0,2]\n");
	printf("  Valores : y(0) = 1 e y'(0) = 0\n");
	
	// Resultados para k = 1, 2, 3, 4
	for (k = 1; k < 5; k++)
		calcular(k);
	
	// Ordens de convergencia
	printf("\n k   Convergencia\n");
	printf("------------------\n");
	for (k = 0; k < 3; k++)
		printf(" %d   %lf\n", k + 1, log(erros[k][0] / erros[k+1][0]) / log(2.0));

	return EXIT_SUCCESS;
}

