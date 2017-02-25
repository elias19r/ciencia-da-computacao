#include <stdio.h>
#include <string.h>
#include "listaestatica.h"

void criarLista(TipoLista *L) {
	L->Ultimo = -1;
}

Apontador pesquisarLista(TipoLista *L, TipoChave C) {
	Apontador i;

	for (i = 0; i <= L->Ultimo; i++)
		if (strcmp(L->Item[i].Chave, C) == 0)
			return i;

	return -1;
}

int inserirUltimoLista(TipoLista *L, TipoItem I) {
	if (L->Ultimo == MAXTAM-1) {
		return 0;
	} else {
		if (pesquisarLista(L, I.Chave) != -1) {
			return 0;
		} else {
			L->Ultimo++;
			L->Item[L->Ultimo] = I;
			return 1;
		}
	}
}

int removerLista(TipoLista *L, Apontador P) {
	Apontador i;

	if (P < 0 || P > L->Ultimo)
		return 0;
	else {
		for (i = P; i <= L->Ultimo-1; i++)
			L->Item[i] = L->Item[i+1];
		L->Ultimo--;
		return 1;
	}
}

int recuperarLista(TipoLista *L, Apontador P, TipoItem *I) {
	if (P < 0 || P > L->Ultimo)
		return 0;
	else
		*I = L->Item[P];
	return 1;
}

int inserirListaPosicao(TipoLista *L, Apontador P, TipoItem I) {
	int i;

	if (L->Ultimo == MAXTAM-1)
		return 0;
	else
		if (P < 0 || P > L->Ultimo + 1)
			return 0;
		else {
			if (pesquisarLista(L, I.Chave) != -1) {
				return 0;
			} else {
				for (i = L->Ultimo; i >= P; i--)
					L->Item[i+1] = L->Item[i];
				L->Item[P] = I;
				L->Ultimo++;
				return 1;
			}
		}
}

int contaLista(TipoLista *L) {
	return L->Ultimo+1;
}

int listaCheia(TipoLista *L) {
	return L->Ultimo == MAXTAM-1;
}

int listaVazia(TipoLista *L) {
	return L->Ultimo == 0;
}
