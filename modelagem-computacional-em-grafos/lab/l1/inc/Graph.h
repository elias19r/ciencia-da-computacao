/*
	TO-DO:
		- remover aresta
		- remover vértice
		- retornar vértices adjacentes
		- obter transposta do grafo
*/

/**
 * Implementação de grafo em C++ usando o modelo proposto por Tanenbaum
 * 2013/03/14
 * Elias Italiano Rodrigues, 7987251, Grafos, BCC-A, ICMC-USP
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

// Inclui headers necessários
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Inclui classes dos vértices e das arestas
#include <Vertice.h>
#include <Edge.h>

/**
 * Classe Graph
 */
class Graph {
	private:
		bool direcional;      // define se é ou não um grafo direcionado
		Vertice *first;       // ponteiro para o primeiro nó da lista de vértices
		Vertice *last;        // ponteiro para o último nó da lista de vértices
		int vertices;         // quantidade total de vértices neste grafo
		int edges;            // quantidade total de arestas neste grafo
		int lastVerticeIndex; // último índice usado para um vértice
		int lastEdgeIndex;    // último índice usado para uma aresta
		int error;            // código do erro ocorrido (ainda não implementado...)
		Vertice **queue;      // fila/lista de vértices auxiliar usada para BFS e DFS
		int queueCount;

		/**
		 * Retorna a quantidade de dígitos de um número inteiro
		 */
		int numberOfDigits(int num) {
			int digits = 0;

			if (num < 0)
				digits = 1; // inclui sinal de negativo

			while (num != 0) {
				num /= 10;
				digits++;
			}
			return digits;
		}
		
		void newQueue() {
			if (this->queue != NULL)
				free(this->queue);
			this->queue = NULL;
			this->queueCount = 0;
		}
		
		void enqueue(Vertice *v) {
			this->queue = (Vertice **) realloc(this->queue, sizeof(Vertice *) * (this->queueCount + 1));
			this->queue[this->queueCount++] = v;
		}
		
		Vertice *dequeue() {
			return NULL;
		}
		
		bool empty() {
			if (this->queueCount > 0)
				return true;
			return false;
		}
		
	public:
		
		/**
		 * Construtor
		 * Recebe o número de vértices como parâmetro
		 */
		Graph(int num = 0, bool direcional = true) {
			this->direcional = direcional;
			this->first = NULL;
			this->last = NULL;
			this->vertices = 0;
			this->edges = 0;
			this->lastVerticeIndex = 0;
			this->lastEdgeIndex = 0;
			this->error = 0;

			for (int i = 0; i < num; i++)
				this->add();
				
			this->queue = NULL;
			this->queueCount = 0;
		}

		/**
		 * Destrutor
		 * Libera toda a memória alocada para o grafo
		 */
		~Graph() {
			// Libera os vértices
			if (this->vertices > 0) {
				Vertice *v = this->first;
				Vertice *tmp;
				do {
					// Libera as arestas do vértice
					if (v->edgesOut > 0) {
						Edge *e = v->first;
						Edge *tmp;
						do {
							tmp = e;
							e = e->next;							
							delete tmp;
						} while (e != NULL);
					}
					tmp = v;
					v = v->next;
					delete tmp;
				} while (v != NULL);
			}
		}

		/**
		 * Adiciona um novo vértice no grafo
		 */
		void add(const char *label = NULL, void *info = NULL) {
			this->lastVerticeIndex++;

			if (this->vertices == 0) {
				this->first = new Vertice(label, info);
				this->first->index = this->lastVerticeIndex;
				this->last = first;
				this->vertices++;

			} else if (this->vertices == 1) {
				this->last = new Vertice(label, info);
				this->last->index = this->lastVerticeIndex;
				this->first->next = this->last;
				this->last->prev = this->first;
				this->vertices++;

			} else {
				Vertice *v = new Vertice(label, info);
				v->index = this->lastVerticeIndex;
				this->last->next = v;
				v->prev = this->last;
				this->last = v;
				this->vertices++;
			}
		}

		/**
		 * Confere se um vértice existe
		 */
		bool exist(int index) {
			if (this->vertices > 0 && index <= this->lastVerticeIndex) {
				Vertice *v = this->first;
				do {
					if (v->index == index)
						return true;
					else
						v = v->next;
				} while (v != NULL);
			}
			return false;
		}

		/**
		 * Confere se um vértice existe (passado um ponteiro)
		 */
		bool exist(Vertice *v = NULL) {
			if (this->vertices > 0 && v != NULL) {
				if (v->index <= this->lastVerticeIndex) {
					Vertice *p = this->first;
					do {
						if (p == v)
							return true;
						else
							p = p->next;
					} while (v != NULL);
				}
			}
			return false;
		}

		/**
		 * Retorna o ponteiro para um vértice
		 */
		Vertice *get(int index) {
			if (this->vertices > 0 && index <= this->lastVerticeIndex) {
				Vertice *v = this->first;
				do {
					if (v->index == index)
						return v;
					else
						v = v->next;
				} while (v != NULL);
			}
			return NULL;
		}

		/**
		 * Adiciona uma nova aresta no grafo
		 */
		bool addEdge(int a, int b, double weight = 0.0, void *info = NULL) {
			Vertice *vA, *vB;

			vA = get(a);
			vB = get(b);
			
			return addEdge(vA, vB, weight, info);
		}
		
		bool addEdge(Vertice *vA = NULL, Vertice *vB = NULL, double weight = 0.0, void *info = NULL) {
			if (vA != NULL && vB != NULL) {
				if (!this->direcional && vB->index < vA->index) {
					Vertice *vC = vA;
					vA = vB;
					vB = vC;
				}
				
				this->lastEdgeIndex++;
				this->edges++;
				vA->edges++;
				vB->edges++;
				vB->edgesIn++;

				if (vA->edgesOut == 0) {
					vA->first = new Edge(weight, info);
					vA->first->to = vB;
					vA->first->index = this->lastEdgeIndex;
					vA->edgesOut++;

					return true;

				} else if (vA->edgesOut == 1) {
					vA->last = new Edge(weight, info);
					vA->last->prev = vA->first;
					vA->first->next = vA->last;
					vA->last->to = vB;
					vA->last->index = this->lastEdgeIndex;
					vA->edgesOut++;

					return true;

				} else {
					Edge *e = new Edge(weight, info);
					vA->last->next = e;
					e->prev = vA->last;
					vA->last = e;
					vA->last->to = vB;
					vA->last->index = this->lastEdgeIndex;
					vA->edgesOut++;

					return true;
				}
			}
			return false;
		}

		/**
		 * Confere se uma aresta existe
		 */
		bool existEdge(int a, int b) {
			Vertice *vA, *vB;

			vA = this->get(a);
			vB = this->get(b);

			return this->existEdge(vA, vB);
		}

		bool existEdge(Vertice *vA = NULL, Vertice *vB = NULL) {
			if (vA != NULL && vB != NULL) {
				if (!this->direcional && vB->index < vA->index) {
					Vertice *vC = vA;
					vA = vB;
					vB = vC;
				}

				// Confere se existe de vA para vB
				if (vA->edgesOut > 0) {
					Edge *e = vA->first;
					do {
						if (e->to == vB)
							return true;
						else
							e = e->next;
					} while (e != NULL);
				}
			}
			return false;
		}

		/**
		 * Recebe o ponteiro de uma aresta
		 */
		Edge *getEdge(int a, int b) {
			Vertice *vA, *vB;

			vA = this->get(a);
			vB = this->get(b);
			
			return this->getEdge(vA, vB);
		}
		
		Edge *getEdge(Vertice *vA = NULL, Vertice *vB = NULL) {
			if (vA != NULL && vB != NULL) {
				if (!this->direcional && vB->index < vA->index) {
					Vertice *vC = vA;
					vA = vB;
					vB = vC;
				}
				
				// Confere se existe de vA para vB
				if (vA->edgesOut > 0) {
					Edge *e = vA->first;
					do {
						if (e->to == vB)
							return e;
						else
							e = e->next;
					} while (e != NULL);
				} else
					return false;
			}
			return NULL;
		}
		
		/**
		 * Remove uma aresta
		 */
		bool removeEdge(int a, int b) {
			Vertice *vA, *vB;

			vA = this->get(a);
			vB = this->get(b);

			return this->removeEdge(vA, vB);
		}

		bool removeEdge(Vertice *vA = NULL, Vertice *vB = NULL) {
			if (vA != NULL && vB != NULL) {
				if (!this->direcional && vB->index < vA->index) {
					Vertice *vC = vA;
					vA = vB;
					vB = vC;
				}

				// Confere se existe de vA para vB
				if (vA->edgesOut > 0) {
					Edge *e = vA->first;
					do {
						// Se achou o vértice
						if (e->to == vB) {
							if (vA->first == e) {
								// A FAZER...
							} else {
								// A FAZER...
							}
							delete e;
							return true;
						}
						else
							e = e->next;
					} while (e != NULL);
				} else
					return false;
			}
			return false;
		}

		/**
		 * Atualiza o peso de uma aresta (definida pelos índices dos vértices)
		 */
		bool updateWeight(int a, int b, double weight) {
			Vertice *vA, *vB;
			
			vA = get(a);
			vB = get(b);
			
			return this->updateWeight(vA, vB);
		}
		
		bool updateWeight(Vertice *vA = NULL, Vertice *vB = NULL, double weight = 0.0) {
			Edge *e = getEdge(vA, vB);

			if (e != NULL) {
				e->weight = weight;
				return true;
			}
			return false;
		}
		
		/**
		 * Retorna a quantidade de vértices no grafo
		 */
		int amountOfVertices() {
			return this->vertices;
		}

		/**
		 * Retorna a quantidade de arestas no grafo
		 */
		int amountOfEdges() {
			return this->edges;
		}

		/**
		 * Imprime o grafo
		 */
		void print(char dot = 'X') {
			char *str = this->toStr(dot);
			if (str != NULL) {
				printf("%s\n", str);
				free(str);
			}
		}

		/**
		 * Retorna um vetor de inteiros com os índices do vértices do grafo
		 */
		int *getAllVerticesIndexes() {
			if (this->vertices > 0) {
				int *all = (int *) malloc(sizeof(int) * this->vertices);
				int i = 0;
				Vertice *v = this->first;

				do {
					all[i++] = v->index;
					v = v->next;
				} while (v != NULL);

				return all;
			}
			return NULL;
		}

		/**
		 * Retorna um vetor de ponteiros para os vértices do grafo
		 */
		Vertice **getAllVertices() {
			if (this->vertices > 0) {
				Vertice **all = (Vertice **) malloc(sizeof(Vertice *) * this->vertices);
				Vertice *v = this->first;
				int i = 0;

				do {
					all[i++] = v;
					v = v->next;

				} while (v != NULL);

				return all;
			}
			return NULL;
		}

		Vertice **getAdjacentes(int index) {
			Vertice *v = get(index);
			return this->getAdjacentes(v);
		}
		
		Vertice **getAdjacentes(Vertice *v) {
			if (v != NULL) {
				if (v->edgesOut > 0) {
					Vertice **all = (Vertice **) malloc(sizeof(Vertice *) * v->edgesOut);
					Edge *e = v->first;
					int i = 0;
					
					do {
						all[i++] = e->to;
						e = e->next;
					} while (e != NULL);
				}
			}
			return NULL;
		}

		bool BFS(int a, int b, bool postOrder = false) {
			Vertice *vA, *vB;
			
			vA = this->get(a);
			vB = this->get(b);
			
			return this->BFS(vA, vB, postOrder);
		}

		bool BFS(Vertice *vA = NULL, Vertice *vB = NULL, bool postOrder = false) {
			if (vA != NULL && vB != NULL) {
				Vertice *v;
				Vertice **vAdj;
				
				// Reseta cor, distância e predecessores de todos os vértices
				v = this->first;
				do {
					v->color = WHITE;
					v->distance = INF;
					v->predecessors(NIL);

					v = v->next;
				} while (v != NULL);
				
				// Coloca o vértice A na lista
				vA->color = GRAY;
				vA->distance = 0;
				vA->predecessors(NIL);
				this->enqueue(vA);
				
				// Laço onde processa e enfila os vértices adjacentes começando pelo inicial (A)
				while (!this->empty()) {
					v = this->dequeue();
					vAdj = getAdjacentes(v);
					
					// ... processa v
					if (v == vB) {
						v->color = BLACK;
						free(vAdj);
						break;
					}
					
					for (int i = 0, n = v->edgesOut; i < n; i++) {
						// ... processa aresta (v, vAdj)
						
						if (vAdj[i]->color == WHITE) {
							vAdj[i]->color = GRAY;
							vAdj[i]->distance = v->distance + 1;
							vAdj[i]->predecessors(v);
							this->enqueue(vAdj[i]);
						}
					}
					v->color = BLACK;
					free(vAdj);
				}
				return true;
			}
			return false;
		}
		
		bool DFS(int a, int b, bool postOrder = false) {
			return false;
		}

		/**
		 * Retorna uma string com o grafo representado em uma tabela
		 */
		char *toStr(char dot = 'X') {
			if (this->vertices > 0) {
				char *str = NULL;
				char formatNum[5];   // formatação usada para os números dos índices do vértices
				char formatSpace[5]; // formatação usada para o espaço em branco
				int count = 1; // contador usado para alocar a string
				int at = 0;    // variável auxiliar usada para apontar o fim da string
				int i, j;      // iteradores

				int *all = getAllVerticesIndexes();
				int amount = amountOfVertices();

				// Recebe a quantidade de dígitos do possível maior índice
				int digits = numberOfDigits(this->lastVerticeIndex);

				// Define as formatações com base na quantidade de dígitos do possível maior índice
				sprintf(formatNum, "%%%dd ", digits);
				sprintf(formatSpace, "%%%dc ", digits);

				// Imprime um espaço em branco
				count += digits+1;
				str = (char *) realloc(str, sizeof(char) * count);
				at += sprintf(str+at, formatSpace, ' ');

				// Imprime os índices da parte de cima da tabela
				for (i = 0; i < amount; i++) {
					count += digits+1;
					str = (char *) realloc(str, sizeof(char) * count);
					at += sprintf(str+at, formatNum, all[i]);
				}

				// Quebra de linha
				count += 1;
				str = (char *) realloc(str, sizeof(char) * count);
				at += sprintf(str+at, "\n");

				// Imprime as linhas da tabela
				for (i = 0; i < amount; i++) {

					// Índice da coluna esquerda da tabela
					count += digits+1;
					str = (char *) realloc(str, sizeof(char) * count);
					at += sprintf(str+at, formatNum, all[i]);

					for (j = 0; j < amount; j++) {
						count += digits+1;
						str = (char *) realloc(str, sizeof(char) * count);

						// Se existe aresta, imprime um caractere dot
						// Se não, imprime apenas um espaço "em branco"
						if (existEdge(all[i], all[j]))
							at += sprintf(str+at, formatSpace, dot);
						else
							at += sprintf(str+at, formatSpace, '.');
					}

					// Quebra de linha
					count += 1;
					str = (char *) realloc(str, sizeof(char) * count);
					at += sprintf(str+at, "\n");
				}
				free(all);

				return str;
			}
			return NULL;
		}
};

#endif
