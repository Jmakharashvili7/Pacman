#include "EnemyGhost.h"
#include "pacman.h"

EnemyGhost::EnemyGhost(GhostColour ghostColour, Tile* spawn)
{
	switch (ghostColour)
	{
	case Red: 
		type = ghostColour;
		direction = 0;
		speed = 0.1f;
		texture = new Texture2D();
		texture->Load("Textures/Ghost_Pink_Sprite_Sheet.png", false);
		sourceRect = new Rect(spawn->sourceRect->X, spawn->sourceRect->Y, 27.0f, 27.0f);
		textureRect = new Rect(0.0f, 0.0f, 32.0f, 32.0f);
		position = new Vector2(sourceRect->Center());
		break;

	case Pink: 
		type = ghostColour;
		direction = 0;
		speed = 0.2f;
		texture = new Texture2D();
		texture->Load("Textures/Ghost_Pink_Sprite_Sheet.png", false);
		sourceRect = new Rect(spawn->position->X, spawn->position->Y, 27.0f, 27.0f);
		textureRect = new Rect(0.0f, 0.0f, 32.0f, 32.0f);
		position = new Vector2(sourceRect->Center());
		break;

	case Blue: 
		type = ghostColour;
		direction = 0;
		speed = 0.2f;
		texture = new Texture2D();
		texture->Load("Textures/Ghost_Pink_Sprite_Sheet.png", false);
		sourceRect = new Rect(spawn->position->X, spawn->position->Y, 27.0f, 27.0f);
		textureRect = new Rect(0.0f, 0.0f, 32.0f, 32.0f);
		position = new Vector2(sourceRect->Center());
		break;

	case Orange: 
		type = ghostColour;
		direction = 0;
		speed = 0.2f;
		texture = new Texture2D();
		texture->Load("Textures/Ghost_Pink_Sprite_Sheet.png", false);
		sourceRect = new Rect(spawn->position->X, spawn->position->Y, 27.0f, 27.0f);
		textureRect = new Rect(0.0f, 0.0f, 32.0f, 32.0f);
		position = new Vector2(sourceRect->Center());
		break;
	}
}

void EnemyGhost::GhostAnimation()
{
	textureRect->X = textureRect->Width * direction;
}

void EnemyGhost::SetCurrentNode(Tile tiles[])
{
	// Check collision against every node
	for (int x = 0; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
		{
			// Check X collision if no collision we can skip this tile
			if (position->X < tiles[y * TILECOUNTX + x].sourceRect->X + tiles[y * TILECOUNTX + x].sourceRect->Width
				&& position->X > tiles[y * TILECOUNTX + x].sourceRect->X);
			else
				break;
			
			// check Y collision
			if (position->Y < tiles[y * TILECOUNTX + x].sourceRect->Y + tiles[y * TILECOUNTX + x].sourceRect->Height
				&& position->Y > tiles[y * TILECOUNTX + x].sourceRect->Y)
				currentTile = &tiles[y * TILECOUNTX + x];
		}
}

bool EnemyGhost::solve_Astar(Tile tiles[])
{
	// Reset the tile state
	for (int x = 0; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
		{
			tiles[y * TILECOUNTX + x].visited = false;
			tiles[y * TILECOUNTX + x].globalGoal = INFINITY;
			tiles[y * TILECOUNTX + x].localGoal = INFINITY;
			tiles[y * TILECOUNTX + x].parent = nullptr;
		}

	auto distance = [](Tile* a, Tile* b)
	{
		return sqrtf((a->sourceRect->X - b->sourceRect->X) * (a->sourceRect->X - b->sourceRect->X) + 
			(a->sourceRect->Y - b->sourceRect->Y) * (a->sourceRect->Y - b->sourceRect->Y));
	};

	// Heuristic function to rank the nodes
	auto heuristic = [distance](Tile* a, Tile* b)
	{
		return distance(a, b);
	};

	// set current node as the starting node
	Tile* nodeCurrent = currentTile;
	currentTile->localGoal = 0.0f;
	currentTile->globalGoal = heuristic(currentTile, tileGoal);

	// Make a list to keep track of unchecked Nodes
	list<Tile*> listNotTestedNodes;
	listNotTestedNodes.push_back(currentTile);

	// keep sorting unless the list is empty or current node is the goal
	while (!listNotTestedNodes.empty() && nodeCurrent != tileGoal)
	{
		// Sort Untested nodes by global goal, so lowest is first
		listNotTestedNodes.sort([](const Tile* lhs, const Tile* rhs) { return lhs->globalGoal < rhs->globalGoal; });
		
		// If the front of the list has alredy been visited then remove it from the list
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
			listNotTestedNodes.pop_front();

		// Check if there are any nodes left if not then quit
		if (listNotTestedNodes.empty())
			break;

		// Move it to the front of the list and mark the node as visited
		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->visited = true;	

		// Check each of this node's neighbours...
		for (auto nodeNeighbour : nodeCurrent->nodeNeighbours)
		{
			// Check if the neighbor nodes have been visited of they are a wall
			if (!nodeNeighbour->visited && nodeNeighbour->wall == 0)
			{
				listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbours potential lowest parent distance
				float possiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

				if (possiblyLowerGoal < nodeNeighbour->localGoal)
				{
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->localGoal = possiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, tileGoal);
				}
			}
		}
	}

	return true;
}

void EnemyGhost::GhostAI(int elapsedTime, Tile tiles[], PlayerClass* _pacman)
{
	// Ghosts speed
	float moveSpeed = speed * elapsedTime;
	// Pacman tile position in array
	int pacmanTile = _pacman->currentTile->posInArray;

	switch (type)
	{
	case Red: // Red ghost AI
		tileGoal = _pacman->currentTile;
		break;
	case Pink:
		if (_pacman->direction == Right)
			tileGoal = &tiles[pacmanTile + 2];
		else if (_pacman->direction == Left)
			tileGoal = &tiles[pacmanTile - 2];
		else if (_pacman->direction == Down)
			tileGoal = &tiles[pacmanTile + 2 *TILECOUNTX];
		else if (_pacman->direction == Up)
			tileGoal = &tiles[pacmanTile - 2 *TILECOUNTX];
		break;
	case Blue:
		// pick a random tile within a certain redius of pacman and chase it until pacman leaves the radius
		break;
	case Orange:
		// hover around the nearest powerup of pacman
		break;
	}

	solve_Astar(tiles);

	if (tileGoal != nullptr)
	{
		if (tileGoal->parent != nullptr)
		{
			// If the tile is on the right of the ghost move right
			if (position->X < tileGoal->position->X)
				direction = 0;

			// If the tile is above the ghost move up
			else if (position->Y > tileGoal->position->Y)
				direction = 1;

			// If the tile is under the ghost move down
			else if (position->Y < tileGoal->position->Y)
				direction = 2;

			// If the tile is on the left of the ghost move left
			else if (position->X > tileGoal->position->X)
				direction = 3;

			// If the tiles are equal move to next Tile
			else
				tileGoal = tileGoal->parent;
		}
	}

	// Ghost movement based on direction
	if (direction == 0) // Right
	{
		sourceRect->X += moveSpeed;
		position->X += moveSpeed;
	}
	else if (direction == 1) // Up 
	{
		sourceRect->Y -= moveSpeed;
		position->Y -= moveSpeed;
	}
	else if (direction == 2) // Down
	{
		sourceRect->Y += moveSpeed;
		position->Y += moveSpeed;
	}
	else if (direction == 3) // Left
	{
		sourceRect->X -= moveSpeed;
		position->X -= moveSpeed;
	}
}

EnemyGhost::~EnemyGhost()
{
	// delete position and texture variables
	delete texture;
	delete sourceRect;
	delete textureRect;
	delete position;
	// delete node variables
	delete currentTile;
	delete tileGoal;
}

