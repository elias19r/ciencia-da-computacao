#include <stdlib.h>
#include <stdio.h>

#include <Graph.h>

int main(int argc, char *argv[]) {
	Graph *g = new Graph();
	
	for (int i = 0; i <= 10; i++) {
		g->add(i);
	}
	
	g->addEdge(0,1);
	g->addEdge(0,2);
	g->addEdge(0,3);
	g->addEdge(1,4);
	g->addEdge(1,5);
	g->addEdge(2,6);
	g->addEdge(2,7);
	g->addEdge(3,9);
	g->addEdge(4,8);
	g->addEdge(7,5);
	g->addEdge(7,10);
	g->addEdge(8,7);

	g->print();
	
	g->BFS(0);
	
	g->get(10)->printPredecessors();
	
	delete g;

	return 0;
}

