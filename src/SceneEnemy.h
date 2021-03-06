#pragma once
#include <vector>
#include <time.h>
#include "Scene.h"
#include "Agent.h"
#include "Path.h"
#include "list"
#include "Node.h"
#include "Dijkstra.h"
#include "GreedyFirstSearch.h"
#include "utils.h"
#include "BFS.h"
#include "Aestrella.h"

class SceneEnemy :
	public Scene
{
	friend struct Heuristics;
public:
	SceneEnemy();
	~SceneEnemy();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();
private:
	//graph creation
	std::vector<std::vector<Node>> graph;
	int wallsOnCollumn(int);
	int wallsonCollumn(int, int);
	int v, h;
	int v1, h1, numDelNodes;
	int algorismType;
	bool throughTunnel = false;
	void createGraphPorPasos();
	void deleteNodesPorPaso();
	Node* findInGraph(Vector2D);
	void createGraph();
	//debugGraph
	void drawGraph();

	//debug busqueda
	void drawGraphConexions();

	std::vector<Agent*> agents;
	Vector2D coinPosition;
	Vector2D enemyTarget;
	Vector2D currentTarget;
	Vector2D currentEnemyTarget;
	bool hasChangedPath;
	bool throughTunnelEnemy;
	int currentTargetIndex;
	int currentTargetIndexEnemy;
	Path path;
	Path enemyPath;
	int num_cell_x;
	int num_cell_y;
	bool draw_grid;
	std::vector<SDL_Rect> maze_rects;
	void drawMaze();
	void drawCoin();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	void initMaze();
	bool loadTextures(char* filename_bg, char* filename_coin);
	std::vector< std::vector<int> > terrain;
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	bool isValidCell(Vector2D cell);
};
