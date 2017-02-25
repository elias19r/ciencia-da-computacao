/**
 * projeto01
 * 2012/08/21
 * Elias Italiano Rodrigues, 7987251, AED1, BCC-A, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>
#include "conjunto.h"

/**
 * A função lê e insere as cartas no conjunto C e as cartas repetidas no conjunto C_repetidas
 *
 * @return int quantidade de cartas no conjunto C_repetidas
 */
int lerCartas(int C_qtde, conjunto *C, conjunto *C_repetidas) {
	int i;
	elemento e;
	
	for (i = 0; i < C_qtde; i++) {
		scanf("%d", &e);
		if (pertence(*C, e)) {
			inserir(*C_repetidas, e);
		} else {
			inserir(*C, e);
		}
	}
	return tamanho(*C_repetidas);
}

/**
 * A função recebe dois conjuntos A e B e suas repetidas em A_repetidas e B_repetidos
 *
 * Fazendo a diferenca entre B_repetidos e A, obtem-se o conjunto das cartas que A pode receber
 * Fazendo a diferenca entre A_repetidos e B, obtem-se o conjunto das cartas que B pode receber
 * O tamanho do menor desses conjuntos será o quantidade de trocas possíveis
 *
 * @return int quantidade de trocas possíveis
 */
int qtdeTrocas(conjunto *A, conjunto *A_repetidas, int A_qtde, int A_qtde_repetidas, conjunto *B, conjunto *B_repetidas, int B_qtde, int B_qtde_repetidas) {
	conjunto C;
	int A_recebe, B_recebe;
		
	if (!(A_qtde_repetidas && B_qtde_repetidas))
		return 0;
	
	diferenca(C, *B_repetidas, *A);
	A_recebe = tamanho(C);
	diferenca(C, *A_repetidas, *B);
	B_recebe = tamanho(C);

	return A_recebe < B_recebe ? A_recebe : B_recebe;
}

int main(int argc, char **argv) {
	int M_qtde, M_qtde_repetidas, L_qtde, L_qtde_repetidas;
	conjunto *M, *M_repetidas, *L, *L_repetidas;
	
	scanf("%d %d", &M_qtde, &L_qtde); while (getchar() != '\n');
	
	if ((M_qtde < 0 || M_qtde > 1000000) || (L_qtde < 0 || L_qtde > 1000000)) {
		printf("Ocorreu um erro. A quantidade de cartas deve estar no intervalo [0, 1.000.000]\n");
		return 0;
	}
	
	M = (conjunto *) malloc(sizeof(conjunto));
	L = (conjunto *) malloc(sizeof(conjunto));
	M_repetidas = (conjunto *) malloc(sizeof(conjunto));
	L_repetidas = (conjunto *) malloc(sizeof(conjunto));
	
	criar(*M);
	criar(*L);
	criar(*M_repetidas);
	criar(*L_repetidas);

	M_qtde_repetidas = lerCartas(M_qtde, M, M_repetidas);
	L_qtde_repetidas = lerCartas(L_qtde, L, L_repetidas);
	
	printf("%d\n", qtdeTrocas(M, M_repetidas, M_qtde, M_qtde_repetidas, L, L_repetidas, L_qtde, L_qtde_repetidas));
	
	free(M);
	free(L);
	free(L_repetidas);
	free(M_repetidas);

	return 0;
}
