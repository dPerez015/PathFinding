#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include "Vector2D.h"
#include "Node.h"
#include "utils.h"
#include "SDL_SimpleApp.h"

struct dijkstra {
private:
	static std::multimap<float, Node*> frontier;
	static std::multimap<float, Node*>::iterator it;
	static std::vector<Vector2D> path;
	static bool notFound;
	static bool notInCostSoFar;

public:

	static void fillPath(std::vector<Vector2D>, Node*);
	static bool checkFrontier(Node*, std::multimap<float, Node*>);
	static std::vector<Vector2D> applyDijkstra(Node*,Vector2D);
	static void draw();
};
