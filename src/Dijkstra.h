#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include "Vector2D.h"
#include "Node.h"

struct dijkstra {
private:
	static std::multimap<float, Node*> frontier;
	static std::multimap<float, Node*>::iterator it;
	static std::vector<Vector2D> path;
	static bool notFound;
	static bool notInCostSoFar;

public:

	static void fillPath(std::vector<Vector2D>, Node*);
	static std::vector<Vector2D> applyDijkstra(Node*,Vector2D);
	
};
