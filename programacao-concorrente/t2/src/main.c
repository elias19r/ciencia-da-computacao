#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <omp.h>

#include <dijkstra.h>
#include <floyd.h>

/**
 * Programa principal
 */
int main(int argc, char **argv)
{
	// Algoritmos
	// 	0 dijkstra
	// 	1 floyd-warshall
	int algorithm = 0;

	// Quantidade padrao de threads
	// 	0 default
	// 	n for parallel threads
	int nthreads = 1;
	
	// Quantidade de juncoes
	int n;

	// Quantidade de arestas
	int m;

	// Recebe argumentos
	if (argc > 1) {
		algorithm = atoi(argv[1]);
		if (algorithm != 0 && algorithm != 1) {
			printf("ERROR: invalid algorithm\nInform:\n\t0 dijkstra\n\t1 floyd-warshall\n");
			return EXIT_FAILURE;
		}
		
		if (argc > 2) {
			nthreads = atoi(argv[2]);
			if (nthreads < 0) {
				printf("ERROR: invalid number of threads\nInform:\n\t0 for default\n\tn for n parallel threads\n");
				return EXIT_FAILURE;				
			}
		}
	}
	
	// Le quantidade de juncoes e de arestas
	scanf("%d", &n);
	scanf("%d", &m);
	
	// Validacao da entrada
	if (n < 2 || n > 1000) {
		printf("ERROR: invalid n value\n");
		return EXIT_FAILURE;
	}
	if (m < n - 1 || m > (n * (n - 1)) / 2)
	{
		printf("ERROR: invalid m value\n");
		return EXIT_FAILURE;
	}

	// Dijkstra
	if (algorithm == 0) {
		Graph graph;
		int i;
		int A, B, W;
		int *allNodes, min;
		
		// Aloca e cria grafo da cidade
		createGraph(&graph);
		allNodes = (int *)malloc(sizeof(int) * n);
		if (allNodes == NULL) {
			printf("ERROR: could not allocate memory for graph (Dijkstra)\n");
			return EXIT_FAILURE;
		}

		// Adiciona todos os vertices
		for (i = 0; i < n; i++) {
			addNode(&graph, i+1);
		}

		// Adiciona todas arestas
		for (i = 0; i < m; i++) {
			scanf("%d %d %d", &A, &B, &W);
			addBothEdge(&graph, A, B, W);
		}	

	#ifdef _OPENMP
		if (nthreads > 0) {
			omp_set_num_threads(nthreads);
		}
		#pragma omp parallel for
	#endif
		// Executa Dijkstra para todos os vertices
		for (i = 0; i < n; i++) {
			allNodes[i] = dijkstra(&graph, i+1);
		}

		// Pega o menor valor		
		min = INT_MAX;
		for (i = 0; i < n; i++) {
			if (allNodes[i] < min) {
				min = allNodes[i];
			}
		}

		// Imprime resposta
		printf("%d\n", min);
		
		destroyGraph(&graph);
		free(allNodes);

	// Floyd-Warshall
	} else if (algorithm == 1) {
		int i, j, k, w;
		
		// Aloca e inicializa a matriz da cidade (grafo)
		int **town = (int **)malloc(sizeof(int *) * n);
		if (town == NULL) {
			printf("ERROR: could not allocate memory for graph (Floyd-Warshall)\n");
			return EXIT_FAILURE;
		}

		for (i = 0; i < n; i++) {
			town[i] = (int *)malloc(sizeof(int) * n);

			if (town[i] == NULL) {
				printf("ERROR: could not allocate memory for graph (Floyd-Warshall)\n");
				return EXIT_FAILURE;
			}

			for (j = 0; j < n; j++) {
				if (i == j)
					town[i][j] = 0; // distancia 0 de uma cidade para ela mesma
				else
					town[i][j] = INT_MAX; // INT_MAX significa nao ha aresta de i para j
			}
		}
	
		// Vai lendo os dados e populando o grafo
		for (k = 0; k < m; k++) {
			scanf("%d %d %d", &i, &j, &w);

			// Adiciona ambas arestas
			town[i-1][j-1] = w;
			town[j-1][i-1] = w;
		}
	
		// Faz o processamento utilizando o algoritmo de Floyd-Warshall
		printf("%d\n", floyd(town, n, nthreads));
		
		// Libera grafo da cidade
		for (i = 0; i < n; i++)
			free(town[i]);
		free(town);
	}

	return EXIT_SUCCESS;
}

