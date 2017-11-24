#pragma once
#include "Vector2D.h"
#include "SDL_SimpleApp.h"
#include "SceneEnemy.h"

class  SceneEnemy;
struct Heuristics {	

	static Vector2D pix2cell(Vector2D v) {
		Vector2D ret= Vector2D((float)((int)v.x / CELL_SIZE), (float)((int)v.y / CELL_SIZE));
		return ret;
	}

	static int manhatanDistance(Vector2D init, Vector2D fin);

	static float enemyHeuristics(SceneEnemy* scene, Vector2D init, Vector2D fin);

};