#ifndef _INDEX_H_
#define _INDEX_H_

/**
 * Estrutura de dados para guardar um registro de indice
 */
typedef struct index_s {
	char *value;
	char *offset;
} index_t;

/**
 * Ordena os indices em memoria (quicksort recursivo) de acordo com o valor do campo
 */
void index_sort(index_t **idx, int left, int right);

/**
 * Atualiza um arquivo de indice de chave primaria
 */
int update_primary_index(const char *file_name, const char *value, int size, int offset);

/**
 * Confere se existe um registro em um arquivo de indice
 */
int index_exists(const char *file_name, const char *key, int size);

/**
 * Procura e retorna um registro de um arquivo de indice
 */
index_t *index_get(const char *file_name, const char *value, int size, int *offset);

/**
 * Atualiza um arquivo de indice de chave secundaria
 * e tambem seu respectivo arquivo de lista invertida
 */
void update_secondary_index(const char *file_name, const char *list_file_name, const char *value, int size, const char *key, int key_size);

#endif
