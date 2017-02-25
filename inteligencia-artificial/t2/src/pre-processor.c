#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Tamanho para o buffer que guarda um nome de produto
#define NAME_SIZE 401

// Valor default para nfreq (os n produtos mais frequentes)
#define NFREQ 30

/**
 * Estrutura para guardar um produto
 * Guarda seu nome em uma string e quantidade de vezes que apareceu em todos exemplos
 */
typedef struct {
	char *name;
	int occur;
} product_t;

/**
 * Estrutura para guardar uma compra
 * Possui um vetor de strings com os nomes dos itens e o total de itens
 */
typedef struct {
	char **item;
	int total;
} purchase_t;

/**
 * Funcao principal
 */
int main(int argc, char **argv)
{
	// Valor que define os n mais frequentes
	int nfreq = NFREQ;

	// Variaveis de buffer para guardar linha de compra e nome de produto
	char *line = NULL;
	size_t len = 0;
	char *name;

	// Iteradores e auxiliares
	int i, j, k, offset, found;
	product_t tmp;
	
	// Vetor de compras
	purchase_t *purchase = NULL;
	int n = 0;
	
	// Conjunto com os nomes de todos os produtos
	product_t *product = NULL;
	int count = 0;
	
	// Pega quantos n frequentes deseja-se
	if (argc > 1) {
		nfreq = atoi(argv[1]);
		if (nfreq == 0) {
			nfreq = NFREQ;
			fprintf(stderr, "NOTE: nfreq fixed to its default value\n");
		}
	}
	
	if (nfreq < 0) {
		fprintf(stderr, "NOTE: nfreq is not being used\n");
	}

	fprintf(stderr, "nfreq: %d\n", nfreq);

	// Le toda a entrada padrao, linha por linha, para a memoria, pois fica mais facil para trabalhar
	name = (char *)malloc(sizeof(char) * NAME_SIZE);
	while ((getline(&line, &len, stdin)) != -1) {

		// Leu uma linha, entao aloca uma nova compra no vetor de compras
		purchase = (purchase_t *)realloc(purchase, sizeof(purchase_t) * (n + 1));
		purchase[n].total = 0;
		purchase[n].item = NULL;
		
		// Copia todos os nomes de produto que estao nessa linha para a compra alocada
		offset = 0;
		while (sscanf(line + offset, "%s", name) != -1) {
			purchase[n].item = (char **)realloc(purchase[n].item, sizeof(char *) * (purchase[n].total + 1));
			purchase[n].item[purchase[n].total] = (char *)malloc(sizeof(char) * (strlen(name) + 1));
			strcpy(purchase[n].item[purchase[n].total], name);
			purchase[n].total++;
			offset += strlen(name) + 1;
		}
		n++;
	}

	// Tendo agora os dados das compras em memoria, cria-se o conjunto com os nomes dos produtos
	for (i = 0; i < n; i++) {
		for (j = 0; j < purchase[i].total; j++) {

			// Busca linearmente para saber se o produto ja foi inserido
			found = 0;
			for (k = 0; k < count; k++) {
				if (strcmp(product[k].name, purchase[i].item[j]) == 0) {
					found = 1;
					product[k].occur++;
					break;
				}
			}
			
			// Se ainda nao foi inserido, insere.
			if (!found) {
				product = (product_t *)realloc(product, sizeof(product_t) * (count + 1));
				product[count].name = (char *)malloc(sizeof(char) * (strlen(purchase[i].item[j]) + 1));
				strcpy(product[count].name, purchase[i].item[j]);
				product[count].occur = 1;
				count++;
			}
		}
	}

	// Verifica se existe nfreq produtos no conjunto de produtos
	if (nfreq > count) {
			fprintf(stderr, "ERROR: nfreq can not be greater than count\n");
			exit(EXIT_FAILURE);
	}

	// Ordena o conjunto de produtos pela sua ocorrencia (frequencia) nas compras
	if (nfreq > 0) {
		for (i = 1; i < count; i++) {
			for (j = 0; j < count-i; j++) {
				if (product[j].occur < product[j+1].occur) {
					tmp = product[j];
					product[j] = product[j+1];
					product[j+1] = tmp;
				}
			}
		}
	}


	// Caso nfreq nao esteja sendo usado, seta ela pra ser igual count
	if (nfreq < 0) {
		nfreq = count;
	}

	fprintf(stderr, "\nnfreq products:\n");
	for (i = 0; i < nfreq; i++) {
		fprintf(stderr, "\t%5d %s\n", product[i].occur, product[i].name);
	}
	
	// Ordena o conjunto de produtos pela nome do produto (somente os nfreq produtos)
	for (i = 1; i < nfreq; i++) {
		for (j = 0; j < nfreq-i; j++) {
			if (strcmp(product[j].name, product[j+1].name) > 0) {
				tmp = product[j];
				product[j] = product[j+1];
				product[j+1] = tmp;
			}
		}
	}
	
	fprintf(stderr, "\nnfreq products (sorted by name):\n");
	for (i = 0; i < nfreq; i++) {
		fprintf(stderr, "\t%5d %s\n", product[i].occur, product[i].name);
	}

	// Imprime o comeco do arquivo .arff com a relacao e atributos
	printf("@relation supermercado\n\n");
	
	for (i = 0; i < nfreq; i++) {
		printf("@attribute %s {y,n}\n", product[i].name);
	}
	
	printf("\n@data\n");
	
	// Imprime cada tupla da parte dos dados do arquivo .arff
	for (i = 0; i < n; i++) {
		for (k = 0; k < nfreq; k++) {
			
			// Para cada produto k do conjunto de produtos, 
			// 	pesquisa se o item j da compra i e' esse produto k.
			// Se sim, imprime 1. Caso contrario, 0.
			found = 0;
			for (j = 0; j < purchase[i].total; j++) {
				if (strcmp(product[k].name, purchase[i].item[j]) == 0) {
					found = 1;
					break;
				}
			}
			
			if (found) {
				printf("y");
			} else {
				printf("?");
			}
		
			if (k != nfreq - 1) {
				printf(",");
			}
		}
		printf("\n");
	}

	// Liberando memoria alocada...
	if (line != NULL) {
		free(line);
	}
	if (purchase != NULL) {
		for (i = 0; i < n; i++) {
			if (purchase[i].item != NULL) {
				for (j = 0; j < purchase[i].total; j++) {
					if (purchase[i].item[j] != NULL) {
						free(purchase[i].item[j]);
					}
				}
				free(purchase[i].item);
			}
		}
		free(purchase);
	}
	if (product != NULL) {
		for (i = 0; i < count; i++) {
			if (product[i].name != NULL) {
				free(product[i].name);
			}
		}
		free(product);
	}
	if (name != NULL) {
		free(name);
	}
	
	return EXIT_SUCCESS;
}
