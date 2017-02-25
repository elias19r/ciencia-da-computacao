#ifndef _ATHLETE_MODEL_H_
#define _ATHLETE_MODEL_H_

/**
 * Estrutura de dados do atleta
 */
typedef struct athlete_s {
	char cpf[12];
	char name[31];
	char ra[11];
	char uni[31];
	char mod[31];
} athlete_t;

/**
 * Faz leitura dos dados de um atleta para dentro de uma estrutura de dados
 * Retorna 0 para sucesso, ou um valor diferente para erro em cada campo
 */
int athlete_read(athlete_t *a);

/**
 * Valida se um chave primaria eh unica
 * Para isso, faz a busca no arquivo de indices e confere se retornou um offset invalido
 */
int athlete_valid_key(const char *key);

/**
 * Faz a insercao do atleta no arquivo de dados
 * Recebe a estrutura de dados e chama a funcao de insercao do banco de dados
 */
int athlete_insert(athlete_t *a);

/**
 * Atualiza todos os arquivos de indice
 */
void athlete_indexes(athlete_t *a, int offset);

/**
 * Remove um atleta do arquivo de dados
 */
int athlete_delete(const char *key);

/**
 * Pesquisa por atletas no arquivo de dados
 * Retorna um vetor de atletas e retorna o tamanho em count (por parametro)
 */
athlete_t **athlete_search(const char *field, const char *value, int *count);

/**
 * Ordena (em ordem crescente de cpf) uma lista de atletas
 */
void athlete_sort(athlete_t **list, int left, int right);

/**
 * Valida se um chave primaria eh unica
 * Para isso, faz a busca no arquivo de indices com arvore-b e confere se retornou um offset invalido
 */
int athlete_valid_key_btree(const char *key);

/**
 * Atualiza o indice do arquivo de arvore-b
 */
void athlete_indexes_btree(athlete_t *a, int offset);

#endif
