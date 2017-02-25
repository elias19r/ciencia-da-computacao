#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

void criar(TipoPilha *P, int n) {
	P->topo = -1;
	P->n = n;
	P->itens = (TipoItem *) malloc(sizeof(TipoItem) * n);
}

void esvaziar(TipoPilha *P) {
	P->topo = -1;
}

int vazia(TipoPilha P) {
	return P.topo == -1;
}

int contar(TipoPilha P) {
	return P.topo + 1;
}

int cheia(TipoPilha P) {
	return P.topo == (P.n - 1);
}

TipoItem topo(TipoPilha P) {
	return P.itens[P.topo];
}

int push(TipoPilha *P, TipoItem I) {

	if (!cheia(*P)) {
		P->topo++;
		P->itens[P->topo] = I;
		return 1;
	}
	return 0;
}

TipoItem pop(TipoPilha *P) {
	if(!vazia(*P)) {
		P->topo--;
		return P->itens[P->topo+1];
	}
	return 0;
}

void imprimir(TipoPilha P) {
	int i;

	for (i = P.topo; i >= 0; i--){
		printf("%3d ", P.itens[i]);
	}
	printf("\n");
}
