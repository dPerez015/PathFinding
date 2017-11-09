#include "Dijkstra.h"

std::multimap<float, Node*> dijkstra::frontier;
std::multimap<float, Node*>::iterator dijkstra::it;
std::vector<Vector2D> dijkstra::path;
bool dijkstra::notFound;
bool dijkstra::notInCostSoFar;

void dijkstra::fillPath(std::vector<Vector2D> path, Node* end) {
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

std::vector<Vector2D> dijkstra::applyDijkstra(Node* startNode, Vector2D endPos) {
	frontier.clear();
	path.clear();
	startNode->previousNode = nullptr;
	frontier.emplace(0.0f, startNode->previousNode);
	bool notFound = true;
	bool notInCostSoFar;
	std::pair <std::multimap<float, Node*>::iterator, std::multimap<float, Node*>::iterator> range;
	while (!frontier.empty() && notFound) {
		it = frontier.begin();
		for (int i = 0; i < it->second->conexiones.size(); i++) {
			if (it->second->conexiones[i]->position == endPos) {
				fillPath(path, it->second->conexiones[i]);
				notFound = false;
			}
			else {
				//frontier.insert(std::pair<float,Node*>(it->second->conexiones[i]->pes, it->second->conexiones[i]->position));
				range = frontier.equal_range(it->second->conexiones[i]->pes);
				notInCostSoFar = true;

				for (auto j = range.first; j != range.second; ++j) {
					if (j->second->pes == j->second->conexiones[i]->pes) {
						notInCostSoFar = false;
					}
				}
				if (notInCostSoFar) {
					frontier.insert(std::pair<float, Node*>(it->second->conexiones[i]->pes, it->second->conexiones[i]));
				}
			}//afegir key utilitzant pesis al multimap
		}
		it = frontier.erase(it);
	}

	return path;
}