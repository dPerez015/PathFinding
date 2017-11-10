#pragma once
#include <iostream>
#include <vector>
#include "Vector2D.h"
#include <queue>
#include "Node.h"


using namespace std;


class BFS {
private:
	
	static queue<Node*> frontier;
	static vector<Node*> visited;
	static vector<Vector2D> path;
	static bool notFound;
public:
	
	//static void BFSinit();
	static void search(Node*, Vector2D); 
	static void expandFrontier(Node*); 
	static void fillPath(Node*);
	


};
