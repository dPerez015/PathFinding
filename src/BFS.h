#pragma once
#include <iostream>
#include <vector>
#include "Vector2D.h"
#include <queue>

using namespace std;

struct Node {
	Vector2D position;
	float cost;
};

struct Connection {
	Node fromNode, toNode;
	float cost;
};

struct graph {
	vector<Node> n;
	vector<Connection> c;
};

class BFS {
private:
	graph worldGraph;
	queue<Node> frontier;
	vector<Node> visited;
public:
	BFS(); //inicialitza la frontera amb el node de la posicio inicial (i el graph)
	~BFS();

	void search(); //funcio principal, si frontier!=0;
	bool isTarget();//si es target->break
	void expandFrontier(Node n); //afageix a la frontera els veins de n
	void addToVisited(Node n); //afageix n a visited
	

	graph getGraph();
	queue<Node> getFrontier();
	vector<Node> getVisited();

};
