#pragma once
#include "Vector2D.h"
#include "SDL_SimpleApp.h"
#include "SceneEnemy.h"
class SceneEnemy;
struct Heuristics {	

	static Vector2D pix2cell(Vector2D v) {
		Vector2D ret= Vector2D((float)((int)v.x / CELL_SIZE), (float)((int)v.y / CELL_SIZE));
		return ret;
	}

	static int manhatanDistance(Vector2D init, Vector2D fin) {
		/*fin = pix2cell(fin);
		init = pix2cell(init);*/

		int ret1 = fin.x - init.x;
		if (abs(ret1)>=(SRC_WIDTH / (CELL_SIZE*2))) {
			if (ret1 < 0) {
				return abs((fin.x + (SRC_WIDTH / CELL_SIZE))-init.x) + abs(fin.y - init.y);
			}
			else {
				return abs(fin.x - (init.x+(SRC_WIDTH / CELL_SIZE))) + abs(fin.y - init.y);
			}
		}
		else {
			return abs(ret1) + abs(init.y - fin.y);
		}
		
	}

	static float enemyHeuristics(SceneEnemy* scene, Vector2D init, Vector2D fin) {
		int ret1 = fin.x - init.x;
		int manhatanPes;
		//manhatan distance
		if (abs(ret1) >= (SRC_WIDTH / (CELL_SIZE * 2))) {
			if (ret1 < 0) {
				manhatanPes= abs((fin.x + (SRC_WIDTH / CELL_SIZE)) - init.x) + abs(fin.y - init.y);
			}
			else {
				manhatanPes= abs(fin.x - (init.x + (SRC_WIDTH / CELL_SIZE))) + abs(fin.y - init.y);
			}
		}
		else {
			manhatanPes= abs(ret1) + abs(init.y - fin.y);
		}
		//pes per proximitat enemic
		float pesEnemy = 5 / (sqrt(manhatanDistance(init, pix2cell(scene->agents[1]->getPosition()))));

		return pesEnemy + manhatanPes;
	}

};