#pragma once
#include <iostream>
#include <vector>
#include<map>
#include "Vector2D.h"
#include "Node.h"

struct dijkstra {
	static std::vector<Vector2D> applyDijkstra(Node* startNode, Vector2D endPos) {
		std::multimap<int, Node*> frontier;
		std::multimap<int, Node*>::iterator it;
		std::vector<Vector2D> path;
		startNode->previousNode = nullptr;
		Node cost;
		cost.pes = 0.0;
		bool notFound = true;

		while (!frontier.empty() && notFound) {
			it = frontier.begin();
			for (int i = 0; i < it->second->conexiones.size(); i++) {
				if (it->second->conexiones[i]->position == endPos) {
					break;
				}
				else {
					
				}
			}
		}

		return;
	}
};
