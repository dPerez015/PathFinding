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

	//no son les funcions de veritat eh, es un exemple
	void expandFrontier() {}
	void addToVisited() {}
	bool isTarget() {}
};
