#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Vector2D.h"
#include "Node.h"
#include "utils.h"
#include "SDL_SimpleApp.h"
#include "Heuristics.h"
#include <time.h>
#include "SceneDebugPF.h"
#include "SceneEnemy.h"
class SceneEnemy;
class SceneDebugPF;

struct AstarCost {
	float acumulatedCost, heuristics;
	AstarCost(float cost, float heuristics):acumulatedCost(cost), heuristics(heuristics) {
	}
	AstarCost() {
		acumulatedCost = 0;
		heuristics = 0;
	}
};


struct Aestrella {
private:
	static std::multimap<AstarCost, Node*> frontier;
	static std::multimap<AstarCost, Node*>::iterator it;
	static std::vector<Vector2D> path;
	static std::vector<std::vector<bool>> visitedNodes;
	static void checkCost(Node*,Node*, AstarCost);
	static void checkCost(Node*, Node*, AstarCost, int&);
	static void activateBool(Vector2D);
	static bool notFound;
	static float pathCost; //util per a la cerca de grup

public:

	static void init(int, int);
	static void fillPath(Node*);
	static void fillPath(Node*,int&);
	static bool checkFrontier(Vector2D);
	static std::vector<Vector2D> search(Node*,Vector2D);
	static std::vector<Vector2D> search(SceneEnemy*, Node*, Vector2D);
	static std::vector<Vector2D> debugSearch(SceneDebugPF*,Node*, Vector2D);
	static void draw();
	static float getPathCost(); //util per a la cerca de grup
};
