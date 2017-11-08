#pragma once
#include <iostream>
#include <vector>
#include "Vector2D.h"
#include <queue>
#include "Node.h"

using namespace std;

struct NodeF {
	Vector2D position;
	float cost;
};

struct ConnectionF {
	NodeF fromNode, toNode;
	float cost;
};

struct graphF {
	vector<NodeF> n;
	vector<ConnectionF> c;
};

class BFS {
private:
	graphF worldGraph;
	queue<NodeF> frontier;
	vector<NodeF> visited; //amb struct Node del david si que es pot fer, sino shan de guardar les connexions (el node anterior)
public:
	BFS(); //inicialitza la frontera amb el node de la posicio inicial (i el graph)
	~BFS();

	void search(); //funcio principal, si frontier!=0;
	bool isTarget();//si es target->break
	void expandFrontier(NodeF n); //afageix a la frontera els veins de n
	void addToVisited(NodeF n); //afageix n a visited
	

	graphF getGraph();
	queue<NodeF> getFrontier();
	vector<NodeF> getVisited();

};
