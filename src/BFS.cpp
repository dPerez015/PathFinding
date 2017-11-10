#include "BFS.h"

#define  GRID_LENGTH 40
#define  GRID_WIDTH 24

queue<Node*> BFS::frontier;
vector<vector<bool>> BFS::visitedNode;
vector<Vector2D> BFS::path;
bool BFS::notFound;


void BFS::BFSinit(Node* startNode) {
	//buidem frontera
	while (!frontier.empty()) frontier.pop();

	//creem el nou vector de Nodes visitats
	visitedNode.clear();
	for (int i = 0; i < GRID_LENGTH; i++) {
		std::vector<bool> tmp(GRID_WIDTH, false);
		visitedNode.push_back(tmp);
	}

	//buidem el path
	path.clear();

	//encara no sha trobat el cami
	notFound = true;

	//preparem el primer Node per la frontea
	frontier.push(startNode);
	startNode->previousNode = nullptr;
}

void BFS::search(Node* startNode, Vector2D endPos) {
	
	BFSinit(startNode);

	while (!frontier.empty() && notFound) {
		if (frontier.front()->position == endPos) {
			fillPath(frontier.front());
			notFound = false;
		}
		else {
			expandFrontier(frontier.front());

			Vector2D aux = Heuristics::pix2cell(frontier.front()->position);
			visitedNode[aux.x][aux.y] = true; //afegir a visited

			frontier.pop(); //treure el node de la frontera
		}
	}

}

void BFS::expandFrontier(Node* n) {
	//afegir els veins de n a la frontera
	for (int i = 0; i < n->conexiones.size(); i++) {
		if (!isVisited(n->conexiones[i])) {
			n->conexiones[i]->previousNode = n;
			frontier.push(n->conexiones[i]);
		}
	}
};

bool BFS::isVisited(Node* n) {
	Vector2D gridPos = Heuristics::pix2cell(n->position);
	if (visitedNode[gridPos.x][gridPos.y] == true)  return true;
	return false;
}

void BFS::fillPath(Node* end) {
	//omplir path amb les posicions dels nodes del cami fent previousNode
	Node* tempNode = end;
	while (tempNode != nullptr) {
		path.push_back(tempNode->position);
		tempNode = tempNode->previousNode;
	}

	Vector2D tempVec;
	for (int i = 0; i = path.size() / 2; i++) {
		tempVec = path[i];
		path[i] = path[path.size() - 1 - i];
		path[path.size() - 1 - i] = tempVec;
	}
};

