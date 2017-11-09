#include "BFS.h"

queue<Node*> BFS::frontier;
vector<Node*> BFS::visited;
vector<Vector2D> BFS::path;

void BFS::search(Node* startNode, Vector2D endPos) {
	frontier.push(startNode);
	visited.push_back(startNode);

	if
} //funcio principal, si frontier!=0;

bool isTarget();//si es target->break

void expandFrontier(Node* n) {
	
}; //afageix a la frontera els veins de n
void addToVisited(Node n); //afageix n a visited
void fillPath(Node*);