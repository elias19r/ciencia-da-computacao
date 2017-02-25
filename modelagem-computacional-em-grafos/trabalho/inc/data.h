#ifndef _DATA_H_
#define _DATA_H_

/**
 * Estrutura para armazenar os dados de heuristica
 */
typedef struct data_s {
	double **distances;
	int lines;
	int cols;
} data_t;

/**
 * Aloca os dados de heuristica para memoria a partir de um arquivo
 */
void data_alloc(const char *filename);

/**
 * Desaloca os dados de heuristica e libera memoria
 */
void data_free();

/**
 * Funcao que sera adicionada ao algoritmo de Dijkstra
 */
double data_heuristic(int v);

/**
 * Define vertice de destino (target)
 */
void data_target(int index);

#endif
