#pragma once
#include <vector>
#include "Vector2D.h"
#include "Node.h"
#include <map>
//#include "Heuristics.h"

struct GFS {	

	static void fillPath(std::vector<Vector2D>& path, Node* end) {	
		while (end!=nullptr) {
			path.push_back(end->position);
			end = end->previousNode;
		}
		Vector2D temp;
		for (int i = 0; i = path.size() / 2;i++) {
			temp = path[i];
			path[i] = path[path.size() - 1 - i];
			path[path.size() - 1 - i] = temp;
		}
	}

	static std::vector<Vector2D> Search(Node* startNode, Vector2D endPos) {
		std::multimap<int, Node*> frontera;
		std::multimap<int, Node*>::iterator it;
		std::vector<Vector2D> path;
		//frontera.insert(std::pair<int,Node*>(Heuristics::manhatanDistance(startNode->position,endPos),startNode));
		bool notFound = true;

		while (!frontera.empty() && notFound) {
			it = frontera.begin();
			for (int i = 0; i < it->second->conexiones.size(); i++) {
				if (it->second->conexiones[i]->position == endPos) {
					notFound = false;
					//llenar el camino
					fillPath(path, it->second->conexiones[i]);
				}
				else {
					//indicamos de donde proviene
					it->second->conexiones[i]->previousNode = it->second;
					//añadimos el nodo a la frontera
					//frontera.insert(std::pair<int, Node*>(Heuristics::manhatanDistance(it->second->conexiones[i]->position, endPos), it->second->conexiones[i]));	
				}
				frontera.erase(it);
			}
		}

		return path;
	}

};