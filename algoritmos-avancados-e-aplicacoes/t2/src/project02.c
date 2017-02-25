#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Tamanho do buffer auxiliar
#define STRAUX_SIZE 2000

/**
 * Estrutura para guardar as informacoes de uma Serie Temportal
 */
typedef struct {
	int id;         // ID (ou classe) da serie
	double *values; // valores numericos
	int numValues;  // quantidade de valores
} TIME_SERIE;

/**
 * Le e retorna todas as Series Temporais de um arquivo texto no formato
 * especificado na descricao do projeto
 */
TIME_SERIE **readArchive(const char *filename, int *numSeries, int *maxNumValues) {
	int i = 0; // contador do total de series no arquivo
	int j;     // contador para a quantidade de valores de cada Serie
	char *strAux = (char *) malloc(sizeof(char) * STRAUX_SIZE); // buffer auxiliar
	char *ptr = NULL; // ponteiro para usar com strtok
	FILE *fp = NULL;
	TIME_SERIE **timeSeries = NULL;
	*maxNumValues = 0; // maior quantidade de valores que uma serie deste arquivo possui

	// Tenta abrir o arquivo
	fp = fopen(filename, "r");
	if(fp == NULL) {
		*numSeries = 0;
		return NULL;
	}

	while(!feof(fp)) {
		// Le uma linha do arquivo para o buffer auxiliar
		if(fgets(strAux, STRAUX_SIZE-1, fp) != NULL) {
			j = 1; // inicializa em 1 para nao usar a posicao 0 (facilitando no algoritmo do DTW)

			// Aloca e incializa uma nova Serie Temporal
			timeSeries = (TIME_SERIE **) realloc(timeSeries, sizeof(TIME_SERIE *) * (i + 1));
			timeSeries[i] = (TIME_SERIE *) malloc(sizeof(TIME_SERIE));
			timeSeries[i]->values = NULL;
		
			// Le a ID (ou classe) da serie
			ptr = strtok(strAux, " ");
			timeSeries[i]->id = atoi(ptr);

			// Vai lendo e alocando os valores numericos desta serie
			ptr = strtok(NULL, " ");
			while(ptr != NULL) {
				timeSeries[i]->values = (double *) realloc(timeSeries[i]->values, sizeof(double) * (j + 1));
				timeSeries[i]->values[j++] = atof(ptr);
				ptr = strtok(NULL, " ");
			}
			timeSeries[i++]->numValues = j;
			
			if (j > *maxNumValues)
				*maxNumValues = j;
		}
	}
	fclose(fp);
	free(strAux);

	// Retorna vetor de Series e quantidade
	*numSeries = i;
	return timeSeries;
}

/**
 * Funcao que retorna o minimo entre tres valores double
 */
double minimum(double num1, double num2, double num3) {
	double minimum = num1;

	if(minimum > num2) {
		minimum = num2;
	}
	if(minimum > num3) {
		minimum = num3;
	}
	
	return minimum;
}

/**
 * Funcao iterativa do algoritmo de DTW que recebe duas Series Temporais:
 * uma do teste e uma do treino e retorna a distancia DTW entre elas
 */
double dtw(TIME_SERIE *testeSerie, TIME_SERIE *treinoSerie, double **matrix) {
	double distance;
	int i, j;
	int row = testeSerie->numValues;
	int col = treinoSerie->numValues;
	double cost;

	// Aplica o algoritmo e preenche os valores da matriz
	for(i = 1; i < row; i++) {
		for(j = 1; j < col; j++) {
			cost = pow(testeSerie->values[i] - treinoSerie->values[j], 2);
			matrix[i][j] = cost + minimum(matrix[i-1][j], matrix[i][j-1], matrix[i-1][j-1]);
		}
	}
	distance = matrix[row-1][col-1];

	return distance;
}

/**
 * Funcao principal
 */
int main(int argc, char *argv[]) {
	int numSeriesTeste, numSeriesTreino, maxRow, maxCol;

	// Le os dois arquivos para memoria
	TIME_SERIE **timeSeriesTeste = readArchive("./data/teste.txt", &numSeriesTeste, &maxRow);
	TIME_SERIE **timeSeriesTreino = readArchive("./data/treino.txt", &numSeriesTreino, &maxCol);
	
	if (timeSeriesTeste == NULL || timeSeriesTreino == NULL) {
		printf("ERROR: couldn't read files\n");
		return 0;
	}

	int i, j, selected = 0;
	double **matrix = NULL; // matriz para o algoritmo DTW
	clock_t start, end;

	// Aloca e inicializa valores da coluna 0 e da linha 0 da matriz para o DTW	
	matrix = (double **) malloc(sizeof(double *) * maxRow);
	for(i = 0; i < maxRow; i++) {
		matrix[i] = (double *) malloc(sizeof(double) * maxCol);
	}
	for(i = 1; i < maxCol; i++) {
		matrix[0][i] = INFINITY;
	}
	for(i = 1; i < maxRow; i++) {
		matrix[i][0] = INFINITY;
	}
	matrix[0][0] = 0.0;

	double distance, distanceAux; // distancia retornada pelo algoritmo do DTW
	int hit = 0;     // contador de acertos
	double accuracy; // acuracia dos acertos

	start = clock();

	/**
	 * Compara cada serie temportal de teste com todas as series de treino
	 * Seleciona aquela que tem a menor distancia DTW e confere se as ID sao iguais
	 * Se sim, contabiliza um acerto
	 */
	for(i = 0; i < numSeriesTeste; i++) {
		distance = INFINITY;
		for(j = 0; j < numSeriesTreino; j++) {
			distanceAux = dtw(timeSeriesTeste[i], timeSeriesTreino[j], matrix);
			if(distanceAux < distance) { 
				distance = distanceAux;
				selected = j;
			}
		}
		if(timeSeriesTeste[i]->id == timeSeriesTreino[selected]->id) {
			hit++;
		}
	}

	end = clock();
	
	// Imprime a acuracia e tempo decorrido na tela
	accuracy = (double) hit / (double) numSeriesTeste;
	printf("Accuracy: %lf \n", accuracy);
	printf("Elapsed time: %.2lf s \n", (double) (end - start) / 1000000.0);

	// Desalocando memoria
	for(i = 0; i < numSeriesTeste; i++) {
		free(timeSeriesTeste[i]->values);
		free(timeSeriesTeste[i]);
	}
	free(timeSeriesTeste);
	for(i = 0; i < numSeriesTreino; i++) {
		free(timeSeriesTreino[i]->values);
		free(timeSeriesTreino[i]);
	}
	free(timeSeriesTreino);
	for(i = 0; i < maxRow; i++) {
		free(matrix[i]);
	}
	free(matrix);
	
	return 0;
}

