/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 1: Metodos de Busca - Problema das N-Rainhas
 * 
 * SCC0230 Inteligencia Artificial
 * 
 * Elias Italiano Rodrigues  7987251
 * Rodolfo Megiato de Lima   7987286
 * Rodrigo Rusa              7986970
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/**
 * Vetor que guarda o estado da solucao
 * 
 * O indice representa a coluna de uma rainha
 * O valor representa a linha dessa rainha
 */
int *Q = NULL;

/**
 * Matriz para guardar os valores disponiveis nos dominios de cada rainha
 * E' usado na busca com restricoes
 */
int **D = NULL;

/**
 * Quantidade de rainhas
 */
int n;

/**
 * Quantidade de movimentos executados
 */
int moves = 0;

/**
 * Imprime um estado das rainhas, e pode desenhar um "tabuleiro"
 */
void print_queens(int board, int conflicts)
{
	int i, j;

	printf("Q = [");
	for (i = 0; i < n; i++) 
		printf(" %d ", Q[i]);
	printf("]\n");

	// Confere se e' para imprimir um tabuleiro
	if (!board) return;

	printf("\n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			if (conflicts && D != NULL && D[j] != NULL)
				printf("------");
			else
				printf("----");
		printf("-\n");
		printf("| ");
		for (j = 0; j < n; j++)
			if (Q[j] == i) {
				if (conflicts && D != NULL && D[j] != NULL)
					printf("(%d) | ", D[j][i]);
				else
					printf("0 | ");
			}
			else {
				if (conflicts && D != NULL && D[j] != NULL)
					printf(" %d  | ", D[j][i]);
				else
					printf("  | ");
			}
		printf("\n");
	}
	for (j = 0; j < n; j++)
		if (conflicts && D != NULL && D[j] != NULL)
			printf("------");
		else
			printf("----");
	printf("-\n");
}

/**
 * Retorna o valor absoluto de um inteiro
 */
int abs(int value)
{
	if (value >= 0)
		return value;

	return -value;
}

/**
 * Confere se um estado Q de rainhas e' uma solucao, ou seja,
 * nenhuma rainha esta' atacando outra rainha
 */
int solved()
{
	int j, q;
	
	for (j = 0; j < n; j++)
		for (q = 0; q < n; q++)
			// Confere se esta' na mesma linha ou na mesma diagonal
			if (j != q && (Q[j] == Q[q] || abs(Q[j] - Q[q]) == abs(j - q)))
				return 0;
	
	return 1;
}

/**
 * Confere se todas rainhas foram colocadas no "tabuleiro"
 * E' usada na busca com restricoes
 */
int all_set()
{
	int j;

	for (j = 0; j < n; j++)
		if (Q[j] == -1)
			return 0;

	return 1;
}

/**
 * Faz verificacao para conferir se nenhuma das rainhas, ainda nao
 * colocas no tabuleiro, esta' sem possiveis valores em seu dominio.
 * E' usada na busca com restricoes
 */
int forward_check()
{
	int i, j, flag;

	for (j = 0; j < n; j++) {
		if (Q[j] != -1)
			continue;
			
		flag = 1;
		for (i = 0; i < n; i++)
			if (D[j][i] == 0) {
				flag = 0;
				break;
			}

		if (flag == 1)
			return 0;
	}

	return 1;
}

/**
 * Seleciona uma rainha para colocar no tabuleiro.
 * Trata-se da tecnica de MRV (menores valores remanescentes)
 * E' usado na busca com restricoes.
 */
int select_queen()
{
	int i, j, min = n, count, queen = 0;

	for (j = 0; j < n; j++) {
		if (Q[j] != -1)
			continue;

		count = 0;
		for (i = 0; i < n; i++)
			if (D[j][i] == 0)
				count++;

		if (count < min) {
			min = count;
			queen = j;
		}
	}

	return queen;
}

/**
 * Atualiza os dominios das rainhas
 * com relacao ao movimento da rainha j para a linha i
 * E' usada na busca com restricoes
 */
void update_domain(int j, int i, int restore)
{
	int k, l, add = 1;
	
	// Se e' para restaurar, soma -1
	if (restore)
		add = -1;

	// Incrementa os dominios com relacao aos conflitos de coluna
	// da propria rainha j com ela mesma
	for (l = 0; l < n; l++)
		if (l != i)
			D[j][l] += add;
	
	// Incrementa os dominios com relacao aos conflitos de linha
	for (k = 0; k < n; k++)
		if (k != j)
			D[k][i] += add;

	// Incrementa os dominios com relacao as diagonais

	k = j - 1;
	l = i - 1;
	while (k >= 0 && k < n && l >= 0 && l < n)
		D[k--][l--] += add;

	k = j + 1;
	l = i + 1;
	while (k >= 0 && k < n && l >= 0 && l < n)
		D[k++][l++] += add;

	k = j - 1;
	l = i + 1;
	while (k >= 0 && k < n && l >= 0 && l < n)
		D[k--][l++] += add;

	k = j + 1;
	l = i - 1;
	while (k >= 0 && k < n && l >= 0 && l < n)
		D[k++][l--] += add;
}

/**
 * Funcao de heuristica para Hill Climb
 * 
 * Retorna a melhor linha para movimentar a rainha j,
 * aquela onde a quantidade de conflitos e' menor
 */
int h(int j)
{
	int i, q, min = n, count, option = Q[j];

	// Para cada linha	
	for (i = 0; i < n; i++) {
		count = 0;

		// Confere o conflito com cada rainha
		for (q = 0; q < n; q++) {
			if (q == j) continue;
			
			// Confere se esta' na mesma linha
			if (Q[q] == i)
				count++;

			// Confere se esta' na diagonal
			else if (abs(i - Q[q]) == abs(j - q))
				count++;
		}

		// Define a linha para onde vai mover
		if (count < min) {
			min = count;
			option = i;
		}
	}

	return option;
}

/**
 * Hill Climb
 * 
 * Escolhe uma linha para mover a rainha j usando a funcao de heuristica h()
 * Uma vez escolhida, nao volta atras. Podendo nao encontrar uma solucao...
 */
int hill_climb()
{
	int j;

	for (j = 0; j < n; j++) {
		// Move a rainha j e contabiliza esse movimento
		Q[j] = h(j);
		moves++;
		
		if (solved())
			return 1;
	}
	
	return solved();
}

/**
 * Busca em profundidade por uma solucao para o problema
 * 
 * E um nivel, move-se a rainha j para uma linha i
 * No seguinte nivel, move-se a rainha j+1 ...
 * 
 * Em cada estado, confere se e' uma solucao para o problema
 * Faz backtracking caso o no' verificado nao seja uma solucao
 */
int dfs(int j)
{
	int i;
	int past_i;
	
	// Caso base, chegou numa folha da arvore
	if (j == n) {
		// Se resolveu, retorna sucesso. Senao, falha.
		if (solved())
			return 1;

		return 0;
	}

	// Se resolveu, retorna sucesso. Senao, vai descer na arvore
	if (solved())
		return 1;

	// Escolhe uma linha i para mover a rainha j
	for (i = 0; i < n; i++) {

		// Guarda a linha atual para fazer o "historico"
		past_i = Q[j];
		
		// Move a rainha e contabiliza esse movimento
		Q[j] = i;
		moves++;

		// Chama recursivo para descer na arvore e mover a rainha j+1
		if (dfs(j + 1))
			return 1;
		else
			// Caso nao encontrou uma solucao,
			// restaura o valor antigo desta rainha e tenta um proximo i
			Q[j] = past_i;
	}

	return 0;
}

/**
 * Constraints
 * 
 * A Busca com Satisfacao de Restricoes usa as seguintes tecnicas:
 * Forward-checking: ao mover uma rainha, confere se nenhuma das outras
 * 	rainhas ficou possiveis valores em seu dominio; se isso ocorreu
 * 	desfaz o movimento feito e restaura o dominio como estava antes.
 * MRV: ao selecionar qual rainha sera' colocada no tabuleiro, seleciona-se
 * 	aquela que tem menos valores possiveis em seu dominio.
 */
int constraints(int j)
{
	int i, past_i;

	// Se todas rainhas foram colocadas no tabuleiro, entao encontrou solucao
	if (all_set())
		return 1;

	for (i = 0; i < n; i++) {
		if (D[j][i] > 0)
			continue;

		// Guarda a linha atual para fazer o "historico"
		past_i = Q[j];

		// Move a rainha e contabiliza esse movimento
		Q[j] = i;
		moves++;

		// Atualiza o dominio das rainhas considerando o movimento feito
		update_domain(j, i, 0);

		// Confere se nenhuma rainha ficou sem possiveis valores em seu dominio
		if (forward_check()) {
			
			// Chama recursivo para continuar procurando uma solucao
			if (constraints(select_queen()))
				return 1;

			// Reconstroi o dominio e restaura movimento feito
			update_domain(j, i, 1);
			Q[j] = past_i;

		} else {
			// Reconstroi o dominio e restaura movimento feito
			update_domain(j, i, 1);
			Q[j] = past_i;
		}
	}

	return 0;
}

/**
 * Programa principal
 */
int main(int argc, char **argv)
{
	int method, draw = 1, j, ret;
	
	// Confere se passou argumento <n>
	if (argc < 3) {
		printf("usage: n_queens <method> <n> [<draw>]\n");
		return EXIT_FAILURE;
	}
	
	// Confere se e' para desenhar os tabuleiros
	if (argc == 4)
		draw = atoi(argv[3]);

	// Metodo usado para resolver
	// 	0: Hill Climb
	// 	1: DFS
	// 	2: Constraints
	method = atoi(argv[1]);
	if (method < 0 || method > 2) {
		printf("ERRO: metodo invalido\n");
		return EXIT_FAILURE;
	}
	
	// Quantidade de rainhas
	n = atoi(argv[2]);
	if (n < 1) {
		printf("ERRO: quantidade de rainhas invalida\n");
		return EXIT_FAILURE;
	}
	
	// Aloca vetor que guarda as posicoes das rainhas
	Q = (int *)malloc(sizeof(int) * n);
	
	if (method < 2) {
		// Gerar uma solucao inicial
		srand(time(NULL));
		for (j = 0; j < n; j++)
			Q[j] = rand() % n;
	} else {
		// Inicializa o tabuleiro sem rainhas
		for (j = 0; j < n; j++) 
			Q[j] = -1;

		// Inicializa os dominios
		D = (int **)malloc(sizeof(int *) * n);
		for (j = 0; j < n; j++)
			D[j] = (int *)calloc(n, sizeof(int));
	}
	if (draw) {
		printf("Estado inicial gerado:\n\n");
		print_queens(1, 1);
	}

	printf("\nRainhas: %d\n", n);

	// Hill Climb
	if (method == 0) {
		printf("Metodo: Hill Climb\n");
		ret = hill_climb();

	// DFS
	} else if (method == 1) {
		printf("Metodo: DFS\n");
		ret = dfs(0);
	
	// Constraints
	} else {
		printf("Metodo: Constraints\n");
		ret = constraints(0);
	}
	
	if (ret)
		printf("Resolvido!\n");
	else
		printf("Solucao nao encontrada...\n");

	printf("Movimentos: %d\n", moves);

	// Estado final
	if (draw) {
		printf("\nEstado final:\n\n");
		print_queens(1, 1);
	}
	
	// Libera memoria
	if (Q != NULL)
		free(Q);
	if (D != NULL) {
		for (j = 0; j < n; j++)
			if (D[j] != NULL)
				free(D[j]);
		free(D);
	}
	
	return EXIT_SUCCESS;
}

