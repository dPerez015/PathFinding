#pragma once
#include <iostream>
#include <vector>
#include "Vector2D.h"
#include <queue>
#include <deque>
#include "Node.h"
#include "Heuristics.h"
#include "utils.h"
#include "SceneDebugPF.h"
#include <time.h>

using namespace std;

class SceneDebugPF;

template<typename T, typename Container = std::deque<T> >
class iterable_queue : public std::queue<T, Container>
{
public:
	typedef typename Container::iterator iterator;
	typedef typename Container::const_iterator const_iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
	const_iterator begin() const { return this->c.begin(); }
	const_iterator end() const { return this->c.end(); }
};

class BFS {
private:	
	static queue<Node*> frontier;
	static iterable_queue<Vector2D> iterableFrontier;
	static vector<vector<bool>> visitedNode;
	static vector<Vector2D> path;
	static bool notFound;
	
public:
	static vector<Vector2D> search(Node*, Vector2D);
	static vector<Vector2D> debugSearch(SceneDebugPF*,Node*, Vector2D);
	static void searchPerTick(Node*, Vector2D); 
	static void draw();
	static void BFSinit(Node*);
	static void expandFrontier(Node*);
	static void expandFrontier(Node*, int&, int&);
	static bool isVisited(Node*);
	static vector<Vector2D> fillPath(Node*);
	static vector <Vector2D> fillPath(Node*, int&);
};
