#include "GreedyFirstSearch.h"

 std::multimap<int, Node*> GFS::frontera;
 std::multimap<int, Node*>::iterator GFS::it;
 std::vector<Vector2D> GFS::path;
 std::vector<std::vector<bool>> GFS::alreadyVisitedNodes;
 bool GFS::notFound;
 bool GFS::notFoundInFrontier;

 void GFS::initGFS(int x, int y) {
	
	 for (int i = 0; i < x; i++) {
		 std::vector<bool> tmp(y, false);
		 alreadyVisitedNodes.push_back(tmp);
	 }
 }
 void GFS::activateBool(Vector2D v) {
	 v=Heuristics::pix2cell(v);
	 alreadyVisitedNodes[v.x][v.y]=true;
 }

 bool GFS::checkVisited(Vector2D v) {
	 v = Heuristics::pix2cell(v);
	 return alreadyVisitedNodes[v.x][v.y];
 }

 void GFS::draw() {
	 it = frontera.begin();
	 if (it != frontera.end()){
		 draw_circle(TheApp::Instance()->getRenderer(), it->second->position.x, it->second->position.y, TheApp::Instance()->getGridCellSize().x / 2, 0, 255, 0, 255);
		 //SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), it->second->position.x, it->second->position.y, it->second->previousNode->position.x, it->second->previousNode->position.y);
		 it++;
 }
	while (it != frontera.end()) {
		draw_circle(TheApp::Instance()->getRenderer(), it->second->position.x, it->second->position.y, TheApp::Instance()->getGridCellSize().x / 2, 255, 255, 0, 255);
		//SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), it->second->position.x, it->second->position.y, it->second->previousNode->position.x, it->second->previousNode->position.y);
		it++;
	}
	for (int i = 0; i < path.size();i++) {
		draw_circle(TheApp::Instance()->getRenderer(), path[i].x, path[i].y, TheApp::Instance()->getGridCellSize().x / 2, 255, 0, 0, 255);
	}

}
//solo paraDebug
void GFS::SearchPerTick(Node* startNode, Vector2D endPos) {
	int temp;
	if (frontera.empty()||!notFound) {
		frontera.clear();
		path.clear();
		for (int i = 0; i < alreadyVisitedNodes.size(); i++) {
			for (int j = 0; j < alreadyVisitedNodes[i].size(); j++)
				alreadyVisitedNodes[i][j] = false;

		}
		startNode->previousNode = nullptr;
		activateBool(startNode->position);
		frontera.insert(std::pair<int, Node*>(Heuristics::manhatanDistance(Heuristics::pix2cell(startNode->position), endPos), startNode));
		notFound = true;
		
	}

	it = frontera.begin();
	std::pair <std::multimap<int, Node*>::iterator, std::multimap<int, Node*>::iterator> range;
	for (int i = 0; i < it->second->conexiones.size(); i++) {
		
		if (Heuristics::pix2cell(it->second->conexiones[i]->position) == endPos) {
			notFound = false;
			//llenar el camino
			//indicamos de donde proviene
			it->second->conexiones[i]->previousNode = it->second;
			fillPath(it->second->conexiones[i]);
		}
		else {
			//añadimos el nodo a la frontera si no esta ya
			temp = Heuristics::manhatanDistance(Heuristics::pix2cell(it->second->conexiones[i]->position), endPos);
			range = frontera.equal_range(temp);
			notFoundInFrontier = true;

			for (auto j = range.first; j != range.second; ++j) {
				if (j->second->position == it->second->conexiones[i]->position)
					notFoundInFrontier = false;
			}
			if (it->second->previousNode != nullptr) {
				if (checkVisited(it->second->conexiones[i]->position))
					notFoundInFrontier = false;
			}
			if (notFoundInFrontier) {
				//indicamos de donde proviene
				it->second->conexiones[i]->previousNode = it->second;
				frontera.insert(std::pair<int, Node*>(temp, it->second->conexiones[i]));
				activateBool(it->second->conexiones[i]->position);
			}
		}
	}
	it = frontera.erase(it);
}

void GFS::fillPath(Node* end) {
	path.clear();
	Node* node=end;
	while (node != nullptr) {
		path.push_back(node->position);
		node = node->previousNode;
	}
	Vector2D temp;
	for (int i = 0; i <= path.size() / 2; i++) {
		temp = path[i];
		path[i] = path[path.size() - 1 - i];
		path[path.size() - 1 - i] = temp;
	}
}

void GFS::fillPath(Node* end,int& numNodes) {
	path.clear();
	Node* node = end;
	while (node != nullptr) {
		path.push_back(node->position);
		node = node->previousNode;
		numNodes++;
	}
	Vector2D temp;
	for (int i = 0; i <= path.size() / 2; i++) {
		temp = path[i];
		path[i] = path[path.size() - 1 - i];
		path[path.size() - 1 - i] = temp;
	}
}

std::vector<Vector2D> GFS::Search(Node* startNode, Vector2D endPos) {
	frontera.clear();
	for (int i = 0; i < alreadyVisitedNodes.size(); i++) {
		for (int j = 0; j < alreadyVisitedNodes[i].size(); j++)
			alreadyVisitedNodes[i][j] = false;

	}
	//path.clear();
	startNode->previousNode = nullptr;
	activateBool(startNode->position);
	frontera.insert(std::pair<int, Node*>(Heuristics::manhatanDistance(Heuristics::pix2cell(startNode->position), endPos), startNode));
	notFound = true;
	
	//notFoundInFrontier;
	int temp;
	std::pair <std::multimap<int, Node*>::iterator, std::multimap<int, Node*>::iterator> range;
	while (!frontera.empty() && notFound) {
		it = frontera.begin();
		for (int i = 0; i < it->second->conexiones.size(); i++) {

			if (Heuristics::pix2cell(it->second->conexiones[i]->position) == endPos) {
				notFound = false;
				//llenar el camino
				//indicamos de donde proviene
				it->second->conexiones[i]->previousNode = it->second;
				fillPath(it->second->conexiones[i]);
			}
			else {
				//añadimos el nodo a la frontera si no esta ya
				temp = Heuristics::manhatanDistance(Heuristics::pix2cell(it->second->conexiones[i]->position), endPos);
				range = frontera.equal_range(temp);
				notFoundInFrontier = true;

				for (auto j = range.first; j != range.second; ++j) {
					if (j->second->position == it->second->conexiones[i]->position)
						notFoundInFrontier = false;
				}
				if (it->second->previousNode != nullptr) {
					if (checkVisited(it->second->conexiones[i]->position))

						notFoundInFrontier = false;
				}
				if (notFoundInFrontier) {
					//indicamos de donde proviene
					it->second->conexiones[i]->previousNode = it->second;
					frontera.insert(std::pair<int, Node*>(temp, it->second->conexiones[i]));
					activateBool(it->second->conexiones[i]->position);
				}
			}
		}
		it = frontera.erase(it);
	}

	return path;
}

std::vector<Vector2D> GFS::debugSearch(SceneDebugPF* scene,Node* startNode, Vector2D endPos) {
	scene->numNodesAddedToF = 0;
	scene->numNodesEvaluated = 0;
	scene->numNodesVisited = 0;
	scene->numPathNodes = 0;
	
	clock_t t = clock();

	frontera.clear();
	for (int i = 0; i < alreadyVisitedNodes.size(); i++) {
		for (int j = 0; j < alreadyVisitedNodes[i].size(); j++)
			alreadyVisitedNodes[i][j] = false;

	}
	//path.clear();
	startNode->previousNode = nullptr;
	activateBool(startNode->position);
	frontera.insert(std::pair<int, Node*>(Heuristics::manhatanDistance(Heuristics::pix2cell(startNode->position), endPos), startNode));
	notFound = true;

	//notFoundInFrontier;
	int temp;
	std::pair <std::multimap<int, Node*>::iterator, std::multimap<int, Node*>::iterator> range;
	while (!frontera.empty() && notFound) {
		scene->numNodesEvaluated++;
		it = frontera.begin();
		for (int i = 0; i < it->second->conexiones.size(); i++) {
			scene->numNodesVisited++;
			if (Heuristics::pix2cell(it->second->conexiones[i]->position) == endPos) {
				notFound = false;
				//llenar el camino
				//indicamos de donde proviene
				it->second->conexiones[i]->previousNode = it->second;
				fillPath(it->second->conexiones[i],scene->numPathNodes);
			}
			else {
				//añadimos el nodo a la frontera si no esta ya
				temp = Heuristics::manhatanDistance(Heuristics::pix2cell(it->second->conexiones[i]->position), endPos);
				range = frontera.equal_range(temp);
				notFoundInFrontier = true;

				for (auto j = range.first; j != range.second; ++j) {
					if (j->second->position == it->second->conexiones[i]->position)
						notFoundInFrontier = false;
				}
				if (it->second->previousNode != nullptr) {
					if (checkVisited(it->second->conexiones[i]->position))

						notFoundInFrontier = false;
				}
				if (notFoundInFrontier) {
					//indicamos de donde proviene
					it->second->conexiones[i]->previousNode = it->second;
					frontera.insert(std::pair<int, Node*>(temp, it->second->conexiones[i]));
					activateBool(it->second->conexiones[i]->position);
					scene->numNodesAddedToF++;
				}
			}
		}
		it = frontera.erase(it);
	}

	t = clock() - t;
	scene->timeOfSearch = clock();
	return path;
}