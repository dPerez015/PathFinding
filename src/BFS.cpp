#include "BFS.h"

queue<Node*> BFS::frontier;
vector<Node*> BFS::visited;
vector<Vector2D> BFS::path;
bool BFS::notFound;

void BFS::search(Node* startNode, Vector2D endPos) {
	
	frontier.push(startNode);//------------¬
	startNode->previousNode = nullptr;//----- tot aixo ficarho a BFSinit() junt amb la neteja de les variables: path.clear() per exemple 
	visited.push_back(startNode);//--------/

	while (!frontier.empty() && notFound) {
		if (frontier.front()->position == endPos) {
			fillPath(frontier.front());
			notFound = false;
		}
		else {
			//expandFrontier(Node actual)
			frontier.pop();
		}
	}

} //funcio principal, si frontier!=0;


void expandFrontier(Node* n) {
	//calcular els nous nodes a afegir a la frontera i en quin ordre
	//apuntar el previousNode dels nous nodes
	//add to visited n
}; 

void fillPath(Node* end) {
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