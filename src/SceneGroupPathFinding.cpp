#include "SceneGroupPathFinding.h"

using namespace std;

SceneGroupPathFinding::SceneGroupPathFinding()
{
	numberOfHills = 20;
	numberOfCoins = 5;

	draw_grid = false;
	srand((unsigned int)time(NULL));
	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze();
	createGraph();
	
	loadTextures("../res/maze.png", "../res/coin.png");

	

	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);


	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1, -1);
	while (!isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[0]->setPosition(cell2pix(rand_cell));

	//create random goals
	createNewCoins(numberOfCoins);

	// PathFollowing next Target
	currentTarget = Vector2D(0, 0);
	currentTargetIndex = -1;

	GFS::initGFS(num_cell_x, num_cell_y);
	dijkstra::initDijkstra(num_cell_x, num_cell_y);
	Aestrella::init(num_cell_x, num_cell_y);
	//path.points=GFS::Search(findInGraph(agents[0]->getPosition()), coinPosition);

}

void SceneGroupPathFinding::createNewCoins(int numberOfGoals) {
	coins.clear();
	// set the coin in a random cell (but at least 3 cells far from the agent)
	for (int i = 0; i < numberOfGoals; ++i) {
		coinPosition = Vector2D(-1, -1);
		while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, pix2cell(agents[0]->getPosition())) < 3) || isThereCoin(coinPosition))
			coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
		coins.push_back(coinPosition);
	}
}

int SceneGroupPathFinding::wallsOnCollumn(int column) {
	int ret = 0;
	for (int i = 0; i < num_cell_y; i++) {
		if (terrain[column][i] == 0) {
			ret++;
		}
	}
	return ret;
}

int SceneGroupPathFinding::wallsonCollumn(int column, int row) {
	int ret = 0;
	for (int i = row - 1; i >= 0; i--) {
		if (terrain[column][i] == 0)
			ret++;
	}
	return ret;
}



void SceneGroupPathFinding::createGraph() {
	for (int i = 0; i < num_cell_x; i++) {

		std::vector<Node> graphCol;
		for (int j = 0; j < num_cell_y; j++) {
			if (terrain[i][j] != 0) {
				Node node;
				node.position = cell2pix(Vector2D(i, j));
				node.pes = 1;
				node.previousNode = nullptr;
				graphCol.push_back(node);
			}
		}
		graph.push_back(graphCol);
	}

	int jGraph = 0;

	for (int i = 0; i < num_cell_x; i++) {
		jGraph = 0;
		for (int j = 0; j < num_cell_y; j++) {
			if (terrain[i][j] != 0) {

				if (terrain[(i + 1) % num_cell_x][j] != 0) {
					int wallsNextCollumn = wallsonCollumn((i + 1) % num_cell_x, j);
					graph[i][jGraph].conexiones.push_back(&graph[(i + 1) % num_cell_x][j - wallsNextCollumn]);
					graph[(i + 1) % num_cell_x][j - wallsNextCollumn].conexiones.push_back(&graph[i][jGraph]);
				}

				if (terrain[i][(j + 1) % num_cell_y] != 0) {
					graph[i][jGraph].conexiones.push_back(&graph[i][(jGraph + 1) % graph[i].size()]);
					graph[i][(jGraph + 1) % graph[i].size()].conexiones.push_back(&graph[i][jGraph]);
				}
				jGraph++;
			}
		}
	}
}

/*
Funci� que retorna el node en el graph d'una posicio determinada
*/
Node* SceneGroupPathFinding::findInGraph(Vector2D position) {
	Vector2D predictedPos = pix2cell(position);

	return &graph[predictedPos.x][predictedPos.y - wallsonCollumn(predictedPos.x, predictedPos.y)];
}

void SceneGroupPathFinding::drawGraph() {
	for (int i = 0; i < num_cell_x; i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			draw_circle(TheApp::Instance()->getRenderer(), graph[i][j].position.x, graph[i][j].position.y, (CELL_SIZE - 1) / 2, 0, 255, 255, 255);
			for (int h = 0; h < graph[i][j].conexiones.size(); h++)
				SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), graph[i][j].position.x, graph[i][j].position.y, graph[i][j].conexiones[h]->position.x, graph[i][j].conexiones[h]->position.y);
		}
	}
}


SceneGroupPathFinding::~SceneGroupPathFinding()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneGroupPathFinding::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		else if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
			Path auxPath;
			Path shortestPath;
			Node* newAgentStartPosition = findInGraph(agents[0]->getPosition());
			path.points.clear();
			vector<Vector2D> goals = coins;
			while (!goals.empty()) {
				for (int i = 0; i < goals.size(); i++) {
					auxPath.points = Aestrella::search(newAgentStartPosition, goals[i]); //en realitat no volem el path, sino el cost acumulat al Node desti (falta fe-ho)
					if (i == 0) {
						closestCoinCost = Aestrella::getPathCost(); // el cost tret del primer search
						closestCoinIndex = 0;
						shortestPath = auxPath;
					}
					else	if (Aestrella::getPathCost() < closestCoinCost) {
						closestCoinCost = Aestrella::getPathCost();
						closestCoinIndex = i;
						shortestPath = auxPath;
					}
				}
				newAgentStartPosition = findInGraph(cell2pix(goals[closestCoinIndex]));
				goals.erase(goals.begin() + closestCoinIndex);
				path.points.insert(path.points.end(), shortestPath.points.begin(), shortestPath.points.end());
			} //al final del while esta el path complet que l'agent recorrer�
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_P) {
			path.points.clear();
			currentTargetIndex = 0;
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_N)
			//deleteNodesPorPaso();
			//BFS::searchPerTick(findInGraph(agents[0]->getPosition()), coinPosition);
			break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			Vector2D cell = pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));
			if (isValidCell(cell))
			{
				if (path.points.size() > 0)
					if (path.points[path.points.size() - 1] == cell2pix(cell))
						break;

				path.points.push_back(cell2pix(cell));
			}
		}
		break;
	default:
		break;
	}

	if ((currentTargetIndex == -1) && (path.points.size() > 0))
		currentTargetIndex = 0;

	if (currentTargetIndex >= 0)
	{
		float dist = Vector2D::Distance(agents[0]->getPosition(), path.points[currentTargetIndex]);
		if (dist < path.ARRIVAL_DISTANCE)
		{
			if (throughTunnel) throughTunnel = false;

			if (currentTargetIndex == path.points.size() - 1)
			{
				if (dist < 3)
				{
					// if we have arrived to the coin, replace it ina random cell!
					shouldDeleteCoin(pix2cell(path.points[currentTargetIndex])); //si hi ha una moneda la elimina
					path.points.clear();
					currentTargetIndex = -1;
					agents[0]->setVelocity(Vector2D(0, 0));
				}
				else
				{
					Vector2D steering_force = agents[0]->Behavior()->Arrive(agents[0], currentTarget, path.ARRIVAL_DISTANCE, dtime);
					agents[0]->update(steering_force, dtime, event);
				}
				return;
			}
			//tunnel stuff
			//else -> el target no es l'ultim target del path
			//si ha de sortir de la dreta
			else if (pix2cell(path.points[currentTargetIndex]).x == num_cell_x - 1 && pix2cell(path.points[currentTargetIndex + 1]).x == 0) {
				currentTarget.x += CELL_SIZE;
				throughTunnel = true;
			}//si ha de sortir de l'esquerra
			else if (pix2cell(path.points[currentTargetIndex]).x == 0 && pix2cell(path.points[currentTargetIndex + 1]).x == num_cell_x - 1) {
				currentTarget.x -= CELL_SIZE;
				throughTunnel = true;
			}
			else {		
				//esborrar coins
				shouldDeleteCoin(pix2cell(path.points[currentTargetIndex])); //si hi ha moneda la elimina
			}
			currentTargetIndex++;
		}

		if (!throughTunnel) {
			currentTarget = path.points[currentTargetIndex];
		}
		Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], currentTarget, dtime);
		agents[0]->update(steering_force, dtime, event);
	}
	else
	{
		agents[0]->update(Vector2D(0, 0), dtime, event);
	}
}
void SceneGroupPathFinding::drawGraphConexions() {
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			if (graph[i][j].previousNode != nullptr)
				SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), graph[i][j].position.x, graph[i][j].position.y, graph[i][j].previousNode->position.x, graph[i][j].previousNode->position.y);
		}

	}
}

void SceneGroupPathFinding::draw()
{
	drawMaze();
	drawHills();
	drawCoin();


	if (draw_grid)
	{

		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);

		//drawGraph();
		//GFS::draw();
		//dijkstra::draw();
		BFS::draw();
		drawGraphConexions();
		for (int i = 0; i < SRC_WIDTH; i += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	for (int i = 0; i < (int)path.points.size(); i++)
	{
		draw_circle(TheApp::Instance()->getRenderer(), (int)(path.points[i].x), (int)(path.points[i].y), 15, 255, 255, 0, 255);
		if (i > 0)
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
	}

	draw_circle(TheApp::Instance()->getRenderer(), (int)currentTarget.x, (int)currentTarget.y, 15, 255, 0, 0, 255);

	agents[0]->draw();
}

const char* SceneGroupPathFinding::getTitle()
{
	return "SDL Steering Behaviors :: PathFinding3 Demo";
}

void SceneGroupPathFinding::drawMaze()
{
	if (draw_grid)
	{

		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
		for (unsigned int i = 0; i < maze_rects.size(); i++)
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &maze_rects[i]);
	}
	else
	{
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL);
	}
}

void SceneGroupPathFinding::drawCoin()
{
	Vector2D coin_coords;
	int offset = CELL_SIZE / 2;
	for (int i = 0; i < coins.size(); ++i) {
		coin_coords = cell2pix(coins[i]);
		SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
	}
	
}

void SceneGroupPathFinding::drawHills() {
	float alphaConversor = (float)255 / maxWeight; //alpha maxim ser� el pes maxim
	SDL_Rect auxRect;
	for (int i = 0; i < num_cell_x; ++i) {
		for (int j = 0; j < num_cell_y; ++j) {
			if (terrain[i][j] > 1) { //totes les cells per defecte tenen pes 1, no volem que s'hi pinti res
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, terrain[i][j] * alphaConversor, 0, 255);
				Vector2D auxCellPos = cell2pix(Vector2D(i, j));
				auxRect = { (int)auxCellPos.x - CELL_SIZE / 2,(int)auxCellPos.y - CELL_SIZE / 2,CELL_SIZE,CELL_SIZE };
				SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &auxRect);

			}
		}
	}
}

void SceneGroupPathFinding::initMaze()
{

	// Initialize a list of Rectagles describing the maze geometry (useful for collision avoidance)
	SDL_Rect rect = { 0, 0, 1280, 32 };
	maze_rects.push_back(rect);
	rect = { 608, 32, 64, 32 };
	maze_rects.push_back(rect);
	rect = { 0, 736, 1280, 32 };
	maze_rects.push_back(rect);
	rect = { 608, 512, 64, 224 };
	maze_rects.push_back(rect);
	rect = { 0,32,32,288 };
	maze_rects.push_back(rect);
	rect = { 0,416,32,320 };
	maze_rects.push_back(rect);
	rect = { 1248,32,32,288 };
	maze_rects.push_back(rect);
	rect = { 1248,416,32,320 };
	maze_rects.push_back(rect);
	rect = { 128,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 288,128,96,32 };
	maze_rects.push_back(rect);
	rect = { 480,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 736,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 896,128,96,32 };
	maze_rects.push_back(rect);
	rect = { 1088,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 128,256,64,32 };
	maze_rects.push_back(rect);
	rect = { 288,256,96,32 };
	maze_rects.push_back(rect);
	rect = { 480, 256, 320, 32 };
	maze_rects.push_back(rect);
	rect = { 608, 224, 64, 32 };
	maze_rects.push_back(rect);
	rect = { 896,256,96,32 };
	maze_rects.push_back(rect);
	rect = { 1088,256,64,32 };
	maze_rects.push_back(rect);
	rect = { 128,384,32,256 };
	maze_rects.push_back(rect);
	rect = { 160,512,352,32 };
	maze_rects.push_back(rect);
	rect = { 1120,384,32,256 };
	maze_rects.push_back(rect);
	rect = { 768,512,352,32 };
	maze_rects.push_back(rect);
	rect = { 256,640,32,96 };
	maze_rects.push_back(rect);
	rect = { 992,640,32,96 };
	maze_rects.push_back(rect);
	rect = { 384,544,32,96 };
	maze_rects.push_back(rect);
	rect = { 480,704,32,32 };
	maze_rects.push_back(rect);
	rect = { 768,704,32,32 };
	maze_rects.push_back(rect);
	rect = { 864,544,32,96 };
	maze_rects.push_back(rect);
	rect = { 320,288,32,128 };
	maze_rects.push_back(rect);
	rect = { 352,384,224,32 };
	maze_rects.push_back(rect);
	rect = { 704,384,224,32 };
	maze_rects.push_back(rect);
	rect = { 928,288,32,128 };
	maze_rects.push_back(rect);

	// Initialize the terrain matrix (for each cell a zero value indicates it's a wall)

	// (1st) initialize all cells to 1 by default
	for (int i = 0; i < num_cell_x; i++)
	{
		vector<int> terrain_col(num_cell_y, 1);
		terrain.push_back(terrain_col);
	}

	maxWeight = 1;

	//posar a random position uns quants cims(mes pes)
	createHill(numberOfHills);

	// (2nd) set to zero all cells that belong to a wall
	int offset = CELL_SIZE / 2;
	for (int i = 0; i < num_cell_x; i++)
	{
		for (int j = 0; j < num_cell_y; j++)
		{
			Vector2D cell_center((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset));
			for (unsigned int b = 0; b < maze_rects.size(); b++)
			{
				if (Vector2DUtils::IsInsideRect(cell_center, (float)maze_rects[b].x, (float)maze_rects[b].y, (float)maze_rects[b].w, (float)maze_rects[b].h))
				{
					terrain[i][j] = 0;

					break;
				}
			}

		}
	}

}

bool SceneGroupPathFinding::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface *image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

Vector2D SceneGroupPathFinding::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D SceneGroupPathFinding::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x / CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool SceneGroupPathFinding::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.x >= terrain.size()) || (cell.y >= terrain[0].size()))
		return false;
	if (terrain[(unsigned int)cell.x][(unsigned int)cell.y] == 0) return false;
	return true;
}

bool SceneGroupPathFinding::isThereCoin(Vector2D cell) {
	if (!coins.empty()) {
		for (int i = 0; i < coins.size(); ++i) {
			if (cell == coins[i]) return true;
		}
	}
	return false;
}

void SceneGroupPathFinding::createHill(int numberOfHills) {
	Vector2D hillPos;
	for (int n = 0; n < numberOfHills; ++n) {
		hillPos = Vector2D((float)((rand() % (num_cell_x - 6)) + 3), (float)((rand() % (num_cell_y - 6)) + 3));

		for (int i = hillPos.x - 3; i <= hillPos.x + 3; ++i) {
			for (int j = hillPos.y - 3; j <= hillPos.y + 3; ++j) {
				int manhDist = Heuristics::manhatanDistance(Vector2D(i, j), hillPos);
				if (manhDist < 4) {
					terrain[i][j] += (4 - manhDist) * 5; //li suma 10 al pes per cada casella que s'apropa al centre
					if (terrain[i][j] > maxWeight) maxWeight = terrain[i][j]; //actualitzar maxWeight (serveix per pintar els pesos despres)
				}
			}
		}

	}
}

void SceneGroupPathFinding::shouldDeleteCoin(Vector2D cell) {
	if (isThereCoin(cell)) {
		if (coins.size() == 1) {
			createNewCoins(numberOfCoins);
		}
		else {
			for (int i = 0; i < coins.size(); ++i) {
				if (cell == coins[i]) {
					coins.erase(coins.begin() + i);
				}
			}
		}
	}
}