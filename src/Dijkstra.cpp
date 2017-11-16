#include "Dijkstra.h"

std::multimap<float, Node*> dijkstra::frontier;
std::multimap<float, Node*>::iterator dijkstra::it;
std::vector<Vector2D> dijkstra::path;
std::vector<std::vector<bool>> dijkstra::visitedNodes;
bool dijkstra::notFound;

void dijkstra::initDijkstra(int x, int y) {
	for (int i = 0; i < x; i++) {
		std::vector<bool> tmp(y, false);
		visitedNodes.push_back(tmp);
	}
}

void dijkstra::draw() {
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

void dijkstra::fillPath(Node* end) {
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

bool dijkstra::checkFrontier( Vector2D tmp) {

//	std::multimap<float, Node*>::iterator checker;
	tmp = Heuristics::pix2cell(tmp);

	return visitedNodes[tmp.x][tmp.y];
}
void dijkstra::checkCost(Node* node, Node* previusNode, float tmp) {
	for (std::multimap<float, Node*>::iterator it = frontier.begin(); it != frontier.end(); it++) {
		if (it->second->position == node->position) {
			if (it->first > tmp) {
				
				it = frontier.erase(it);
				node->previousNode = previusNode;
				frontier.emplace(tmp, node);
				break;
			}
		}
	}
	
}
void dijkstra::activateBool(Vector2D pos) {
	pos = Heuristics::pix2cell(pos);
	visitedNodes[pos.x][pos.y]=true;
}

std::vector<Vector2D> dijkstra::applyDijkstra(Node* startNode, Vector2D endPos) {
	frontier.clear();
	path.clear();
	for (int i = 0; i < visitedNodes.size(); i++) {
		for (int j = 0; j < visitedNodes[i].size(); j++) {
			visitedNodes[i][j] = false;
		}
	}
	startNode->previousNode = nullptr;
	activateBool(startNode->position);
	frontier.emplace(0.0f, startNode);
	float temp;//comprovarà que el valor de la frontera sigui més petit que aquest
	notFound = true;
	std::pair <std::multimap<float, Node*>::iterator, std::multimap<float, Node*>::iterator> range;
	while (!frontier.empty() && notFound) {
		it = frontier.begin();
		for (int i = 0; i < it->second->conexiones.size(); i++) {
			if (Heuristics::pix2cell(it->second->conexiones[i]->position) == endPos) {
				notFound = false;
				it->second->conexiones[i]->previousNode = it->second;
				fillPath(it->second->conexiones[i]);
			}
			else {
				//frontier.insert(std::pair<float,Node*>(it->second->conexiones[i]->pes, it->second->conexiones[i]->position));
				//range = frontier.equal_range(it->second->conexiones[i]->pes);
				temp = it->first + it->second->conexiones[i]->pes;
				
				if (checkFrontier(it->second->conexiones[i]->position)) {
					checkCost(it->second->conexiones[i],it->second, temp);
				}
				else {
					activateBool(it->second->conexiones[i]->position);
					it->second->conexiones[i]->previousNode = it->second;
					frontier.insert(std::pair<float, Node*>(temp, it->second->conexiones[i]));
				}
			}
		}
		it = frontier.erase(it);
	}

	return path;
}