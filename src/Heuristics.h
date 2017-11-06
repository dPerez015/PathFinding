#pragma once
#include "Vector2D.h"
#include "SDL_SimpleApp.h"

struct Heuristics {	
private:
	Vector2D pix2cell(Vector2D v) {
		return Vector2D((float)((int)v.x / CELL_SIZE), (float)((int)v.y / CELL_SIZE));
	}
public:
	static int manhatanDistance(Vector2D init, Vector2D fin) {
		//fin = pix2cell(fin);
		return abs(init.x - fin.x) + abs(init.y - fin.y);
	}
};