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
	static vector<Node*> visited; //amb struct Node del david si que es pot fer, sino shan de guardar les connexions (el node anterior)
	static vector<Vector2D> path;
public:
	
	static void search(Node*, Vector2D); //funcio principal, si frontier!=0;
	static bool isTarget();//si es target->break
	static void expandFrontier(Node*); //afageix a la frontera els veins de n
	static void addToVisited(Node n); //afageix n a visited
	static void fillPath(Node*);
	


};
