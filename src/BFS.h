#pragma once
#include <iostream>
#include <vector>
#include "Vector2D.h"
#include <queue>
#include "Node.h"
#include "Heuristics.h"


using namespace std;


class BFS {
private:	
	static queue<Node*> frontier;
	static vector<vector<bool>> visitedNode;
	static vector<Vector2D> path;
	static bool notFound;
	
public:
	static void search(Node*, Vector2D);
	static void BFSinit(Node*);
	static void expandFrontier(Node*); 
	static void fillPath(Node*);
	static bool isVisited(Node*);

};
