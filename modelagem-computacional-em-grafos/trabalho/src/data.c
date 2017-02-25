#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <data.h>

/**
 * Acesso local aos dados de heuristica e o vertice destino (target)
 */
data_t *d = NULL;
int t = 0;

/**
 * Aloca os dados de heuristica para memoria a partir de um arquivo
 */
void data_alloc(const char *filename) {
	FILE *fp;
	int i, j;
	char buffer[50];

	// Aloca estrutura	
	d = (data_t *) calloc(1, sizeof(data_t));
	
	fp = fopen(filename, "r");
	if (fp != NULL) {
		fscanf(fp, "%d", &(d->lines));
		fscanf(fp, "%d", &(d->cols));
		
		// aloca memoria
		d->distances = (double **) calloc(d->lines + 1, sizeof(double *));
		for (i = 0; i <= d->lines; i++)
			d->distances[i] = (double *) calloc(d->cols + 1, sizeof(double));
			
		// le os valores, inutilizando os indices zero (0)
		for (i = 1; i <= d->lines; i++)
			for (j = 1; j <= d->cols; j++) {
				fscanf(fp, "%s", buffer);
				if (strcmp(buffer, "#") == 0)
					d->distances[i][j] = INFINITY;
				else
					d->distances[i][j] = atof(buffer);
			}
		
		fclose(fp);
	}
}

/**
 * Desaloca os dados de heuristica e libera memoria
 */
void data_free() {
	int i;
	
	if (d != NULL) {
		if (d->distances != NULL) {
			for (i = 0; i <= d->lines; i++)
				free(d->distances[i]);
			free(d->distances);
		}
		free(d);
	}
}

/**
 * Funcao que sera adicionada ao algoritmo de Dijkstra
 */
double data_heuristic(int v) {
	if (v == 0 || t == 0 || d == NULL)
		return INFINITY;
	
	// Consulta sempre na coluna maior
	if (v > t)
		return d->distances[t][v];

	return d->distances[v][t];
}

/**
 * Define vertice de destino (target)
 */
void data_target(int index) {
	t = index;
}
