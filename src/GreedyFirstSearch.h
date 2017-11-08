#pragma once
#include <vector>
#include "Vector2D.h"
#include "Node.h"
#include <map>
#include "Heuristics.h"
#include "utils.h"
struct GFS {	
private:
	static std::multimap<int, Node*> frontera;
	static std::multimap<int, Node*>::iterator it;
	static std::vector<Vector2D> path;
	static std::vector<std::vector<bool>> alreadyVisitedNodes;
	static bool notFound;
	static bool notFoundInFrontier;

public:
	static void activateBool(Vector2D);
	static bool checkVisited(Vector2D);
	static void initGFS(int, int);
	static void draw();
	static void SearchPerTick(Node*,Vector2D);
	static void fillPath(Node*);
	static std::vector<Vector2D>Search(Node*, Vector2D);

};