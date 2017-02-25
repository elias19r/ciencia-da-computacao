#include <stdlib.h>
#include <stdio.h>

// Inclui TAD grafo
#include <graph.h>

// Inclui TAD dos dados de heuristica referentes a este problema
#include <data.h>

int main(int argc, char **argv) {
	int i, j, s, t;
	vertex_t *v;
	graph_t *g;
	
	double normal_dist, heuris_dist;
	int normal_count, heuris_count;
	
	printf("Origem\tDestino\tNumeros de Operacoes sem / com dados de heuristica\tDistancias sem / com dados de heuristica\n");
	
	for (i = 1; i < 28; i++) {
		for (j = 1; j < 28; j++) {
			normal_count = heuris_count = 0;
			normal_dist = heuris_dist = 0;
			s = i;
			t = j;
			v = NULL;
	
			// Aloca e inicializa o grafo
			g = (graph_t *) malloc(sizeof(graph_t));
			graph_initialize(g);
			g->dir = 0;
	
			// Adiciona os vertices
			graph_add_from_file(g, "table-1.dat");
	
			// Cria as arestas entre os vertices
			graph_link_from_file(g, "table-2.dat");

			// Aloca e adiciona dados na matriz de distancias em linha reta (dados de heuristica)
			data_alloc("table-3.dat");
	
			// Define vertice de destino (target)
			data_target(t);
			v = graph_get(g, t);
	
			printf("%d\t%d", s, t);

			// Roda o algoritmo de Dijkstra padrao
			normal_count = graph_dijkstra(g, s, NULL);
			normal_dist = v->distance;
	
			// Roda o algoritmo de Dijkstra, passando a funcao de heuristica
			heuris_count = graph_dijkstra(g, s, &data_heuristic);
			heuris_dist = v->distance;
			
			printf("\t%d / %d\t%.1lf / %.1lf\n", normal_count, heuris_count, normal_dist, heuris_dist);
	
			// Libera memoria dos dados de heuristica
			data_free();

			// Finaliza e desaloca o grafo
			graph_finalize(g);
			free(g);
		}
	}
	
	return 0;
}

