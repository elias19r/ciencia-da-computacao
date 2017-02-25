class Vertice;

/**
 * Classe da aresta
 */
class Edge {
	public:
		/**
		 * Construtor
		 */
		Edge(double weight, void *info) {
			this->index = 0;
			this->to = NULL;
			this->weight = weight;
			this->info = info;
			this->prev = NULL;
			this->next = NULL;
		}
		
		/**
		 * Destrutor
		 */
		~Edge() {}
		
		int index;        // índice da aresta
		Vertice *to;      // ponteiro para o vértice que esta aresta aponta
		double weight;    // peso associado à aresta
		void *info;       // informação anexada à aresta
		Edge *prev;       // ponteiro para a aresta anterior
		Edge *next;       // ponteiro para a próxima aresta
};
