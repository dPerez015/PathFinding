#include "Dijkstra.h"

std::multimap<float, Node*> dijkstra::frontier;
std::multimap<float, Node*>::iterator dijkstra::it;
std::vector<Vector2D> dijkstra::path;
bool dijkstra::notFound;
bool dijkstra::inCostSoFar;
bool dijkstra::isLowerCost;

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
	for (int i = 0; i = path.size() / 2; i++) {
		temp = path[i];
		path[i] = path[path.size() - 1 - i];
		path[path.size() - 1 - i] = temp;
	}
}

bool dijkstra::checkFrontier(Node* pos, std::multimap<float, Node*> frontier) {

	std::multimap<float, Node*>::iterator checker;

	for (checker = frontier.begin(); checker != frontier.end(); checker++) {
		if (checker->first == pos->pes) {
			return true;
		}
	}

	return false;
}

std::vector<Vector2D> dijkstra::applyDijkstra(Node* startNode, Vector2D endPos) {
	frontier.clear();
	path.clear();
	startNode->previousNode = nullptr;
	frontier.emplace(0.0f, startNode);
	float temp;//comprovarà que el valor de la frontera sigui més petit que aquest
	notFound = true;
	std::pair <std::multimap<float, Node*>::iterator, std::multimap<float, Node*>::iterator> range;
	while (!frontier.empty() && notFound) {
		it = frontier.begin();
		for (int i = 0; i < it->second->conexiones.size(); i++) {
			if (it->second->conexiones[i]->position == endPos) {
				notFound = false;
				fillPath(it->second->conexiones[i]);
			}
			else {
				//frontier.insert(std::pair<float,Node*>(it->second->conexiones[i]->pes, it->second->conexiones[i]->position));
				range = frontier.equal_range(it->second->conexiones[i]->pes);
				temp = it->first + it->second->conexiones[i]->pes;
				inCostSoFar = checkFrontier(it->second->conexiones[i], frontier);
 
				if (inCostSoFar == false) {
					if (temp < it->second->conexiones[i]->pes)
						isLowerCost = true;
				}
				if (isLowerCost == true) {
					frontier.insert(std::pair<float, Node*>(temp, it->second->conexiones[i]));
				}
			}
		}
		it = frontier.erase(it);
	}

	return path;
}