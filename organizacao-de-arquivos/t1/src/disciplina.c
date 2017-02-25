#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <myfunctions.h>

// Inclui header
#include <disciplina.h>

/**
 * Inicializa uma estrutura de disciplina
 */
void disciplina_new(Disciplina *d) {
	memset(d, 0, DISCIPLINA_STRUCT_SIZE);
}

/**
 * Define os dados de uma estrutra de disciplina
 */
void disciplina_set(Disciplina *d) {
	// Lê dados
	char *nome = scan_str();
	char *codigo = scan_str();
	char *professor = scan_str();
	
	// Salva na estrutura da disciplina
	strncpy(d->nome, nome, DISCIPLINA_NOME_SIZE);
	strncpy(d->codigo, codigo, DISCIPLINA_CODIGO_SIZE);
	strncpy(d->professor, professor, DISCIPLINA_PROFESSOR_SIZE);
	
	free(nome);
	free(codigo);
	free(professor);
}

/**
 * Escreve a estrutura de uma disciplina no arquivo de dados (ordenadamente pelo código)
 */
void disciplina_write(FILE *fp, Disciplina *d) {
	int start, end, central, pos;
	long n;
	// Estrutura auxiliar
	Disciplina *aux = (Disciplina *) calloc(1, DISCIPLINA_RECORD_SIZE);
	
	// Calcula o total de registros e salva e n	
	fseek(fp, 0, SEEK_END);
	n = ftell(fp);
	n /= DISCIPLINA_RECORD_SIZE;

	// Define o começo e fim
	start = 0;
	end = n-1;

	// Encontra a posição correta para inserir o disciplina no arquivo
	pos = 0;
	while (start <= end) {
		central = (start + end) / 2;

		// Leitura do registro		
		fseek(fp, central * DISCIPLINA_RECORD_SIZE, SEEK_SET);
		fread(aux, DISCIPLINA_RECORD_SIZE, 1, fp);
		
		// Comparação dos números
		if (strcmp(aux->codigo, d->codigo) == 0) {
			pos = -1;
			break;
		} else {
			if (strcmp(d->codigo, aux->codigo) > 0) {
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
		// Insere a disciplina no final do arquivo
		fseek(fp, 0, SEEK_END);
		fwrite(d, DISCIPLINA_RECORD_SIZE, 1, fp);
		n++;

		// Vai fazendo "shift" dela até a sua posição correta
		if (n > 1) {
			int i;

			for (i = n-2; i >= pos; i--) {
				fseek(fp, i * DISCIPLINA_RECORD_SIZE, SEEK_SET);
				fread(aux, DISCIPLINA_RECORD_SIZE, 1, fp);
				fwrite(aux, DISCIPLINA_RECORD_SIZE, 1, fp);
			
				fseek(fp, i * DISCIPLINA_RECORD_SIZE, SEEK_SET);
				fwrite(d, DISCIPLINA_RECORD_SIZE, 1, fp);
			}
		}
	}
	
	free(aux);
	fflush(fp);
}

/**
 * Pesquisa por uma disciplina no arquivo de dados
 * Se encontrado: retorna 1. Senão: 0
 * Algoritmo de busca binária iterativa em arquivo
 */
int disciplina_search(FILE *fp, const char *codigo) {
	int start, end, central, found;
	long n;
	Disciplina *key = (Disciplina *) calloc(1, DISCIPLINA_RECORD_SIZE);
	
	// Calcula o total de registros e salva e n	
	fseek(fp, 0, SEEK_END);
	n = ftell(fp);
	n /= DISCIPLINA_RECORD_SIZE;

	// Define começo e fim
	start = 0;
	end = n-1;

	found = 0;
	while (start <= end) {
		central = (start + end) / 2;
		
		// Leitura do registro
		fseek(fp, central * DISCIPLINA_RECORD_SIZE, SEEK_SET);
		fread(key, DISCIPLINA_RECORD_SIZE, 1, fp);
		
		// Comparação dos códigos
		if (strcmp(key->codigo, codigo) == 0) {
			found = 1;
			break;
		} else {
			if (strcmp(codigo, key->codigo) > 0) {
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
 * Imprime na tela o arquivo de dados das disciplinas
 */
void disciplina_dump(FILE *fp) {
	int i;
	long n;
	
	// Estrutura auxiliar
	Disciplina *d = (Disciplina *) calloc(1, DISCIPLINA_RECORD_SIZE);

	// Calcula o total de registros e salva e n		
	fseek(fp, 0, SEEK_END);
	n = ftell(fp);
	n /= DISCIPLINA_RECORD_SIZE;
	
	// Laço para leitura e impressão dos dados
	for (i = n-1; i >= 0; i--) {
		fseek(fp, i * DISCIPLINA_RECORD_SIZE, SEEK_SET);
		fread(d, DISCIPLINA_RECORD_SIZE, 1, fp);
		
		printf("%s - %s - %s\n", d->codigo, d->nome, d->professor);
	}
	
	free(d);
}
