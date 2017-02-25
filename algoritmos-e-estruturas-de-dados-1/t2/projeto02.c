/**
 * projeto02
 * 2012/09/12
 * Elias Italiano Rodrigues, 7987251, AED1, BCC-A, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "listaestatica.h"

/**
 * Remove o '\n' do final de uma string
 * Usado para remover '\n' que o fgets captura quando lê uma string
 */
void removerLn(char *str)
{
	int i = strlen(str) - 1;
	if (str[i] == '\n')
		str[i] = '\0';
}

/**
 * Imprime os alunos das disciplinas. Cada disciplina em uma linha.
 */
void imprimirDisciplinas(TipoLista *L, int count) {
	int i, j;

	for (i = 0; i <= count; i++) {
		for (j = 0; j <= L[i].Ultimo; j++) {
			printf("%s ", L[i].Item[j].Chave);
		}
		printf("\n");
	}
}

int main(int argc, char **argv) {
	TipoLista *L, *L_expulsos;
	TipoItem I;
	TipoChave C;
	int i, count = -1;
	
	// Aloca 1000 listas para as disciplinas
	L = (TipoLista *) malloc(sizeof(TipoLista) * 1000);

	// Aloca a lista de expulsos
	L_expulsos = (TipoLista *) malloc(sizeof(TipoLista));
	
	do {
		fgets(C, 50, stdin); // leitura da chave
		removerLn(C);
		
		if (isupper(C[0])) {
			if (strcmp(C, "FIM") == 0) {
				imprimirDisciplinas(L, count);
				printf("\n");
				// Encerra o semestre, resetando as disciplinas
				//     e resetando a lista de expulsos
				count = -1;
				criarLista(L_expulsos);
				continue;
			} else {
				count++;
				criarLista(&L[count]); // inicializa a lista para uma disciplina
			}
		} else {
			if (C[0] == '\0')
				continue;
			
			Apontador P;
			TipoItem I_expulso;
			int expulso = 0;
			// Confere se o aluno já está em alguma disciplina anterior.
			// Em caso afirmativo, ele é excluído dessa disciplina e expulso
			//     da universidade, não sendo permitido que ele seja matriculado
			//     nas próximas disciplinas.
			for (i = 0; i < count; i++) {
				P = pesquisarLista(&L[i], C);
				if (P != -1) {
					removerLista(&L[i], P);
					expulso = 1;
				}
			}
			if (expulso) {
				strcpy(I_expulso.Chave, C);
				inserirUltimoLista(L_expulsos, I_expulso); // insere na lista de expulsos
			}			
			if (pesquisarLista(L_expulsos, C) == -1) {
				strcpy(I.Chave, C);
				inserirUltimoLista(&L[count], I);
			}
		}
	} while (strcmp(C, "TERMINA") != 0);

	imprimirDisciplinas(L, count);
	
	return 0;
}
