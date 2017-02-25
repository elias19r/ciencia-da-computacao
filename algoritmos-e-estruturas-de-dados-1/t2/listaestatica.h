#ifndef LISTAESTATICA_H

#define LISTAESTATICA_H
#define MAXTAM 1000

#include <stdio.h>
#include <string.h>
#include "listaestatica.h"

typedef int Apontador;
typedef char TipoChave[51];

typedef struct{
	TipoChave Chave;
} TipoItem;

typedef struct{
	TipoItem Item[MAXTAM];
	Apontador Ultimo;
} TipoLista;

/**
 * Cria a lista resetando ela (ultimo = -1)
 */
void criarLista(TipoLista *L);

/**
 * Pesquisa um item na lista a partir de uma chave
 * @return int -1 para não encontrado, um apontador para o item quando encontrado
 */
Apontador pesquisarLista(TipoLista *L, TipoChave C);

/**
 * Insere um novo item na lista, desde que não haja um item com a mesma chave
 * @return int 0 para erro, 1 para sucesso
 */
int inserirUltimoLista(TipoLista *L, TipoItem I);

/**
 * Passada uma posição, remove o item da lista
 * @return int 0 para erro, 1 para sucesso
 */
int removerLista(TipoLista *L, Apontador P);

/**
 * Recupera um item da lista em I a partir de uma apontador P
 * @return int 0 para erro, 1 para sucesso
 */
int recuperarLista(TipoLista *L, Apontador P, TipoItem *I);

/**
 * Insere um item na lista na posição informada
 * @return int 0 para erro, 1 para sucesso
 */
int inserirListaPosicao(TipoLista *L, Apontador P, TipoItem I);

/**
 * Quantidade de itens na lista
 * @return int quantidade de itens na lista
 */
int contaLista(TipoLista *L);

/**
 * Confere se a lista está cheia
 * @return int 0 para não cheia, 1 para cheia
 */
int listaCheia(TipoLista *L);

/**
 * Confere se a lista está vazia
 * @return int 1 para vazia, 0 para não vazia
 */
int listaVazia(TipoLista *L);

#endif
