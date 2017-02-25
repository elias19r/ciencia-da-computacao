/**
 * ICMC-USP
 * Implementacao de backtracking para colorir mapas
 * Elias Italiano Rodrigues, 7987251
 * Rodolfo Megiato de Lima,  7987286
 * 2013/09
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <mylib.h>
#include <color_map.h>

/**
 * Estrutura de dados para guardar todas as informacoes referentes ao caso
 */
typedef struct {
	int num;         // quantidade de regioes no mapa
	char **names;    // nomes das regioes
	int **neighbour; // vizinhos de cada regiao (grafo: vetores de adjacentes)
	int **domains;   // dominio de cara regiao (cores que podem assumir)
	int *colors;     // resultado com as cores que cada regiao foi pintada
	int *degrees;    // quantidade de vizinhos (grau) de cada regiao
	unsigned long long count; // quantidade de atribuicoes feitas no processo de colorir
} map_t;

/**
 * Variaveis internas ao programa
 */
map_t *m = NULL;     // estrutura que guarda as informacoes do mapa
int amount = 0;      // quantidade de cores
char **color = NULL; // nome de cada cor

/**
 * Inicializa a estrutura de dados para o mapa
 * amount_colors quantidade de cores
 *           ... nome de cada cor
 *
 * Exemplo:
 * 	color_map_init(4, "Yellow", "Green", "Blue", "Red");
 */
void color_map_init(int amount_colors, ...) {
	int i;
	va_list ap;
	char *color_name;

	// Se ja havia algo alocado, libera
	color_map_free();

	// Aloca nova estrutura de dados e reseta valores
	m = (map_t *) malloc(sizeof(map_t));
	m->num = 0;
	m->names = NULL;
	m->neighbour = NULL;
	m->domains = NULL;
	m->colors = NULL;
	m->degrees = NULL;
	m->count = 0;

	// Define a quantidade de cores
	amount = amount_colors;
	color = (char **) malloc(sizeof(char *) * (amount_colors + 1));

	// Nome da primera cor obrigatoriamente defina como "None"
	color[0] = (char *) malloc(sizeof(char) * (strlen("None") + 1));
	strcpy(color[0], "None");

	// Define o nome de cada cor pegando os argumentos da funcao
	va_start(ap, amount_colors);
	for (i = 1; i <= amount_colors; i++) {
		color_name = va_arg(ap, char *);
		color[i] = (char *) malloc(sizeof(char) * (strlen(color_name) + 1));
		strcpy(color[i], color_name);
	}
	va_end(ap);
}

/**
 * Recebe a entrada, trata e armazena na estrutura de dados interna
 * 
 * A entrada contem na primeira linha o numero de regioes do mapa
 * Nas linhas seguintes, o nome de cada regiao, dois pontos, nomes de suas
 * regioes vizinhas separadas por virgula e terminado com ponto final
 * 
 * Exemplo:
 * N
 * Region 1: Region 5, Region 3, Region 7.
 * Region 2: Region 10, Region 3, Region 6.
 * ...
 * Region N: Region 12.
 */
int color_map_in() {
	int i, j, count;
	char **list, *word;

	// Recebe a quantidade de regioes no mapa
	scanf("%d\n", &m->num);

	if (m->num < 1) return 0; // confere se ha pelo menos 1 regiao

	// Aloca o espaco necessario na estrutura de dados
	m->names = (char **) malloc(sizeof(char *) * m->num);
	m->neighbour = (int **) malloc(sizeof(int *) * m->num);
	m->domains = (int **) malloc(sizeof(int *) * (m->num + 1));
	m->colors = (int *) calloc(m->num, sizeof(int));
	m->degrees = (int *) malloc(sizeof(int) * m->num);

	// Variavel auxiliar para guardar temporariamente a lista de regioes vizinhas para cada regiao
	list = (char **) malloc(sizeof(char *) * m->num);

	// Recebe as regioes e suas respectivas listas de vizinhos
	for (i = 0; i < m->num; i++) {
		m->names[i] = scan();
		list[i] = scanln();

		/* Inicializa o dominios (cores que acada regiao pode assumir)
		 * O indice zero eh usado como contador para o tamanho do dominio,
		 * os demais indices so podem receber valores  0 ou 1 para marcar
		 * se a regiao tem ou nao tem determinada cor em seu dominio
		 * 	0 para tem a cor
		 * 	1 para NAO tem a cor
		 */
		m->domains[i] = (int *) calloc(amount+1, sizeof(int));
		m->domains[i][0] = amount;
	}
	// Entrada invalida no dominio das regioes para ter alguem a priori
	// para fazer a comparacao quando usar MRV
	m->domains[i] = (int *) calloc(amount+1, sizeof(int));
	m->domains[i][0] = amount+1;

	// Processa a lista de vizinhos de cada regiao e cria o grafo (vetores de adjacentes)
	for (i = 0; i < m->num; i++) {
		count = 0;
		m->neighbour[i] = NULL;

		// Pega a primeira palavra na lista de vizinhos
		word = next(list[i]);

		// Enquanto houver palavras na lista de vizinhos
		while (*word != '\0') {
			trim(&word);

			// Procura pelo indice que foi associado a palavra
			for (j = 0; j < m->num; j++)
				if (strcmp(m->names[j], word) == 0) {
					m->neighbour[i] = (int *) realloc(m->neighbour[i], sizeof(int) * (count + 1));
					m->neighbour[i][count++] = j; // faz a ligacao no grafo
					break;
				}
			m->degrees[i] = count; // define o grau

			free(word);
			word = next(NULL); // pega a proxima palavra
		}

		free(word);
		free(list[i]);
	}
	free(list);

	return 1;
}

/**
 * Confere se a atribuicao de uma cor a uma regiao eh consistente, ou seja,
 * nenhum de seus vizinhos esta pintado com aquela cor
 */
static int color_map_is_consistent(int region, int color) {
	int i;

	// Percorre os vizinhos e confere
	for (i = 0; i < m->degrees[region]; i++)
		if (m->colors[m->neighbour[region][i]] == color)
			return 0;

	return 1;
}

/**
 * Backtracking para colorir o mapa
 */
static int color_map_make_recursive(int method) {
	int region, color, i;
	int domain_bk[amount+1]; // variavel para backup do dominio de uma regiao
	                         // usada no metodo com forward-checking

	// Se metodo 1 (forward-checking), confere se alguem ficou sem opcao de cor
	if (method >= 1)
		for (i = 0; i < m->num; i++)
			if (m->domains[i][0] < 1) return 0;

	// Seleciona uma regiao que ainda nao tem cor
	if (method == 2) {
	// Modo com MRV
		int j;
		i = m->num;
		for (j = 0; j < m->num; j++)
			if (m->colors[j] == 0 && m->domains[j][0] < m->domains[i][0])
				i = j;

	} else if (method >= 3) {
	// Modo com MRV + verificacao de grau (degree)
		int j;
		i = m->num;
		for (j = 0; j < m->num; j++)
			if (
				m->colors[j] == 0 &&
				(m->domains[j][0] < m->domains[i][0] ||
				(m->domains[j][0] == m->domains[i][0] && m->degrees[j] < m->degrees[i]))
			)
				i = j;

	// Modo convencional
	} else
		for (i = 0; i < m->num; i++)
			if (m->colors[i] == 0) break;

	// Se i igual m->num, significa que todas regioes ja tem cor, entao sucesso!
	if (i == m->num) return 1;

	// Define a regiao
	region = i;

	// Itera para cada cor disponivel
	for (color = 1; color <= amount; color++) {
		// Confere se atribuicao eh consistente
		if (!color_map_is_consistent(region, color))
			continue;

		// Confere se cor esta no dominio
		if (method >= 1 && m->domains[region][color] == 1)
			continue;

		// Atribui a cor e contabiliza a operacao
		m->colors[region] = color;
		m->count++;

		// Se esta no metodo de forward-checking
		if (method >= 1) {
			// Faz backup do dominio da regiao atual
			memcpy(domain_bk, m->domains[region], (amount+1) * sizeof(int));

			// Altera dominio
			m->domains[region][0] = 1;
			for (i = 1; i <= amount; i++)
				if (i != color) m->domains[region][i] = 1;

			/* Altera dominio de todas as regioes vizinhas que:
			 * 	ainda nao foram pintadas,
			 * 	e ainda tem a cor em seu dominio
			 */
			for (i = 0; i < m->degrees[region]; i++)
				if (m->colors[m->neighbour[region][i]] == 0 && m->domains[m->neighbour[region][i]][color] == 0) {
					m->domains[m->neighbour[region][i]][color] = 1;
					m->domains[m->neighbour[region][i]][0]--;
				}
		}

		// Faz a chamada recursiva, "descendo" em profundidade do grafo
		if (color_map_make_recursive(method) != 0)
			return 1;

		// Se falhou em algum ponto o algoritmo voltara para esta linha (o backtracking ocorre aqui)
		// Entao nao foi uma boa escolha: remove a cor
		m->colors[region] = 0;

		// Defaz alteracoes nos dominios (se esta no metodo de forward-checking)
		if (method >= 1) {
			// Restaura backup do dominio
			memcpy(m->domains[region], domain_bk, (amount+1) * sizeof(int));

			// Restaura dominio dos vizinhos que foram alterados
			for (i = 0; i < m->degrees[region]; i++)
				if (m->colors[m->neighbour[region][i]] == 0 && m->domains[m->neighbour[region][i]][color] == 1) {
					m->domains[m->neighbour[region][i]][color] = 0;
					m->domains[m->neighbour[region][i]][0]++;
				}
		}
	}

	return 0;
}

/**
 * Realiza a coloracao do mapa de acordo com o metodo escolhido
 * Eh um "stub" para a funcao recursiva de backtracking
 * 
 * method pode ser:
 * 	0 : backtracking simples, sem poda
 * 	1 : backtracking com forward-checking
 * 	2 : backtracking com forward-checking e MRV
 * 	3 : backtracking com forward-checking, MRV e desempate por grau (degree)
 */
int color_map_make(int method) {
	return color_map_make_recursive(method);
}

/**
 * Gera a saida com as cores definidas para cada regiao
 * 
 * Exemplo:
 * Region 1: Yellow.
 * Region 2: Blue.
 * Region 3: Green.
 * Region 4: Green.
 * ...
 * Region N: Red.
 */
void color_map_out() {
	int i;

	for (i = 0; i < m->num; i++)
		printf("%s: %s.\n", m->names[i], color[m->colors[i]]);
}

/**
 * Libera toda memoria alocada para um caso
 */
void color_map_free() {
	int i;

	// Nomes das cores disponiveis
	if (color != NULL) {
		for (i = 0; i <= amount; i++)
			if (color[i] != NULL)
				free(color[i]);
		free(color);
		color = NULL;
	}

	// Estrutura de dados do mapa
	if (m != NULL) {
		// Libera nomes
		if (m->names != NULL) {
			for (i = 0; i < m->num; i++)
				if (m->names[i] != NULL)
					free(m->names[i]);
			free(m->names);
		}

		// Libera vizinhos (grafo)
		if (m->neighbour != NULL) {
			for (i = 0; i < m->num; i++)
				if (m->neighbour[i] != NULL)
					free(m->neighbour[i]);
			free(m->neighbour);
		}

		// Libera graus
		if (m->degrees != NULL)
			free(m->degrees);

		// Libera dominios
		if (m->domains != NULL) {
			for (i = 0; i <= m->num; i++)
				if (m->domains[i] != NULL)
					free(m->domains[i]);
			free(m->domains);
		}

		// Libera cores atribuidas
		if (m->colors != NULL)
			free(m->colors);

		// Libera a estrutura de dados e reseta para nulo
		free(m);
		m = NULL;
	}
}

/**
 * Imprime na tela todas as informacoes lidas para conferir se esta tudo OK
 */
void color_map_debug() {
		int i, j;

		printf("\nNAMES\n-----\n");
		for (i = 0; i < m->num; i++)
			printf("%4d: %s\n", i, m->names[i]);

		printf("\nGRAPH\n-----\n");
		for (i = 0; i < m->num; i++) {
			printf("%4d :", i);
			for (j = 0; j < m->degrees[i]; j++)
				printf("%4d", m->neighbour[i][j]);
			printf("\n");
		}
		
		printf("\nDEGREES\n------\n");
		for (i = 0; i < m->num; i++)
			printf("%4d", i);
		printf("\n");
		for (i = 0; i < m->num; i++)
			printf("%4d", m->degrees[i]);
		
		printf("\n\nCOLORS AVAILABLE\n----------------\n");
		for (i = 0; i <= amount; i++)
			printf("%4d: %s\n", i, color[i]);

		printf("\nDOMAINS\n-------\n");
		for (i = 0; i < m->num; i++) {
			printf("%4d :%3d |", i, m->domains[i][0]);
			for (j = 1; j <= amount; j++)
				printf("%3d", m->domains[i][j]);
			printf("\n");
		}
		printf("%4d :%3d |", i, m->domains[i][0]);
		for (j = 1; j <= amount; j++)
			printf("%3d", m->domains[i][j]);
		printf(" (invalid)\n");

		printf("\nRESULT COLORS\n-------------\n");
		for (i = 0; i < m->num; i++)
			printf("%4d", i);
		printf("\n");
		for (i = 0; i < m->num; i++)
			printf("%4d", m->colors[i]);

		printf("\n\nTOTAL COUNT\n-----------\n%llu\n\n", m->count);
}

