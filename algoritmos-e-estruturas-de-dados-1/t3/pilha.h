#ifndef _PILHA_H_
#define _PILHA_H_

typedef int TipoItem;
typedef int Apontador;

typedef struct {
	TipoItem *itens;
	Apontador topo;
	int n;
} TipoPilha;

void criar(TipoPilha *P, int n);
void esvaziar(TipoPilha *P);
int vazia(TipoPilha P);
int contar(TipoPilha P);
int cheia(TipoPilha P);
TipoItem topo(TipoPilha P);
int push(TipoPilha *P, TipoItem I);
TipoItem pop(TipoPilha *P);
void imprimir(TipoPilha P);

#endif
