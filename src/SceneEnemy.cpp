#include "SceneEnemy.h"

using namespace std;


SceneEnemy::SceneEnemy()
{
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze();
	createGraph();
	/*v = 0;
	h = 0;
	v1 = 0;
	h1 = 0;*/
	numDelNodes = 0;
	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));

	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);

	Agent* enemy = new Agent;
	enemy->loadSpriteTexture("../res/zombie1.png", 8);
	agents.push_back(enemy);

	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1,-1);
	while (!isValidCell(rand_cell)) 
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[0]->setPosition(cell2pix(rand_cell));
	
	
	Vector2D rand_cell2 = (-1,-1);
	while(!isValidCell(rand_cell2))
		rand_cell2 = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[1]->setPosition(cell2pix(rand_cell2));
	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1,-1);
	while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell)<3)) 
		coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	
	//targetEnemy
	enemyTarget = Vector2D(-1, -1);
	while(!isValidCell(enemyTarget))
		enemyTarget= Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));

	// PathFollowing next Target
	currentTarget = Vector2D(0, 0);
	currentTargetIndex = -1;

	GFS::initGFS(num_cell_x, num_cell_y);
	dijkstra::initDijkstra(num_cell_x, num_cell_y);
	Aestrella::init(num_cell_x, num_cell_y);
	

	currentEnemyTarget = Vector2D(0, 0);
	currentTargetIndexEnemy = -1;
	enemyPath.points = Aestrella::search(findInGraph(agents[1]->getPosition()), enemyTarget);

	path.points = Aestrella::search(this,findInGraph(agents[0]->getPosition()), coinPosition);
}

int SceneEnemy::wallsOnCollumn(int column) {
	int ret=0;
	for (int i = 0; i < num_cell_y; i++) {
		if (terrain[column][i] == 0) {
			ret++;
		}
	}
	return ret;
}

int SceneEnemy::wallsonCollumn(int column, int row) {
	int ret = 0;
	for (int i = row - 1; i >= 0; i--) {
		if (terrain[column][i] == 0)
			ret++;
	}
	return ret;
}



void SceneEnemy::createGraph() {
	for (int i = 0; i < num_cell_x; i++) {
		
		std::vector<Node> graphCol;
		for (int j = 0; j < num_cell_y; j++) {
			if (terrain[i][j] != 0) {
				Node node;
				node.position = cell2pix(Vector2D(i, j));
				node.pes = 1;
				node.previousNode=nullptr;
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

				if (terrain[(i + 1) % num_cell_x][j] == 1) {
					int wallsNextCollumn = wallsonCollumn((i + 1)%num_cell_x, j);
					graph[i][jGraph].conexiones.push_back(&graph[(i + 1) % num_cell_x][j-wallsNextCollumn]);
					graph[(i + 1) % num_cell_x][j - wallsNextCollumn].conexiones.push_back(&graph[i][jGraph]);
				}

				if (terrain[i][(j + 1) % num_cell_y] == 1) {
					graph[i][jGraph].conexiones.push_back(&graph[i][(jGraph + 1) % graph[i].size()]);
					graph[i][(jGraph + 1) %  graph[i].size()].conexiones.push_back(&graph[i][jGraph]);
				}
				jGraph++;
			}
		}
	}
	/*for (int i = 0; i < num_cell_x; i++) {
		int nodesErased = 0;
		for (int j = 0; j < num_cell_y-nodesErased; j++) {
			if (graph[i][j].conexiones.empty()) {
				nodesErased++;
				graph[i].erase(graph[i].begin() + j);
				j--;
			}
		}
	}*/
}


/*
Funció que retorna el node en el graph d'una posicio determinada
*/
Node* SceneEnemy::findInGraph(Vector2D position) {
	Vector2D predictedPos=pix2cell(position);
	
	return &graph[predictedPos.x][predictedPos.y - wallsonCollumn(predictedPos.x, predictedPos.y)];
}

void SceneEnemy::drawGraph() {
	for (int i = 0; i < num_cell_x; i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			draw_circle(TheApp::Instance()->getRenderer(), graph[i][j].position.x, graph[i][j].position.y, (CELL_SIZE - 1) / 2, 0, 255, 255,255);
			for (int h = 0; h < graph[i][j].conexiones.size(); h++)
				SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), graph[i][j].position.x, graph[i][j].position.y, graph[i][j].conexiones[h]->position.x, graph[i][j].conexiones[h]->position.y);
		}
	}
}


SceneEnemy::~SceneEnemy()
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

void SceneEnemy::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
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
	if ((currentTargetIndex == -1) && (path.points.size()>0))
		currentTargetIndex = 0;

	if (currentTargetIndex >= 0)
	{	
		//player
		float dist = Vector2D::Distance(agents[0]->getPosition(), path.points[currentTargetIndex]);
		if (dist < path.ARRIVAL_DISTANCE)
		{
			if (throughTunnel) throughTunnel = false;
			if (currentTargetIndex == path.points.size() - 1)
			{
				if (dist < 3)
				{
					path.points.clear();
					currentTargetIndex = -1;
					agents[0]->setVelocity(Vector2D(0, 0));
					// if we have arrived to the coin, replace it ina random cell!
					if (pix2cell(agents[0]->getPosition()) == coinPosition)
					{
						coinPosition = Vector2D(-1, -1);
						while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, pix2cell(agents[0]->getPosition())) < 3))
							coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
						path.points = Aestrella::search(this, findInGraph(agents[0]->getPosition()), coinPosition);
						//path.points = GFS::Search(findInGraph(agents[0]->getPosition()), coinPosition);
					}

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
			currentTargetIndex++;

		}
		else if (Heuristics::manhatanDistance(pix2cell(agents[0]->getPosition()), pix2cell(agents[1]->getPosition())) < 6) {
			currentTargetIndex = 0;
			path.points = Aestrella::search(this,findInGraph(agents[0]->getPosition()), coinPosition);
		}
		if (!throughTunnel) {
			currentTarget = path.points[currentTargetIndex];
		}

		Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], currentTarget, dtime);
		agents[0]->update(steering_force, dtime, event);
	} 
	else
	{
		agents[0]->update(Vector2D(0,0), dtime, event);
	}

	//enemy

	if ((currentTargetIndexEnemy == -1) && (enemyPath.points.size()>0))
		currentTargetIndexEnemy = 0;

	if (currentTargetIndexEnemy>=0) {



		float distEnemy = Vector2D::Distance(agents[1]->getPosition(), enemyPath.points[currentTargetIndexEnemy]);

		if (distEnemy < path.ARRIVAL_DISTANCE)
		{
			if (throughTunnelEnemy) throughTunnelEnemy = false;
			if (currentTargetIndexEnemy == enemyPath.points.size() - 1)
			{
				if (distEnemy < 3)
				{
					enemyPath.points.clear();
					currentTargetIndexEnemy = -1;
					agents[1]->setVelocity(Vector2D(0, 0));
					// if we have arrived to the coin, replace it ina random cell!
					if (pix2cell(agents[1]->getPosition()) == enemyTarget)
					{
						enemyTarget = Vector2D(-1, -1);
						while ((!isValidCell(enemyTarget)) || (Vector2D::Distance(enemyTarget, pix2cell(agents[1]->getPosition())) < 3))
							enemyTarget = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
						enemyPath.points = Aestrella::search(findInGraph(agents[1]->getPosition()), enemyTarget);
						//path.points = GFS::Search(findInGraph(agents[0]->getPosition()), coinPosition);
					}


				}
				else
				{
					Vector2D steering_force = agents[1]->Behavior()->Arrive(agents[1], currentEnemyTarget, enemyPath.ARRIVAL_DISTANCE, dtime);
					agents[1]->update(steering_force, dtime, event);
				}
				return;
			}
			//tunnel stuff
			//else -> el target no es l'ultim target del path
			//si ha de sortir de la dreta
			else if (pix2cell(enemyPath.points[currentTargetIndexEnemy]).x == num_cell_x - 1 && pix2cell(enemyPath.points[currentTargetIndexEnemy + 1]).x == 0) {
				currentEnemyTarget.x += CELL_SIZE;
				throughTunnelEnemy = true;
			}//si ha de sortir de l'esquerra
			else if (pix2cell(enemyPath.points[currentTargetIndexEnemy]).x == 0 && pix2cell(enemyPath.points[currentTargetIndexEnemy + 1]).x == num_cell_x - 1) {
				currentEnemyTarget.x -= CELL_SIZE;
				throughTunnelEnemy = true;
			}
			currentTargetIndexEnemy++;

		}

		if (!throughTunnelEnemy) {
			currentEnemyTarget = enemyPath.points[currentTargetIndexEnemy];
		}

		Vector2D steering_force = agents[1]->Behavior()->Seek(agents[1], currentEnemyTarget, dtime);
		agents[1]->update(steering_force, dtime, event);
	}
}
void SceneEnemy::drawGraphConexions(){
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			if(graph[i][j].previousNode!=nullptr)
				SDL_RenderDrawLine(TheApp::Instance()->getRenderer(),graph[i][j].position.x, graph[i][j].position.y, graph[i][j].previousNode->position.x, graph[i][j].previousNode->position.y);
		}

	}
}

void SceneEnemy::draw()
{
	drawMaze();
	drawCoin();


	if (draw_grid)
	{
		
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		
		//drawGraph();
		/*if(isBFS == true)
			BFS::draw();
		if (isDijkstra == true)
			dijkstra::draw();
		if (isGFS == true)
			GFS::draw();
		if (isAestrella == true)
			Aestrella::draw();*/
		drawGraphConexions();
		for (int i = 0; i < SRC_WIDTH; i+=CELL_SIZE)
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

	for (int i = 0; i < (int)enemyPath.points.size(); i++)
	{
		draw_circle(TheApp::Instance()->getRenderer(), (int)(enemyPath.points[i].x), (int)(enemyPath.points[i].y), 15, 0, 255, 255, 255);
		if (i > 0)
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(enemyPath.points[i - 1].x), (int)(enemyPath.points[i - 1].y), (int)(enemyPath.points[i].x), (int)(enemyPath.points[i].y));
	}

	draw_circle(TheApp::Instance()->getRenderer(), (int)currentTarget.x, (int)currentTarget.y, 15, 255, 0, 0, 255);
	agents[1]->draw();
	agents[0]->draw();
	
}

const char* SceneEnemy::getTitle()
{
	return "SDL Steering Behaviors ::  Enemy Avoidance";
}

void SceneEnemy::drawMaze()
{
	if (draw_grid)
	{

		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
		for (unsigned int i = 0; i < maze_rects.size(); i++)
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &maze_rects[i]);
	}
	else
	{
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
	}
}

void SceneEnemy::drawCoin()
{
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void SceneEnemy::initMaze()
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
	// (2nd) set to zero all cells that belong to a wall
	int offset = CELL_SIZE / 2;
	for (int i = 0; i < num_cell_x; i++)
	{
		for (int j = 0; j < num_cell_y; j++)
		{
			Vector2D cell_center ((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset));
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

bool SceneEnemy::loadTextures(char* filename_bg, char* filename_coin)
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

Vector2D SceneEnemy::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D SceneEnemy::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x/CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool SceneEnemy::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.x >= terrain.size()) || (cell.y >= terrain[0].size()) )
		return false;
	return !(terrain[(unsigned int)cell.x][(unsigned int)cell.y] == 0);
}