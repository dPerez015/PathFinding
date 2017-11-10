#include "BFS.h"

queue<Node*> BFS::frontier;
vector<vector<bool>> BFS::visitedNode;
vector<Vector2D> BFS::path;
bool BFS::notFound;

void BFS::search(Node* startNode, Vector2D endPos) {
	
	frontier.push(startNode);//------------¬
	startNode->previousNode = nullptr;//----- tot aixo ficarho a BFSinit() junt amb la neteja de les variables: path.clear() per exemple i visitedNode to false
	visited.push_back(startNode);//--------/

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

void BFS::fillPath(Node* end) {
	//omplir path amb les posicions dels nodes del cami fent previousNode
	Node* temp = end; //potser no cal, es pot fer amb end
	while (temp != nullptr) {
		
		//path.push_back(temp->position);
		temp = temp->previousNode;
		
	}

	//path.reverse();
	/*Vector2D temp;
	for (int i = 0; i = path.size() / 2; i++) {
		temp = path[i];
		path[i] = path[path.size() - 1 - i];
		path[path.size() - 1 - i] = temp;
	}*/
};

bool BFS::isVisited(Node* n) {
	Vector2D gridPos = Heuristics::pix2cell(n->position);
	if (visitedNode[gridPos.x][gridPos.y] == true)  return true;
	return false;
}