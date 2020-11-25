#include "EnemyGhost.h"
#include "pacman.h"

#include <iostream>
#include <list>

EnemyGhost::EnemyGhost()
{ 
	direction = 1;
	speed = 0.2f;
	texture = new Texture2D();
	texture->Load("Textures/Ghost_Pink_Sprite_Sheet.png", false);
	rectPosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	sourceRect = new Rect(0.0f, 0.0f, 32.0f, 32.0f);
	position = new Vector2(sourceRect->Center());
}

void EnemyGhost::GhostAnimation()
{
	sourceRect->X = sourceRect->Width * direction;
}

void EnemyGhost::GhostMovement(int elapsedTime)
{
	// Ghost movement based on direction
	if (direction == 0) // Right
		rectPosition->X += speed * elapsedTime;
	else if (direction == 1) // Up 
		rectPosition->Y -= speed * elapsedTime;
	else if (direction == 2) // Down
		rectPosition->Y += speed * elapsedTime;
	else if (direction == 3) // Left
		rectPosition->X -= speed * elapsedTime;

	// Change ghost direction if hit the edge
	if (rectPosition->X + sourceRect->Width >= Graphics::GetViewportWidth())
		direction = 3;
	else if (rectPosition->X <= 0)
		direction = 0;
	else if (rectPosition->Y + sourceRect->Height >= Graphics::GetViewportHeight())
		direction = 1;
	else if (rectPosition->Y <= 0)
		direction = 2;
}

void EnemyGhost::SetCurrentNode(Node nodes[])
{
	// Check collision against every node
	for (int x = 0; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
		{
			// Check X collision if no collision we can skip this tile
			if (rectPosition->X < nodes[y * TILECOUNTX + x].position->X + nodes[y * TILECOUNTX + x].sourceRect->Width
				&& rectPosition->X > nodes[y * TILECOUNTX + x].position->X);
			else
				break;
			
			// check Y collision
			if (rectPosition->Y < nodes[y * TILECOUNTX + x].position->Y + nodes[y * TILECOUNTX + x].sourceRect->Height
				&& rectPosition->Y > nodes[y * TILECOUNTX + x].position->Y)
				nodeStart = &nodes[y * TILECOUNTX + x];
		}
}

void EnemyGhost::SetGoalNode(PlayerClass* _pacman)
{
	nodeGoal = _pacman->currentNode;
}

bool EnemyGhost::solve_Astar(Node nodes[])
{
	// Reset the tile state
	for (int x = 0; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
		{
			nodes[y * TILECOUNTX + x].visited = false;
			nodes[y * TILECOUNTX + x].globalGoal = INFINITY;
			nodes[y * TILECOUNTX + x].localGoal = INFINITY;
			nodes[y * TILECOUNTX + x].parent = nullptr;
		}


	auto distance = [](Node* a, Node* b)
	{
		return sqrtf((a->position->X - b->position->X) * (a->position->X - b->position->X) + (a->position->Y - b->position->Y) * (a->position->Y - b->position->Y));
	};

	// Heuristic function to rank the nodes
	auto heuristic = [distance](Node* a, Node* b)
	{
		return distance(a, b);
	};

	// set current node as the starting node
	Node* nodeCurrent = nodeStart;
	nodeStart->localGoal = 0.0f;
	nodeStart->globalGoal = heuristic(nodeStart, nodeGoal);

	// Make a list to keep track of unchecked Nodes
	list<Node*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	// keep sorting unless the list is empty or current node is the goal
	while (!listNotTestedNodes.empty() && nodeCurrent != nodeGoal)
	{
		// Sort Untested nodes by global goal, so lowest is first
		listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->globalGoal < rhs->globalGoal; });
		
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
				nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, nodeGoal);
			}
		}
	}

	return true;
}

EnemyGhost::~EnemyGhost()
{
	delete texture;
	delete rectPosition;
	delete sourceRect;
}

