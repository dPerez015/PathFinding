#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include "Vector2D.h"
#include "Node.h"

struct dijkstra {
	static void fillPath(std::vector<Vector2D>& path, Node* end) {
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

	static std::vector<Vector2D> applyDijkstra(Node* startNode, Vector2D endPos) {
		std::multimap<float, Node*> frontier;
		std::multimap<float, Node*>::iterator it;
		std::vector<Vector2D> path;
		startNode->previousNode = nullptr;
		frontier.emplace(0.0f, startNode->previousNode);
		bool notFound = true;
		bool notInCostSoFar = false;
		std::pair <std::multimap<int, Node*>::iterator, std::multimap<int, Node*>::iterator> range;
		while (!frontier.empty() && notFound) {
			it = frontier.begin();
			for (int i = 0; i < it->second->conexiones.size(); i++) {
				if (it->second->conexiones[i]->position == endPos) {
					fillPath(path, it->second->conexiones[i]);
					notFound = false;
				}
				else {

				}//afegir key utilitzant pesis al multimap


			}
			it = frontier.erase(it);
		}

		return path;
	}
};
