#pragma once
#include "Vector2D.h"
#include <vector>
struct Node {
	Vector2D position;
	float pes;
	std::vector<Node*>conexiones;
	Node* previousNode;
};