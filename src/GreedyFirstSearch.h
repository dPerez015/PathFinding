#pragma once
#include <vector>
#include "Vector2D.h"
#include "Node.h"
#include <map>
#include "Heuristics.h"

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
		startNode->previousNode = nullptr;
		frontera.insert(std::pair<int,Node*>(Heuristics::manhatanDistance(Heuristics::pix2cell(startNode->position),endPos),startNode));
		bool notFound = true;
		bool notFoundInFrontier;
		int temp;
		std::pair <std::multimap<int, Node*>::iterator, std::multimap<int, Node*>::iterator> range;
		while (!frontera.empty() && notFound) {
			it = frontera.begin();
			for (int i = 0; i < it->second->conexiones.size(); i++) {
				if (Heuristics::pix2cell(it->second->conexiones[i]->position) == endPos) {
					notFound = false;
					//llenar el camino
					fillPath(path, it->second->conexiones[i]);
				}
				else {
					//indicamos de donde proviene
					it->second->conexiones[i]->previousNode = it->second;
					//añadimos el nodo a la frontera si no esta ya
					temp = Heuristics::manhatanDistance(Heuristics::pix2cell(it->second->conexiones[i]->position), endPos);
					range = frontera.equal_range(temp);
					notFoundInFrontier = true;

					for (auto j = range.first; j != range.second; ++j) {
						if (j->second->position == it->second->conexiones[i]->position)
							notFoundInFrontier = false;
					}
					if(notFoundInFrontier)
						frontera.insert(std::pair<int, Node*>(temp, it->second->conexiones[i]));
					
				}		
			}
			it = frontera.erase(it);
		}

		return path;
	}

};