#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <myfunctions.h>

// Inclui os headers dos TADs
#include <aluno.h>
#include <disciplina.h>
#include <matricula.h>

/**
 * Função principal
 */
int main(int argc, char *argv[]) {
	// String para leitura dos comandos
	char *cmd;
	
	// Abertura dos arquivos (apenas uma vez)
	FILE *fpa = fopen("file/alunos.dat", "wb+");
	FILE *fpd = fopen("file/disciplina.dat", "wb+");
	FILE *fpm = fopen("file/matriculas.txt", "w+");
	
	while (1) {
		// Recebe o comando
		cmd = scan_str();
		
		// Comando "sair"
		if (strcmp(cmd, "sair") == 0) {
			free(cmd);
			break;
		
		// Comando "cadastrar aluno"
		} else if (strcmp(cmd, "cadastrar aluno") == 0) {
			Aluno *a = (Aluno *) calloc(1, sizeof(Aluno));
			
			// Inicializa, define e cadastra o aluno no arquivos de dados
			aluno_new(a);
			aluno_set(a);
			aluno_write(fpa, a);
			
			free(a);
		
		// Comando "cadastrar disciplina"
		} else if (strcmp(cmd, "cadastrar disciplina") == 0) {
			Disciplina *d = (Disciplina *) calloc(1, sizeof(Disciplina));
			
			// Inicializa, define e cadastra a disciplina no arquivos de dados
			disciplina_new(d);
			disciplina_set(d);
			disciplina_write(fpd, d);
			
			free(d);
		
		// Comando "matricula"
		} else if (strcmp(cmd, "matricular") == 0) {
			Matricula *m = (Matricula *) calloc(1, sizeof(Matricula));
			
			// Inicializa e define a matrícula
			matricula_new(m);
			matricula_set(m);
			
			// Pesquisa se o aluno e a disciplina existem
			if (aluno_search(fpa, m->numero) && disciplina_search(fpd, m->codigo)) {
				// Cadastra a matrícula e ordena o arquivo de dados
				matricula_write(fpm, m);
			}
			
			free(m);
		
		// Comando "dump aluno"
		} else if (strcmp(cmd, "dump aluno") == 0) {
			// Imprime arquivo de dados dos alunos
			aluno_dump(fpa);
		
		// Comando "dump disciplina"
		} else if (strcmp(cmd, "dump disciplina") == 0) {
			// Imprime o arquivo de dados das disciplinas
			disciplina_dump(fpd);

		// Comando "dump matricula"
		} else if (strcmp(cmd, "dump matricula") == 0) {
			// Imprime o arquivo de dados das matrículas
			matricula_dump(fpm);

		}

		free(cmd);
	}

	// Fecha os arquivos de dados	
	fclose(fpa);
	fclose(fpd);
	fclose(fpm);
	
	return 0;
}
