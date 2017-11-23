#pragma once
#include <vector>
#include "Vector2D.h"
#include "Node.h"
#include <map>
#include "Heuristics.h"
#include "utils.h"
#include "SceneDebugPF.h"
#include <time.h>

class SceneDebugPF;

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
	static void fillPath(Node*, int&);
	static std::vector<Vector2D>Search(Node*, Vector2D);
	static std::vector<Vector2D>debugSearch(SceneDebugPF*,Node*, Vector2D);
};