#ifndef _GRAPH_H_
#define _GRAPH_H_

/**
 * Implementacao de grafo em C usando vetores
 * Elias Italiano Rodrigues, 7987251, Grafos, BCC, ICMC-USP
 * 2013/06
 */

#define _GRAPH_WHITE_ 0
#define _GRAPH_GRAY_  1
#define _GRAPH_BLACK_ 2
#define _GRAPH_INF_   10e21

struct vertex_s;
struct edge_s;

/**
 * Estrutura de dados do grafo
 */
typedef struct graph_s {
	int dir;                    // se eh direcionado
	struct vertex_s **vertices; // vetor de vertices
	int count;                  // tamanho do vetor de vertices
	int amount;                 // quantidade de vertices nao-nulos
} graph_t;

/**
 * Estrutura de dados do vertice
 */
typedef struct vertex_s {
	int index;             // indice do vertice
	char *label;           // rotulo para o vertice
	struct edge_s **edges; // vetor de arestas
	int count;             // tamanho do vetor de arestas
	int amount;            // quantidade de arestas nao-nulas
	struct vertex_s *pre;  // predecessor
	int color;             // cor
	double distance;       // distancia
	int frequence;         // frequencia
} vertex_t;

/**
 * Estrutura de dados da aresta
 */
typedef struct edge_s {
	struct vertex_s *to; // vertice para o qual a aresta aponta
	double weight;       // peso da aresta
} edge_t;

/**
 * Inicializa os valores de um grafo
 */
void graph_initialize(graph_t *g);

/**
 * Finaliza um grafo, liberando toda a memoria alocada
 */
void graph_finalize(graph_t *g);

/**
 * Cria um grafo com uma quantidade de vertices
 */
void graph_create(graph_t *g, int count);

/**
 * Adiciona um vertice no grafo
 */
int graph_add(graph_t *g, int v, const char *label);

/**
 * Adiciona vertices no grafo a partir de um arquivo
 */
int graph_add_from_file(graph_t *g, const char *filename);

/**
 * Altera informacoes de um vertice
 */
int graph_edit(graph_t *g, int v, int new_index, const char *new_label);

/**
 * Remove um vertice do grafo
 */
int graph_delete(graph_t *g, int v);

/**
 * Retorna o ponteiro de um vertice do grafo
 */
vertex_t *graph_get(graph_t *g, int v);

/**
 * Confere se um vertice existe no grafo
 */
int graph_exists(graph_t *g, int v);

/**
 * Retorna a quantidade de vertices no grafo
 */
int graph_amount(graph_t *g);

/**
 * Cria uma aresta, ligando dois vertices existentes
 */
int graph_link(graph_t *g, int a, int b, double weight);

/**
 * Cria arestas, ligando dois vertices existentes, a partir de um arquivo
 */
int graph_link_from_file(graph_t *g, const char *filename);

/**
 * Remove uma aresta entre dois vertices existentes
 */
int graph_unlink(graph_t *g, int a, int b);

/**
 * Confere se uma aresta existe no grafo
 */
int graph_exists_link(graph_t *g, int a, int b);

/**
 * Retorna um ponteiro de uma aresta do grafo
 */
edge_t *graph_get_link(graph_t *g, int a, int b);

/**
 * Atualiza o peso de uma aresta do grafo
 */
int graph_weight(graph_t *g, int a, int b, double weight);

/**
 * Retorna uma string com o grafo representado em uma tabela
 */
char *graph_to_str(graph_t *g, char dot);

/**
 * Imprime o grafo
 */
void graph_print(graph_t *g, char dot);

/**
 * Algoritmo de busca em largura no grafo (Breadth-First Search)
 */
void graph_bfs(graph_t *g, int s);

/**
 * Algoritmo recursivo de busca em profundidade no grafo (Depth-First Search)
 */
void graph_dfs(graph_t *g);

/**
 * Inicializacao dos valores dos vertices para o algoritmo de Dijkstra
 */
int graph_dijkstra_initialize(graph_t *g, int s);

/**
 * Funcao de relaxamento de um vertice para o algoritmo de Dijkstra
 */
void graph_dijkstra_relax(graph_t *g, vertex_t *u, vertex_t *v, double w, int *count, double (*func)(int));

/**
 * Algoritmo de Dijkstra para caminhos minimos
 */
int graph_dijkstra(graph_t *g, int s, double (*func)(int));

/**
 * Passado os indices dos vertices de origem e destino,
 * procura por eles no grafo e depois chama a funcao
 * que imprime o caminho entre eles recursivamente
 */
int graph_print_path(graph_t *g, int s, int t);

/**
 * Imprime o caminho entre dos vertices
 */
void graph_print_path_recursive(graph_t *g, vertex_t *s, vertex_t *t);

#endif
