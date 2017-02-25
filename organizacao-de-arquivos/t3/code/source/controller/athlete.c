#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/model/athlete.h"
#include "../../include/view/athlete.h"
#include "../../include/controller/athlete.h"

/**
 * Acao "cadastrar" um novo atleta
 * Faz leitura dos dados e valida a chave primaria
 * Se tudo OK, manda inserir e atualizar os indices
 */
int athlete_action_insert() {
	// Aloca um atleta
	athlete_t *a = (athlete_t *) malloc(sizeof(athlete_t));
	
	// Faz leitura dos dados
	if (athlete_read(a) == 0) {

		// Confere se a chave eh valida
		if (athlete_valid_key(a->cpf)) {
			// Insere no arquivo de dados
			int offset = athlete_insert(a);
		
			if (offset > -1) {
				// Atualiza os indexes (primario e secundario)
				athlete_indexes(a, offset);
				free(a);
				return 1;
			}
		} else {
			free(a);
			printf("Conflito de chave primaria. Registro nao inserido!\n");
			return 0;
		}
	}

	free(a);
	return 0;
}

/**
 * Acao "remover" um atleta
 * Apenas executa a remocao invocando a funcao do model
 */
int athlete_action_delete(const char *cpf) {
	return athlete_delete(cpf);
}

/**
 * Acao "buscar" por um atleta
 * Recebe o campo e o valor e manda buscar
 * Se encontrou resultados, imprime
 */
int athlete_action_search(const char *field, const char *value) {
	athlete_t **res = NULL;
	int count = 0;

	// Faz a busca	
	res = athlete_search(field, value, &count);

	// Se encontrou algo, ordena e imprime os resultados
	if (res != NULL && count > 0) {
		athlete_sort(res, 0, count-1);
		athlete_print_results(res, count);
		
		// Liberacao de memoria
		while (count != 0)
			free(res[--count]);
		free(res);

		return 1;
	}

	return 0;
}

/**
 * Acao "buscar" por atletas combinada com E/OU
 * Recebe os campos e os valores e manda buscar
 * Se encontrou resultados, imprime
 */
int athlete_action_search_combined(const char *field1, const char *value1, const char *op, const char *field2, const char *value2) {
	athlete_t **res1 = NULL, **res2 = NULL, **res = NULL;
	int count1 = 0, count2 = 0, count = 0, ret = 0, i, j;

	// Primeira busca
	res1 = athlete_search(field1, value1, &count1);
	
	// Segunda busca
	res2 = athlete_search(field2, value2, &count2);

	// Ordena resultados da primeira busca
	if (res1 != NULL && count1 > 0)
		athlete_sort(res1, 0, count1-1);
	
	// Ordena resultados da segunda busca
	if (res2 != NULL && count2 > 0)
		athlete_sort(res2, 0, count2-1);

	// Processamento cosequencial: faz intersecao dos dois resultados (matching)
	if (strcmp(op, "e") == 0 || strcmp(op, "E") == 0) {
		i = j = 0;
		
		// Equanto houver itens nas duas listas, faca
		while (i < count1 && j < count2) {
			// item1 == item2
			if (strcmp(res1[i]->cpf, res2[j]->cpf) == 0) {
				res = (athlete_t **) realloc(res, sizeof(athlete_t *) * (count + 1));
				res[count] = res1[i];
				++count;
				++i;
				++j;
			// item1 < item2
			} else if (strcmp(res1[i]->cpf, res2[j]->cpf) < 0) {
				++i;
			// item1 > item2
			} else {
				++j;
			}
		}

	// Processamento cosequencial: faz intercalacao dos resultados (merging)
	} else if (strcmp(op, "ou") == 0 || strcmp(op, "OU") == 0) {
			i = j = 0;

			// Enquanto houver item em uma das listas, faca
			while (i < count1 || j < count2) {

				// Se ainda nao acabou nenhuma das listas
				if (i < count1 && j < count2) {
					// item1 == item2
					if (strcmp(res1[i]->cpf, res2[j]->cpf) == 0) {
						res = (athlete_t **) realloc(res, sizeof(athlete_t *) * (count + 1));
						res[count] = res1[i];
						++count;
						++i;
						++j;
					// item1 < item2
					} else if (strcmp(res1[i]->cpf, res2[j]->cpf) < 0) {
						res = (athlete_t **) realloc(res, sizeof(athlete_t *) * (count + 1));
						res[count] = res1[i];
						++count;
						++i;
					// item1 > item2
					} else {
						res = (athlete_t **) realloc(res, sizeof(athlete_t *) * (count + 1));
						res[count] = res2[j];
						++count;
						++j;
					}

				// Se acabou a lista 2, vai pegando os restantes da lista 1
				} else if (i < count1) {
					res = (athlete_t **) realloc(res, sizeof(athlete_t *) * (count + 1));
					res[count] = res1[i];
					++count;
					++i;

				// Se acabou a lista 1, vai pegando os restantes da lista 2
				} else if (j < count2) {
					res = (athlete_t **) realloc(res, sizeof(athlete_t *) * (count + 1));
					res[count] = res2[j];
					++count;
					++j;
				}
			}
	}
	
	// Se obteve algum resultado, imprime
	if (res != NULL && count > 0) {
		athlete_print_results(res, count);
		ret = 1;
		
		// Liberacao de memoria
		free(res);
	}

	// Liberacao de memoria
	if (res1 != NULL) {
		while (count1 != 0)
			free(res1[--count1]);
		free(res1);
	}
	if (res2 != NULL) {
		while (count2 != 0)
			free(res2[--count2]);
		free(res2);
	}	

	return ret;
}

/**
 * Acao "dump <arquivo>"
 * Apenas imprime o arquivo na tela, invocando a funcao da view
 */
void athlete_action_dump(const char *file_name) {
	athlete_print_file(file_name);
}

