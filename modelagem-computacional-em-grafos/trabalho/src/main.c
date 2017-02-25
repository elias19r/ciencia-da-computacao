#include <stdlib.h>
#include <stdio.h>

// Inclui TAD grafo
#include <graph.h>

// Inclui TAD dos dados de heuristica referentes a este problema
#include <data.h>

int main(int argc, char **argv) {
	int count = 0, s = 0, t = 0;
	vertex_t *v = NULL;
	
	if (argc > 0 && argc < 3) {
		printf("Error: incorrect input.\nUsage: %s <source: int> <target: int>\n", argv[0]);
		return 1;
	}

	// Recebe vertices origem (s) e destino (t) dos argumentos do programa
	s = atoi(argv[1]);
	t = atoi(argv[2]);
	
	// Aloca e inicializa o grafo
	graph_t *g = (graph_t *) malloc(sizeof(graph_t));
	graph_initialize(g);
	g->dir = 0;
	
	// Adiciona os vertices
	graph_add_from_file(g, "table-1.dat");
	
	// Cria as arestas entre os vertices
	graph_link_from_file(g, "table-2.dat");

	// Imprime o grafo para conferir
	printf("GRAPH\n-----\n");
	graph_print(g, 'X');
	
	// Aloca e adiciona dados na matriz de distancias em linha reta (dados de heuristica)
	data_alloc("table-3.dat");
	
	// Define vertice de destino (target)
	data_target(t);
	v = graph_get(g, t);
	
	// Roda o algoritmo de Dijkstra padrao
	count = graph_dijkstra(g, s, NULL);

	// Imprime contagem de operacoes e caminho da origem ao destino
	printf("\nNORMAL DIJKSTRA\n---------------\n");
	printf("Source:     %d\n", s);
	printf("Taget:      %d\n\n", t);
	printf("Operations: %d\n\n", count);
	if (v != NULL)
		printf("Distance:   %.1lf\n\n", v->distance);
	printf("Path:\n");
	graph_print_path(g, s, t);
	
	// Roda o algoritmo de Dijkstra, passando a funcao de heuristica
	count = graph_dijkstra(g, s, &data_heuristic);

	// Imprime contagem de operacoes e caminho da origem ao destino
	printf("\nDIJKSTRA USING HEURISTIC DATA\n-----------------------------\n");
	printf("Source:     %d\n", s);
	printf("Target:     %d\n\n", t);
	printf("Operations: %d\n\n", count);
	if (v != NULL)
		printf("Distance:   %.1lf\n\n", v->distance);
	printf("Path:\n");
	graph_print_path(g, s, t);
	
	// Libera memoria dos dados de heuristica
	data_free();

	// Finaliza e desaloca o grafo
	graph_finalize(g);
	free(g);
	
	return 0;
}

