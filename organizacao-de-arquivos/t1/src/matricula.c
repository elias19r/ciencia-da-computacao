#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <myfunctions.h>

// Inclui header
#include <matricula.h>

/**
 * Inicializa uma estrutura de matrícula
 */
void matricula_new(Matricula *m) {
	memset(m, 0, MATRICULA_STRUCT_SIZE);
}

/**
 * Define os dados de uma estrutra de matrícula
 */
void matricula_set(Matricula *m) {
	// Lê dados
	char *numero = scan_str();
	char *codigo = scan_str();

	// Salva na estrutura da matrícula
	strncpy(m->numero, numero, MATRICULA_NUMERO_SIZE);	
	strncpy(m->codigo, codigo, MATRICULA_CODIGO_SIZE);
	
	free(numero);
	free(codigo);
}

/**
 * Escreve uma matrícula no arquivo de dados.
 * Ordenadamente pelo número do aluno e depois pelo código da disciplina
 */
void matricula_write(FILE *fp, Matricula *m) {
	int i;
	long n;
	
	// Buffers auxiliares
	char aux_numero[8], aux_codigo[8];
	char aux[19];
	
	// Calcula o total de registros e salva e n
	fseek(fp, 0, SEEK_END);
	n = ftell(fp);
	n /= MATRICULA_RECORD_SIZE;

	// Escreve a matrícula no final do arquivo	
	fprintf(fp, "%-7s - %-7s\n", m->numero, m->codigo);
	
	// Vai "shiftando" a matrícula para sua posição correta
	for (i = n-1; i >= 0; i--) {
		fseek(fp, i * MATRICULA_RECORD_SIZE, SEEK_SET);
		fread(aux, MATRICULA_RECORD_SIZE, 1, fp);
		aux[18] = '\0';
		strncpy(aux_numero, aux, MATRICULA_NUMERO_SIZE-1);
		strncpy(aux_codigo, aux + MATRICULA_NUMERO_SIZE-1 + (3 * sizeof(char)), MATRICULA_CODIGO_SIZE-1);
		
		// Remove espaços em branco pois isso interfere na comparação
		remove_spaces(aux_numero);
		remove_spaces(aux_codigo);

		if (strcmp(aux_numero, m->numero) > 0) {
			fprintf(fp, "%s", aux);
			fseek(fp, i * MATRICULA_RECORD_SIZE, SEEK_SET);		
			fprintf(fp, "%-7s - %-7s\n", m->numero, m->codigo);
		
		// Se os números dos alunos são iguais, faz "shift" pelo código da disciplina
		} else if (strcmp(aux_numero, m->numero) == 0) {
			if (strcmp(aux_codigo, m->codigo) > 0) {
				fprintf(fp, "%s", aux);
				fseek(fp, i * MATRICULA_RECORD_SIZE, SEEK_SET);		
				fprintf(fp, "%-7s - %-7s\n", m->numero, m->codigo);				
			}
		} else {
			break;
		}
	}
	fflush(fp);
}

/**
 * Imprime na tela o arquivo de dados das matrículas
 */
void matricula_dump(FILE *fp) {
	// Buffer auxiliar
	char record[19];
	
	fseek(fp, 0, SEEK_SET);
	// Laço de leitura e impressão
	while (!feof(fp)) {
		fread(record, MATRICULA_RECORD_SIZE, 1, fp);
		record[18] = '\0';
		if (!feof(fp)) {
			printf("%s", record);
		}
	}
}
