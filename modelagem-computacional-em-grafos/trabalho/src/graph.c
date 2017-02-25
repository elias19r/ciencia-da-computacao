/**
 * Implementacao de grafo em C usando vetores
 * Elias Italiano Rodrigues, 7987251, Grafos, BCC, ICMC-USP
 * 2013/06
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <graph.h>

/**
 * Variavel auxiliar para o algoritmo DFS
 */
int time;

/**
 * Estrutura de lista auxiliar para o BFS e Dijkstra
 */
typedef struct queue_s {
	vertex_t **elements;
	int size;
	int start;
	int end;
} queue_t;

/**
 * Finaliza a lista
 */
static void queue_finalize(queue_t *q) {
	if (q->elements != NULL)
		free(q->elements);
}

/**
 * Inicializa a lista
 */
static void queue_initialize(queue_t *q) {
	queue_finalize(q);
	q->elements = NULL;
	q->size = 0;
	q->start = 0;
	q->end = -1;
}

/**
 * Confere se a lista esta vazia
 */
static int queue_empty(queue_t *q) {
	if (q->start > q->end) 
		return 1;

	return 0;
}

/**
 * Adiciona um vertice no fim lista
 */
static void queue_enqueue(queue_t *q, vertex_t *v) {
	q->elements = (vertex_t **) realloc(q->elements, sizeof(vertex_t *) * (q->size + 1));
	q->elements[q->size++] = v;
	q->end++;
}

/**
 * "Remove" e retorna um vertice do comeco da lista
 */
static vertex_t *queue_dequeue(queue_t *q) {
	q->start++;
	return q->elements[q->start-1];
}

/**
 * Troca dois elementos na lista
 */
static void queue_swap(queue_t *q, int i, int j) {
	vertex_t *tmp  = q->elements[i];
	q->elements[i] = q->elements[j];
	q->elements[j] = tmp;
}

/**
 * Ordena a lista (quicksort recursivo)
 */
static void queue_sort(queue_t *q, int left, int right) {
	if (left < right) {
		int i = left, j = left + 1;
		
		while (j <= right) {
			// Comparacao de valores e troca (swap)
			if (q->elements[j]->distance < q->elements[left]->distance) {
				i++;
				queue_swap(q, i, j);
			}
			j++;
		}
		// Troca (swap)
		queue_swap(q, left, i);

		// Chamadas recursivas para esquerda e para direita
		queue_sort(q, left, i - 1);
		queue_sort(q, i + 1, right);
	}	
}

/**
 * Popula a lista com um vetor de vertices
 */
static void queue_create(queue_t *q, vertex_t **vertices, int count) {
	int i;
	
	for (i = 0; i < count; i++)
		if (vertices[i] != NULL) // nao insere vertices nulos
			queue_enqueue(q, vertices[i]);
}

/**
 * Inicializa os valores de um grafo
 */
void graph_initialize(graph_t *g) {
	g->dir = 1;
	g->vertices = NULL;
	g->count = 0;
	g->amount = 0;
}

/**
 * Finaliza um grafo, liberando toda a memoria alocada
 */
void graph_finalize(graph_t *g) {
	if (g->vertices != NULL) {
		int i, j;

		// Percorre liberando o vetor de vertices
		for (i = 0; i < g->count; i++)
			if (g->vertices[i] != NULL) {

				// Percorre liberando o vetor de arestas de cada vertice
				if (g->vertices[i]->edges != NULL) {
					for (j = 0; j < g->vertices[i]->count; j++)
						if (g->vertices[i]->edges[j] != NULL)
							free(g->vertices[i]->edges[j]);
				
					free(g->vertices[i]->edges);
				}

				// Libera a label				
				if (g->vertices[i]->label != NULL)
					free(g->vertices[i]->label);

				// Libera o vertice
				free(g->vertices[i]);
			}

		free(g->vertices);
		g->vertices = NULL;
	}
	
	// Reseta valors das variaveis
	g->dir = 1;
	g->count = 0;
	g->amount = 0;
}

/**
 * Cria um grafo com uma quantidade de vertices
 */
void graph_create(graph_t *g, int count) {
	int i;
	
	g->vertices = (vertex_t **) calloc(1, sizeof(vertex_t *) * count);
	g->count = g->amount = count;
	
	for (i = 0; i < count; i++) {
		g->vertices[i] = (vertex_t *) calloc(1, sizeof(vertex_t));
		g->vertices[i]->index = i + 1; // nao por padrao o indice 0 (zero)
		g->vertices[i]->edges = NULL;
		g->vertices[i]->count = 0;
		g->vertices[i]->amount = 0;
	}
}

/**
 * Adiciona um vertice no grafo
 */
int graph_add(graph_t *g, int v, const char *label) {
	// Confere se o vertice ainda nao existe no grafo
	if (!graph_exists(g, v)) {
		g->vertices = (vertex_t **) realloc(g->vertices, sizeof(vertex_t *) * (g->count + 1));
		g->vertices[g->count] = (vertex_t *) calloc(1, sizeof(vertex_t));
		g->vertices[g->count]->index = v;
		g->vertices[g->count]->label = (char *) malloc(sizeof(char) * (strlen(label) + 1));
		strcpy(g->vertices[g->count]->label, label);
		g->vertices[g->count]->edges = NULL;
		g->vertices[g->count]->count = 0;
		g->vertices[g->count]->amount = 0;
		g->count++;
		g->amount++;
		
		return 1;
	}

	return 0;
}

/**
 * Funcao para ler do arquivo uma string limitada por aspas (no caso, a label)
 */
static void graph_fscanf_label(FILE *fp, char **label) {
	int count = 0, quote = 0;
	char c;
	
	*label = NULL;
	while (!feof(fp)) {
		fscanf(fp, "%c", &c);
		
		if (c == '"') {
			quote = !quote;
			if (!quote) {
				*label = (char *) realloc(*label, sizeof(char) * (count + 1));
				(*label)[count] = '\0';
				break;
			}
		} else {
			if (isspace(c) && !quote) {
				if (count > 0) {
					*label = (char *) realloc(*label, sizeof(char) * (count + 1));
					(*label)[count] = '\0';
					break;
				}
			} else {
				*label = (char *) realloc(*label, sizeof(char) * (count + 1));
				(*label)[count++] = c;
			}
		}
	}
}

/**
 * Adiciona vertices no grafo a partir de um arquivo
 */
int graph_add_from_file(graph_t *g, const char *filename) {
	FILE *fp;
	int index;
	char *label;
	
	// Abre o arquivo	
	fp = fopen(filename, "r");
	if (fp != NULL) {
		while (!feof(fp)) {
			// Le indice e label (o arquivo deve conter obrigatoriamente esses dois campos)
			fscanf(fp, "%d", &index);
			graph_fscanf_label(fp, &label);

			if (!feof(fp)) 
				graph_add(g, index, label); // adiciona o vertice

			free(label);
		}
		fclose(fp);
		return 1;
	}
	
	return 0;
}

/**
 * Altera informacoes de um vertice
 */
int graph_edit(graph_t *g, int v, int new_index, const char *new_label) {
	int i, v_pos = -1;
	
	// Procura pelo vertice
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL && g->vertices[i]->index == v) {
			v_pos = i;
			break;
		}

	// Se encontrou, atualiza info
	if (v_pos != -1) {
		g->vertices[v_pos]->index = new_index;
		g->vertices[v_pos]->label = (char *) realloc(g->vertices[v_pos]->label, sizeof(char) * (strlen(new_label) + 1));
		strcpy(g->vertices[v_pos]->label, new_label);
		
		return 1;
	}

	return 0;
}

/**
 * Remove um vertice do grafo
 */
int graph_delete(graph_t *g, int v) {
	int i, j, v_pos = -1;
	
	// Procura pelo vertice
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL && g->vertices[i]->index == v) {
			v_pos = i;
			break;
		}
	
	// Se encontrou
	if (v_pos != -1) {

		// Remove todas as arestas do outros vertices que ligam com este
		for (i = 0; i < g->count; i++)
			if (g->vertices[i] != NULL) {
				for (j = 0; j < g->vertices[i]->count; j++)
					if (g->vertices[i]->edges[j] != NULL && g->vertices[i]->edges[j]->to == g->vertices[v_pos]) {
						free(g->vertices[i]->edges[j]);
						g->vertices[i]->edges[j] = NULL;
						g->vertices[i]->amount--;
					}
			}
		
		// Remove arestas deste vertice
		if (g->vertices[v_pos]->edges != NULL) {
			for (i = 0; i < g->vertices[v_pos]->count; i++)
				if (g->vertices[v_pos]->edges[i] != NULL)
					free(g->vertices[v_pos]->edges[i]);

			free(g->vertices[v_pos]->edges);
		}
		
		// Libera a label
		if (g->vertices[v_pos]->label != NULL)
			free(g->vertices[v_pos]->label);
		
		// Libera o vertice
		free(g->vertices[v_pos]);

		g->vertices[v_pos] = NULL;
		g->amount--;

		return 1;
	}
	
	return 0;
}

/**
 * Retorna o ponteiro de um vertice do grafo
 */
vertex_t *graph_get(graph_t *g, int v) {
	int i;
	
	// Procura o vertice. Se encontrou, retorna o ponteiro
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL && g->vertices[i]->index == v)
			return g->vertices[i];
	
	return NULL;
}

/**
 * Confere se um vertice existe no grafo
 */
int graph_exists(graph_t *g, int v) {
	int i;
	
	// Procura pelo vertice. Se encontrou, retorna 1
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL && g->vertices[i]->index == v)
			return 1;
	
	return 0;
}

/**
 * Retorna a quantidade de vertices no grafo
 */
int graph_amount(graph_t *g) {
	return g->amount;
}

/**
 * Cria uma aresta, ligando dois vertices existentes
 */
int graph_link(graph_t *g, int a, int b, double weight) {
	int a_pos = -1, b_pos = -1, i;
	
	// Procura pelos vertices
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL) {
			if (g->vertices[i]->index == a)
				a_pos = i;
		
			if (g->vertices[i]->index == b)
				b_pos = i;
		}
	
	// Se encontrou os vertices, cria aresta de a para b
	if (a_pos != -1 && b_pos != -1) {
		g->vertices[a_pos]->edges = (edge_t **) realloc(g->vertices[a_pos]->edges, sizeof(edge_t *) * (g->vertices[a_pos]->count + 1));
		g->vertices[a_pos]->edges[g->vertices[a_pos]->count] = (edge_t *) calloc(1, sizeof(edge_t));
		g->vertices[a_pos]->edges[g->vertices[a_pos]->count]->to = g->vertices[b_pos];
		g->vertices[a_pos]->edges[g->vertices[a_pos]->count]->weight = weight;
		g->vertices[a_pos]->count++;
		g->vertices[a_pos]->amount++;

		// Se o grafo nao eh direcionado, cria aresta de b para a tambem
		if (!g->dir) {
			g->vertices[b_pos]->edges = (edge_t **) realloc(g->vertices[b_pos]->edges, sizeof(edge_t *) * (g->vertices[b_pos]->count + 1));
			g->vertices[b_pos]->edges[g->vertices[b_pos]->count] = (edge_t *) calloc(1, sizeof(edge_t));
			g->vertices[b_pos]->edges[g->vertices[b_pos]->count]->to = g->vertices[a_pos];
			g->vertices[b_pos]->edges[g->vertices[b_pos]->count]->weight = weight;
			g->vertices[b_pos]->count++;
			g->vertices[b_pos]->amount++;
		}
		
		return 1;
	}
	
	return 0;
}

/**
 * Cria arestas, ligando dois vertices existentes, a partir de um arquivo
 */
int graph_link_from_file(graph_t *g, const char *filename) {
	FILE *fp;
	int a, b, ret = -1;
	double weight;

	// Abre o arquivo	
	fp = fopen(filename, "r");
	if (fp != NULL) {
		while (!feof(fp)) {
			// Le os dois indices e o peso (o arquivo deve conter obrigatoriamente esses campos)
			fscanf(fp, "%d", &a);
			fscanf(fp, "%d", &b);
			fscanf(fp, "%lf", &weight);
			if (!feof(fp))
				ret = graph_link(g, a, b, weight); // cria a aresta
		}
		fclose(fp);
		return ret;
	}
	
	return ret;
}

/**
 * Remove uma aresta entre dois vertices existentes
 */
int graph_unlink(graph_t *g, int a, int b) {
	int a_pos = -1, b_pos = -1, i;
	
	// Procura pelos vertices no grafo
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL) {
			if (g->vertices[i]->index == a)
				a_pos = i;
		
			if (g->vertices[i]->index == b)
				b_pos = i;
		}
	
	// Se encontrou os vertices
	if (a_pos != -1 && b_pos != -1) {

		// Procura pela aresta de a para b e remove
		for (i = 0; i < g->vertices[a_pos]->count; i++)
			if (g->vertices[a_pos]->edges[i] != NULL && g->vertices[a_pos]->edges[i]->to == g->vertices[b_pos]) {
				free(g->vertices[a_pos]->edges[i]);
				g->vertices[a_pos]->edges[i] = NULL;
				g->vertices[a_pos]->amount--;
				break;
			}

		// Se o grafo nao eh direcionado, procura pela aresta de b para a e remove tambem
		if (!g->dir)
			for (i = 0; i < g->vertices[b_pos]->count; i++)
				if (g->vertices[b_pos]->edges[i] != NULL && g->vertices[b_pos]->edges[i]->to == g->vertices[a_pos]) {
					free(g->vertices[b_pos]->edges[i]);
					g->vertices[b_pos]->edges[i] = NULL;
					g->vertices[b_pos]->amount--;
					break;
				}
		
		return 1;
	}
	
	return 0;
}

/**
 * Confere se uma aresta existe no grafo
 */
int graph_exists_link(graph_t *g, int a, int b) {
	int a_pos = -1, b_pos = -1, i;
	
	// Procura pelos vertices
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL) {
			if (g->vertices[i]->index == a)
				a_pos = i;

			if (g->vertices[i]->index == b)
				b_pos = i;
		}
	
	// Se encontrou os vertices
	if (a_pos != -1 && b_pos != -1)
	
		// Procura pela aresta de a para b
		for (i = 0; i < g->vertices[a_pos]->count; i++)
			if (g->vertices[a_pos]->edges[i] != NULL && g->vertices[a_pos]->edges[i]->to == g->vertices[b_pos])
				return 1;
	
	return 0;
}

/**
 * Retorna um ponteiro de uma aresta do grafo
 */
edge_t *graph_get_link(graph_t *g, int a, int b) {
	int a_pos = -1, b_pos = -1, i;
	
	// Procura pelos vertices
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL) {
			if (g->vertices[i]->index == a)
				a_pos = i;

			if (g->vertices[i]->index == b)
				b_pos = i;
		}
	
	// Se encontrou os vertices
	if (a_pos != -1 && b_pos != -1)

		// Procura pela aresta de a para b
		for (i = 0; i < g->vertices[a_pos]->count; i++)
			if (g->vertices[a_pos]->edges[i] != NULL && g->vertices[a_pos]->edges[i]->to == g->vertices[b_pos])
				return g->vertices[a_pos]->edges[i];
	
	return NULL;
}

/**
 * Atualiza o peso de uma aresta do grafo
 */
int graph_weight(graph_t *g, int a, int b, double weight) {
	int ret = 0;
	edge_t *e;

	// Recebe ponteiro da aresta e altera seu peso
	e = graph_get_link(g, a, b);
	if (e != NULL) {
		e->weight = weight;
		ret = 1;
	}

	// Se o grafo nao eh direcionado, faz o mesmo para a aresta de b para a
	if (!g->dir) {
		e = graph_get_link(g, b, a);
		if (e != NULL) {
			e->weight = weight;
			ret = 1;
		}
	}
	
	return ret;
}

/**
 * Retorna a quantidade de digitos de um numero inteiro
 */
static int graph_number_of_digits(int num) {
	int digits = 0;

	if (num < 0)
		digits = 1; // inclui sinal de negativo

	while (num != 0) {
		num /= 10;
		digits++;
	}

	return digits;
}

/**
 * Retorna uma string com o grafo representado em uma tabela
 */
char *graph_to_str(graph_t *g, char dot) {
	if (g->amount > 0) {
		char *str = NULL;
		char format_num[5];   // formatacao usada para os numeros dos indices do vertices
		char format_space[5]; // formatacao usada para o espaco em branco
		int count = 1;        // contador usado para alocar a string
		int at = 0;           // variavel auxiliar usada para apontar o fim da string
		int i, j;             // iteradores

		// Recebe a quantidade de digitos do possivel maior indice
		int digits = graph_number_of_digits(g->count);
		if (digits < 2)
			digits = 2; // garante no minimo o tamanho de 2 digitos

		// Define as formatacoes com base na quantidade de digitos do possivel maior indice
		sprintf(format_num, "%%%dd ", digits);
		sprintf(format_space, "%%%dc ", digits);
		
		// Imprime um espaco em branco
		count += digits+1;
		str = (char *) realloc(str, sizeof(char) * count);
		at += sprintf(str+at, format_space, ' ');

		// Imprime os indices da parte de cima da tabela
		for (i = 0; i < g->count; i++)
			if (g->vertices[i] != NULL) {
				count += digits+1;
				str = (char *) realloc(str, sizeof(char) * count);
				at += sprintf(str+at, format_num, g->vertices[i]->index);
			}
		
		// Quebra de linha
		count += 1;
		str = (char *) realloc(str, sizeof(char) * count);
		at += sprintf(str+at, "\n");

		// Imprime as linhas da tabela
		for (i = 0; i < g->count; i++)
			if (g->vertices[i] != NULL) {
				// Indice da coluna esquerda da tabela
				count += digits+1;
				str = (char *) realloc(str, sizeof(char) * count);
				at += sprintf(str+at, format_num, g->vertices[i]->index);

				for (j = 0; j < g->count; j++)
					if (g->vertices[j] != NULL) {
						count += digits+1;
						str = (char *) realloc(str, sizeof(char) * count);

						// Se existe aresta, imprime um caractere dot
						// Se nao, imprime apenas um espaco "em branco"
						if (graph_exists_link(g, g->vertices[i]->index, g->vertices[j]->index))
							at += sprintf(str+at, format_space, dot);
						else
							at += sprintf(str+at, format_space, '.');
					}

				// Quebra de linha
				count += 1;
				str = (char *) realloc(str, sizeof(char) * count);
				at += sprintf(str+at, "\n");
			}

		// Retorna a string
		return str;
	}

	return NULL;
}

/**
 * Imprime o grafo
 */
void graph_print(graph_t *g, char dot) {
	char *str = graph_to_str(g, dot);
	printf("%s", str);
	free(str);
}

/**
 * Algoritmo de busca em largura no grafo (Breadth-First Search)
 */
void graph_bfs(graph_t *g, int s) {
	int i, s_pos = -1;
	vertex_t *v;
	queue_t *q = (queue_t *) calloc(1, sizeof(queue_t));
	
	// Inicializa todos os vertices do grafo
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL) {
			g->vertices[i]->color = _GRAPH_WHITE_;
			g->vertices[i]->distance = _GRAPH_INF_;
			g->vertices[i]->pre = NULL;

			if (g->vertices[i]->index == s)
				s_pos = i;
		}

	// Comeca o algoritmo de BFS
	if (s_pos != -1) {
		g->vertices[s_pos]->color = _GRAPH_GRAY_;
		g->vertices[s_pos]->distance = 0.0;
		g->vertices[s_pos]->pre = NULL;

		// Libera e inicializa a lista		
		queue_initialize(q);
		
		// Adiciona o vertice inicial na lista
		queue_enqueue(q, g->vertices[s_pos]);

		// Enquanto a lista nao estiver vazia
		while (!queue_empty(q)) {
			v = queue_dequeue(q);

			// Percorre os adjacentes de v
			for (i = 0; i < v->count; i++)
				if (v->edges[i] != NULL)

					// Se eh cor branca
					if (v->edges[i]->to->color == _GRAPH_WHITE_) {
						v->edges[i]->to->color = _GRAPH_GRAY_;
						v->edges[i]->to->distance = v->distance + 1.0;
						v->edges[i]->to->pre = v;
						queue_enqueue(q, v->edges[i]->to);
					}

			v->color = _GRAPH_BLACK_;
		}

		// Libera e reinicializa a lista
		queue_finalize(q);
		free(q);
	}
}

/**
 * Visita da busca em profundidade
 */
static void graph_dfs_visit(vertex_t *v) {
	int i;
	
	v->color = _GRAPH_GRAY_;
	time = time + 1;
	v->distance = (double) time;

	// Percorre e chama recursivamente para os vertices adjacentes	
	for (i = 0; i < v->count; i++)
		if (v->edges[i] != NULL)

			// Se eh cor branca
			if (v->edges[i]->to->color == _GRAPH_WHITE_) {
				v->edges[i]->to->pre = v;
				graph_dfs_visit(v->edges[i]->to); // chamada recursiva
			}

	v->color = _GRAPH_BLACK_;
	v->frequence = time = time + 1;
}

/**
 * Algoritmo recursivo de busca em profundidade no grafo (Depth-First Search)
 */
void graph_dfs(graph_t *g) {
	int i;
	
	// Inicializa todos os vertices do grafo
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL) {
			g->vertices[i]->color = _GRAPH_WHITE_;
			g->vertices[i]->frequence = 0;
			g->vertices[i]->pre = NULL;
		}

	time = 0;

	// Percorre os vertices do grafo, visitando-os
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL && g->vertices[i]->color == _GRAPH_WHITE_)
			graph_dfs_visit(g->vertices[i]);
}

/**
 * Inicializacao dos valores dos vertices para o algoritmo de Dijkstra
 */
int graph_dijkstra_initialize(graph_t *g, int s) {
	int i, s_pos = -1;
	
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL) {
			g->vertices[i]->distance = _GRAPH_INF_;
			g->vertices[i]->pre = NULL;
			
			if (g->vertices[i]->index == s)
				s_pos = i;
		}
	
	if (s_pos != -1)
		g->vertices[s_pos]->distance = 0.0;
	
	return s_pos;
}

/**
 * Funcao de relaxamento de um vertice para o algoritmo de Dijkstra
 */
void graph_dijkstra_relax(graph_t *g, vertex_t *u, vertex_t *v, double w, int *count, double (*func)(int)) {
	if (*func != NULL) {
		if (v->distance > u->distance + w + (*func)(v->index)) {
			v->distance = u->distance + w;
			v->pre = u;
			(*count)++;
		}
	} else {
		if (v->distance > u->distance + w) {
			v->distance = u->distance + w;
			v->pre = u;
			(*count)++;
		}
	}

}

/**
 * Algoritmo de Dijkstra para caminhos minimos
 */
int graph_dijkstra(graph_t *g, int s, double (*func)(int)) {
	
	// Inicializacao dos vertices
	if (graph_dijkstra_initialize(g, s) != -1) {
		int i, count = 0;
		double w;
		queue_t *q = (queue_t *) calloc(1, sizeof(queue_t));
		vertex_t *u, *v;
		
		// Inicia e popula a lista com os vertices do grafo
		queue_initialize(q);
		queue_create(q, g->vertices, g->count);
		queue_sort(q, q->start, q->end);

		// Enquanto houver vertices na lista
		while (!queue_empty(q)) {
			u = queue_dequeue(q);
		
			// Para cada adjacente de u
			for (i = 0; i < u->count; i++) {
				if (u->edges[i] != NULL) {
					v = u->edges[i]->to;
					w = u->edges[i]->weight;
					graph_dijkstra_relax(g, u, v, w, &count, func);
					queue_sort(q, q->start, q->end);
				}
			}
		}
	
		queue_finalize(q);
		free(q);
		
		return count;
	}

	return -1;
}

/**
 * Passado os indices dos vertices de origem e destino,
 * procura por eles no grafo e depois chama a funcao
 * que imprime o caminho entre eles recursivamente
 */
int graph_print_path(graph_t *g, int s, int t) {
	int i, s_pos = -1, t_pos = -1;
	
	// Procura pelos vertices
	for (i = 0; i < g->count; i++)
		if (g->vertices[i] != NULL) {
			if (g->vertices[i]->index == s)
				s_pos = i;

			if (g->vertices[i]->index == t)
				t_pos = i;
		}
	
	// Se encontrou os vertices
	if (s_pos != -1 && t_pos != -1) {
		graph_print_path_recursive(g, g->vertices[s_pos], g->vertices[t_pos]);
		return 1;
	}
	
	return 0;
}

/**
 * Imprime o caminho entre dos vertices
 */
void graph_print_path_recursive(graph_t *g, vertex_t *s, vertex_t *t) {
	if (s == t) {
		printf("%4d \"%s\"\n", s->index, s->label);
	} else if (t->pre == NULL) {
		printf("No path found.\n");
	} else {
		graph_print_path_recursive(g, s, t->pre);
		printf("%4d \"%s\"\n", t->index, t->label);
	}
}

