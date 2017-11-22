#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include "Vector2D.h"
#include "Node.h"
#include "utils.h"
#include "SDL_SimpleApp.h"
#include "Heuristics.h"
struct dijkstra {
private:
	static std::multimap<float, Node*> frontier;
	static std::multimap<float, Node*>::iterator it;
	static std::vector<Vector2D> path;
	static std::vector<std::vector<bool>> visitedNodes;
	static void checkCost(Node*,Node*, float);
	static void activateBool(Vector2D);
	static bool notFound;

public:

	static void initDijkstra(int, int);
	static void fillPath(Node*);
	static bool checkFrontier(Vector2D);
	static std::vector<Vector2D> search(Node*,Vector2D);
	static void draw();
};
