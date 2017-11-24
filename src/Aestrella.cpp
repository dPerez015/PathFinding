#include "Aestrella.h"

std::multimap<AstarCost, Node*> Aestrella::frontier;
std::multimap<AstarCost, Node*>::iterator Aestrella::it;
std::vector<Vector2D> Aestrella::path;
std::vector<std::vector<bool>> Aestrella::visitedNodes;
bool Aestrella::notFound;
float Aestrella::pathCost;


bool operator <(const AstarCost & l, const AstarCost & r) {
	return (l.acumulatedCost + l.heuristics) < (r.acumulatedCost + r.heuristics);
}
bool operator !=(const AstarCost& l, const AstarCost& r) {
	if (l.acumulatedCost == r.acumulatedCost && l.heuristics == r.heuristics) {
		return false;
	}
	return true;
}

void Aestrella::init(int x, int y) {
	for (int i = 0; i < x; i++) {
		std::vector<bool> tmp(y, false);
		visitedNodes.push_back(tmp);
	}
}

void Aestrella::draw() {
	it = frontier.begin();
	if (it != frontier.end()) {
		draw_circle(TheApp::Instance()->getRenderer(), it->second->position.x, it->second->position.y, TheApp::Instance()->getGridCellSize().x / 2, 0, 255, 0, 255);
		//SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), it->second->position.x, it->second->position.y, it->second->previousNode->position.x, it->second->previousNode->position.y);
		it++;
	}
	while (it != frontier.end()) {
		draw_circle(TheApp::Instance()->getRenderer(), it->second->position.x, it->second->position.y, TheApp::Instance()->getGridCellSize().x / 2, 255, 255, 0, 255);
		//SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), it->second->position.x, it->second->position.y, it->second->previousNode->position.x, it->second->previousNode->position.y);
		it++;
	}
	for (int i = 0; i < path.size(); i++) {
		draw_circle(TheApp::Instance()->getRenderer(), path[i].x, path[i].y, TheApp::Instance()->getGridCellSize().x / 2, 255, 0, 0, 255);
	}
}

void Aestrella::fillPath(Node* end) {
	path.clear();
	while (end != nullptr) {
		path.push_back(end->position);
		end = end->previousNode;
	}
	Vector2D temp;
	for (int i = 0; i < path.size() / 2; i++) {
		temp = path[i];
		path[i] = path[path.size() - 1 - i];
		path[path.size() - 1 - i] = temp;
	}
}
void Aestrella::fillPath(Node* end, int& numNodes) {
	path.clear();
	while (end != nullptr) {
		path.push_back(end->position);
		numNodes++;
		end = end->previousNode;
	}
	Vector2D temp;
	for (int i = 0; i < path.size() / 2; i++) {
		temp = path[i];
		path[i] = path[path.size() - 1 - i];
		path[path.size() - 1 - i] = temp;
	}
}

bool Aestrella::checkFrontier( Vector2D tmp) {

//	std::multimap<float, Node*>::iterator checker;
	tmp = Heuristics::pix2cell(tmp);

	return visitedNodes[tmp.x][tmp.y];
}
void Aestrella::checkCost(Node* node, Node* previusNode, AstarCost tmp) {

	for (std::multimap<AstarCost, Node*>::iterator it = frontier.begin(); it != frontier.end(); it++) {
		if (it->second->position == node->position) {
			if (it->first.acumulatedCost < tmp.acumulatedCost) {
				
				it = frontier.erase(it);
				node->previousNode = previusNode;
				frontier.emplace(tmp, node);
				break;
			}
		}
	}
	
}
void Aestrella::checkCost(Node* node, Node* previusNode, AstarCost tmp, int& nodesAdded) {

	for (std::multimap<AstarCost, Node*>::iterator it = frontier.begin(); it != frontier.end(); it++) {
		if (it->second->position == node->position) {
			if (it->first.acumulatedCost < tmp.acumulatedCost) {
				nodesAdded++;
				it = frontier.erase(it);
				node->previousNode = previusNode;
				frontier.emplace(tmp, node);
				break;
			}
		}
	}

}
void Aestrella::activateBool(Vector2D pos) {
	pos = Heuristics::pix2cell(pos);
	visitedNodes[pos.x][pos.y]=true;
}

std::vector<Vector2D> Aestrella::search(Node* startNode, Vector2D endPos) {
	frontier.clear();
	path.clear();
	pathCost = 0;
	for (int i = 0; i < visitedNodes.size(); i++) {
		for (int j = 0; j < visitedNodes[i].size(); j++) {
			visitedNodes[i][j] = false;
		}
	}
	startNode->previousNode = nullptr;
	activateBool(startNode->position);
	frontier.emplace(AstarCost(0,0), startNode);
	AstarCost temp;//comprovarà que el valor de la frontera sigui més petit que aquest
	notFound = true;
	
	while (!frontier.empty() && notFound) {
		it = frontier.begin();
		for (int i = 0; i < it->second->conexiones.size(); i++) {
			if (Heuristics::pix2cell(it->second->conexiones[i]->position) == endPos) {
				pathCost = it->first.acumulatedCost + it->second->conexiones[i]->pes;
				notFound = false;
				it->second->conexiones[i]->previousNode = it->second;
				fillPath(it->second->conexiones[i]);
			}
			else {
				//frontier.insert(std::pair<float,Node*>(it->second->conexiones[i]->pes, it->second->conexiones[i]->position));
				//range = frontier.equal_range(it->second->conexiones[i]->pes);
				temp.acumulatedCost = it->first.acumulatedCost + it->second->conexiones[i]->pes;
				temp.heuristics = Heuristics::manhatanDistance(Heuristics::pix2cell(it->second->conexiones[i]->position), endPos);
				
				if (checkFrontier(it->second->conexiones[i]->position)) {
					checkCost(it->second->conexiones[i],it->second, temp);
				}
				else {
					activateBool(it->second->conexiones[i]->position);
					it->second->conexiones[i]->previousNode = it->second;
					frontier.insert(std::pair<AstarCost, Node*>(temp, it->second->conexiones[i]));
				}
			}
		}
		it = frontier.erase(it);
	}

	return path;
}

std::vector<Vector2D> Aestrella::search(SceneEnemy* scene, Node* startNode, Vector2D endPos) {
	frontier.clear();
	path.clear();
	pathCost = 0;
	for (int i = 0; i < visitedNodes.size(); i++) {
		for (int j = 0; j < visitedNodes[i].size(); j++) {
			visitedNodes[i][j] = false;
		}
	}
	startNode->previousNode = nullptr;
	activateBool(startNode->position);
	frontier.emplace(AstarCost(0, 0), startNode);
	AstarCost temp;//comprovarà que el valor de la frontera sigui més petit que aquest
	notFound = true;

	while (!frontier.empty() && notFound) {
		it = frontier.begin();
		for (int i = 0; i < it->second->conexiones.size(); i++) {
			if (Heuristics::pix2cell(it->second->conexiones[i]->position) == endPos) {
				pathCost = it->first.acumulatedCost + it->second->conexiones[i]->pes;
				notFound = false;
				it->second->conexiones[i]->previousNode = it->second;
				fillPath(it->second->conexiones[i]);
			}
			else {
				//frontier.insert(std::pair<float,Node*>(it->second->conexiones[i]->pes, it->second->conexiones[i]->position));
				//range = frontier.equal_range(it->second->conexiones[i]->pes);
				temp.acumulatedCost = it->first.acumulatedCost + it->second->conexiones[i]->pes;
				temp.heuristics = Heuristics::enemyHeuristics(scene,Heuristics::pix2cell(it->second->conexiones[i]->position), endPos);

				if (checkFrontier(it->second->conexiones[i]->position)) {
					checkCost(it->second->conexiones[i], it->second, temp);
				}
				else {
					activateBool(it->second->conexiones[i]->position);
					it->second->conexiones[i]->previousNode = it->second;
					frontier.insert(std::pair<AstarCost, Node*>(temp, it->second->conexiones[i]));
				}
			}
		}
		it = frontier.erase(it);
	}

	return path;
}

std::vector<Vector2D> Aestrella::debugSearch(SceneDebugPF* scene,Node* startNode, Vector2D endPos) {
	scene->numNodesEvaluated = 0;
	scene->numNodesVisited = 0;
	scene->numNodesAddedToF = 0;
	scene->numPathNodes = 0;
	clock_t t = clock();

	frontier.clear();
	path.clear();
	for (int i = 0; i < visitedNodes.size(); i++) {
		for (int j = 0; j < visitedNodes[i].size(); j++) {
			visitedNodes[i][j] = false;
		}
	}
	startNode->previousNode = nullptr;
	activateBool(startNode->position);
	frontier.emplace(AstarCost(0, 0), startNode);
	AstarCost temp;//comprovarà que el valor de la frontera sigui més petit que aquest
	notFound = true;

	while (!frontier.empty() && notFound) {
		it = frontier.begin();
		scene->numNodesEvaluated++;
		for (int i = 0; i < it->second->conexiones.size(); i++) {
			scene->numNodesVisited++;
			if (Heuristics::pix2cell(it->second->conexiones[i]->position) == endPos) {
				notFound = false;
				it->second->conexiones[i]->previousNode = it->second;
				fillPath(it->second->conexiones[i],scene->numPathNodes);
			}
			else {
				//frontier.insert(std::pair<float,Node*>(it->second->conexiones[i]->pes, it->second->conexiones[i]->position));
				//range = frontier.equal_range(it->second->conexiones[i]->pes);
				temp.acumulatedCost = it->first.acumulatedCost + it->second->conexiones[i]->pes;
				temp.heuristics = Heuristics::manhatanDistance(Heuristics::pix2cell(it->second->conexiones[i]->position), endPos);

				if (checkFrontier(it->second->conexiones[i]->position)) {
					checkCost(it->second->conexiones[i], it->second, temp,scene->numNodesAddedToF);
				}
				else {
					scene->numNodesAddedToF++;
					activateBool(it->second->conexiones[i]->position);
					it->second->conexiones[i]->previousNode = it->second;
					frontier.insert(std::pair<AstarCost, Node*>(temp, it->second->conexiones[i]));
				}
			}
		}
		it = frontier.erase(it);
	}
	t = clock() - t;
	scene->timeOfSearch = t;
	return path;
}

float Aestrella::getPathCost() { return pathCost; }