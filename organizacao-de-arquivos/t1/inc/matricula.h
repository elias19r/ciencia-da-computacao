#ifndef _MATRICULA_H_
#define _MATRICULA_H_

// Definições de tamanhos dos campos
#define MATRICULA_NUMERO_SIZE (8 * sizeof(char))
#define MATRICULA_CODIGO_SIZE (8 * sizeof(char))

// Definições de tamanhos da estrutura e do registro em arquivo
#define MATRICULA_STRUCT_SIZE sizeof(Matricula)
#define MATRICULA_RECORD_SIZE (18 * sizeof(char))

// A estrutura de dados para a matrícula
typedef struct {
	char numero[8];
	char codigo[8];
} Matricula;

/**
 * Inicializa uma estrutura de matrícula
 */
void matricula_new(Matricula *m);

/**
 * Define os dados de uma estrutra de matrícula
 */
void matricula_set(Matricula *m);

/**
 * Escreve uma matrícula no arquivo de dados (ordenadamente pelo número do aluno)
 */
void matricula_write(FILE *fp, Matricula *m);

/**
 * Ordena o arquivo de dados das matrículas:
 * Pelo número do aluno (crescente) e depois pelo código da disciplina (crecente)
 */
void matricula_sort(FILE *fp);

/**
 * Imprime na tela o arquivo de dados das matrículas
 */
void matricula_dump(FILE *fp);

#endif
