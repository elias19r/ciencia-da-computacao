#ifndef _DISCIPLINA_H_
#define _DISCIPLINA_H_

// Definições dos tamanhos dos campos
#define DISCIPLINA_CODIGO_SIZE    (8  * sizeof(char))
#define DISCIPLINA_NOME_SIZE      (31 * sizeof(char))
#define DISCIPLINA_PROFESSOR_SIZE (31 * sizeof(char))

// Definições dos tamanhos da estrutura e do registro em arquivo
#define DISCIPLINA_STRUCT_SIZE    sizeof(Disciplina)
#define DISCIPLINA_RECORD_SIZE    sizeof(Disciplina)

// A estrutura de dados para a disciplina
typedef struct {
	char codigo[8];
	char nome[31];
	char professor[31];
} Disciplina;

/**
 * Inicializa uma estrutura de disciplina
 */
void disciplina_new(Disciplina *d);

/**
 * Define os dados de uma estrutra de disciplina
 */
void disciplina_set(Disciplina *d);

/**
 * Escreve a estrutura de uma disciplina no arquivo de dados (ordenadamente pelo código)
 */
void disciplina_write(FILE *fp, Disciplina *d);

/**
 * Pesquisa por uma disciplina no arquivo de dados
 * Se encontrado: retorna 1. Senão: 0
 */
int disciplina_search(FILE *fp, const char *codigo);

/**
 * Imprime na tela o arquivo de dados das disciplinas
 */
void disciplina_dump(FILE *fp);

#endif
