#ifndef _ALUNO_H_
#define _ALUNO_H_

// Definições de tamanhos dos campos
#define ALUNO_NUMERO_SIZE (8  * sizeof(char))
#define ALUNO_NOME_SIZE   (31 * sizeof(char))
#define ALUNO_ANO_SIZE    sizeof(int)

// Definições de tamanhos da estrutura e do registro em arquivo
#define ALUNO_STRUCT_SIZE sizeof(Aluno)
#define ALUNO_RECORD_SIZE sizeof(Aluno)

// A estrutura de dados para o aluno
typedef struct {
	char numero[8];
	char nome[31];
	int ano;
} Aluno;

/**
 * Inicializa uma estrutura de aluno
 */
void aluno_new(Aluno *a);

/**
 * Define os dados de uma estrutra de aluno
 */
void aluno_set(Aluno *a);

/**
 * Escreve a estrutura de um aluno no arquivo de dados (ordenadamente pelo número)
 */
void aluno_write(FILE *fp, Aluno *a);

/**
 * Pesquisa por um aluno no arquivo de dados
 * Se encontrado: retorna 1. Senão: 0
 */
int aluno_search(FILE *fp, const char *numero);

/**
 * Imprime na tela o arquivo de dados dos alunos
 */
void aluno_dump(FILE *fp);

#endif
