#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <myfunctions.h>

// Inclui header
#include <aluno.h>

/**
 * Inicializa uma estrutura de aluno
 */
void aluno_new(Aluno *a) {
	memset(a, 0, ALUNO_STRUCT_SIZE);
}

/**
 * Define os dados de uma estrutra de aluno
 */
void aluno_set(Aluno *a) {
	// Lê dados
	char *nome = scan_str();
	char *numero = scan_str();
	
	// Salva na estrutura do aluno
	strncpy(a->nome, nome, ALUNO_NOME_SIZE);
	strncpy(a->numero, numero, ALUNO_NUMERO_SIZE);
	scanf("%d", &(a->ano));
	
	free(nome);
	free(numero);
}

/**
 * Escreve a estrutura de um aluno no arquivo de dados (ordenadamente pelo número)
 */
void aluno_write(FILE *fp, Aluno *a) {
	int start, end, central, pos;
	long n;
	// Estrutura auxiliar
	Aluno *aux = (Aluno *) calloc(1, ALUNO_RECORD_SIZE);
	
	// Calcula o total de registros e salva e n	
	fseek(fp, 0, SEEK_END);
	n = ftell(fp);
	n /= ALUNO_RECORD_SIZE;

	// Define o começo e fim
	start = 0;
	end = n-1;

	// Encontra a posição correta para inserir o aluno no arquivo
	pos = 0;
	while (start <= end) {
		central = (start + end) / 2;

		// Leitura do registro		
		fseek(fp, central * ALUNO_RECORD_SIZE, SEEK_SET);
		fread(aux, ALUNO_RECORD_SIZE, 1, fp);
		
		// Comparação dos números
		if (strcmp(aux->numero, a->numero) == 0) {
			pos = -1;
			break;
		} else {
			if (strcmp(a->numero, aux->numero) > 0) {
				start = central + 1;
				pos = start;
			} else {
				end = central - 1;
				pos = end;
			}
		}
	}

	// Se existe uma posição válida,
	if (pos != -1) {
		// Insere o aluno no final do arquivo
		fseek(fp, 0, SEEK_END);
		fwrite(a, ALUNO_RECORD_SIZE, 1, fp);
		n++;

		// Vai fazendo "shift" dele até a sua posição correta
		if (n > 1) {
			int i;

			for (i = n-2; i >= pos; i--) {
				fseek(fp, i * ALUNO_RECORD_SIZE, SEEK_SET);
				fread(aux, ALUNO_RECORD_SIZE, 1, fp);
				fwrite(aux, ALUNO_RECORD_SIZE, 1, fp);
			
				fseek(fp, i * ALUNO_RECORD_SIZE, SEEK_SET);
				fwrite(a, ALUNO_RECORD_SIZE, 1, fp);
			}
		}
	}
	
	free(aux);
	fflush(fp);
}

/**
 * Pesquisa por um aluno no arquivo de dados
 * Se encontrado: retorna 1. Senão: 0
 * Algoritmo de busca binária iterativa em arquivo
 */
int aluno_search(FILE *fp, const char *numero) {
	int start, end, central, found;
	long n;
	Aluno *key = (Aluno *) calloc(1, ALUNO_RECORD_SIZE);

	// Calcula o total de registros e salva e n	
	fseek(fp, 0, SEEK_END);
	n = ftell(fp);
	n /= ALUNO_RECORD_SIZE;

	// Define o começo e fim
	start = 0;
	end = n-1;

	found = 0;
	while (start <= end) {
		central = (start + end) / 2;

		// Leitura do registro		
		fseek(fp, central * ALUNO_RECORD_SIZE, SEEK_SET);
		fread(key, ALUNO_RECORD_SIZE, 1, fp);
		
		// Comparação dos números
		if (strcmp(key->numero, numero) == 0) {
			found = 1;
			break;
		} else {
			if (strcmp(numero, key->numero) > 0) {
				start = central + 1;
			} else {
				end = central - 1;
			}
		}
	}

	free(key);
	return found;
}

/**
 * Imprime na tela o arquivo de dados dos alunos
 */
void aluno_dump(FILE *fp) {
	int i;
	long n;
	
	// Estrutura auxiliar
	Aluno *a = (Aluno *) calloc(1, ALUNO_RECORD_SIZE);

	// Calcula o total de registros e salva e n
	fseek(fp, 0, SEEK_END);
	n = ftell(fp);
	n /= ALUNO_RECORD_SIZE;

	// Laço para leitura e impressão dos dados	
	for (i = n-1; i >= 0; i--) {
		fseek(fp, i * ALUNO_RECORD_SIZE, SEEK_SET);
		fread(a, ALUNO_RECORD_SIZE, 1, fp);
		printf("%s - %s - %d\n", a->numero, a->nome, a->ano);
	}
	
	free(a);
}
