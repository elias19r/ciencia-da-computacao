/**
 * projeto03
 * 2012/10/09
 * Elias Italiano Rodrigues, 7987251, AED1, BCC-A, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pilha.h" // incluindo TAD Pilha

// Definindo valor numérico para as operações
#define NUM 0
#define POP 1
#define INV 2
#define DUP 3
#define SWP 4
#define ADD 5
#define SUB 6
#define MUL 7
#define DIV 8
#define MOD 9

int main(int argc, char **argv) {

	char leitura[12];
	int  P, N, valor, i, j, erro;
	int **programa = NULL;
	int *v = NULL;
	TipoPilha pilha;
	TipoItem item1, item2;

	// Criação da pilha
	criar(&pilha, 100000);

	scanf("%s", leitura);
	while (strcmp(leitura, "QUIT") != 0) {
		P = 0;

		// Leitura das operações
		while (strcmp(leitura, "END") != 0) {
			programa = (int **) realloc(programa, sizeof(int *) * (P + 1));
			programa[P++] = (int *) malloc(sizeof(int) * 2);

			if (strcmp(leitura, "NUM") == 0) {
				scanf("%d", &valor);
				programa[P-1][0] = NUM;
				programa[P-1][1] = valor;
			} else if (strcmp(leitura, "POP") == 0) {
				programa[P-1][0] = POP;
			} else if (strcmp(leitura, "INV") == 0) {
				programa[P-1][0] = INV;
			} else if (strcmp(leitura, "DUP") == 0) {
				programa[P-1][0] = DUP;
			} else if (strcmp(leitura, "SWP") == 0) {
				programa[P-1][0] = SWP;
			} else if (strcmp(leitura, "ADD") == 0) {
				programa[P-1][0] = ADD;
			} else if (strcmp(leitura, "SUB") == 0) {
				programa[P-1][0] = SUB;
			} else if (strcmp(leitura, "MUL") == 0) {
				programa[P-1][0] = MUL;
			} else if (strcmp(leitura, "DIV") == 0) {
				programa[P-1][0] = DIV;
			} else if (strcmp(leitura, "MOD") == 0) {
				programa[P-1][0] = MOD;
			}

			scanf("%s", leitura);
		}

		// Leitura de N
		scanf("%s", leitura);
		N = atoi(leitura);
		v = (int *) malloc(sizeof(int) * N);

		// Leitura dos N valores
		for (i = 0; i < N; i++) {
			scanf("%s", leitura);
			v[i] = atoi(leitura);
		}

		// Execução da máquina para cada valor de entrada v
		for (i = 0; i < N; i++) {
			erro = 0;
			esvaziar(&pilha);
			push(&pilha, v[i]); // inserindo o primeiro valor na pilha
					
			// Conferindo e executando cada operação
			for (j = 0; j < P; j++) {

				switch (programa[j][0]) {
					case NUM:
						if (!push(&pilha, programa[j][1]))
							erro = 1;
					break;

					case POP:
						if (!pop(&pilha))
							erro = 1;
					break;

					case INV:
						if (contar(pilha) > 0) {
							item1 = pop(&pilha);
							push(&pilha, -item1);
						} else
							erro = 1;
					break;

					case DUP:
						if (contar(pilha) > 0) {
							item1 = topo(pilha);
							push(&pilha, item1);
						} else
							erro = 1;
					break;

					case SWP:
						if (contar(pilha) > 1) {
							item1 = pop(&pilha);
							item2 = pop(&pilha);
							push(&pilha, item1);
							push(&pilha, item2);
						} else
							erro = 1;
					break;

					case ADD:
						if (contar(pilha) > 1) {
							item1 = pop(&pilha);
							item2 = pop(&pilha);
							push(&pilha, item1 + item2);
						} else
							erro = 1;
					break;

					case SUB:
						if (contar(pilha) > 1) {
							item1 = pop(&pilha);
							item2 = pop(&pilha);
							push(&pilha, item1 - item2);
						} else
							erro = 1;
					break;

					case MUL:
						if (contar(pilha) > 1) {
							item1 = pop(&pilha);
							item2 = pop(&pilha);
							push(&pilha, item1 * item2);
						} else
							erro = 1;
					break;

					case DIV:
						if (contar(pilha) > 1) {
							item1 = pop(&pilha);
							if (item1 != 0) {
								item2 = pop(&pilha);
								push(&pilha, item2 / item1);
							} else
								erro = 1;
						} else
							erro = 1;
					break;

					case MOD:
						if (contar(pilha) > 1) {
							item1 = pop(&pilha);
							item2 = pop(&pilha);
							push(&pilha, item2 % item1);
						} else
							erro = 1;
					break;

					default: erro = 1;
				}
			}
			
			// Conferindo as situações de erro e imprimindo resultado 
			if (erro || contar(pilha) != 1 || topo(pilha) > 1000000000 || topo(pilha) < -1000000000)
				printf("ERROR\n");
			else
				printf("%d\n", topo(pilha));
		}
		printf("\n");

		// Liberação de memória
		for (i = 0; i < P; i++)
			free(programa[i]);
		free(programa);
		free(v);
		programa = NULL;
		v = NULL;

		scanf("%s", leitura);
	}
	
	// Liberando itens da pilha, pois foram alocados na memória Heap
	free(pilha.itens);

	return 0;
}
