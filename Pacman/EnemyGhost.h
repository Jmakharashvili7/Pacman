#pragma once

// Just need to include main header file
#include "S2D/S2D.h"
#include "Tile.h"
#include "PlayerClass.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

enum GhostColour { Red, Pink, Blue, Orange };

class EnemyGhost
{
public:
	EnemyGhost(GhostColour ghostColour, Tile* spawn);
	~EnemyGhost();
	int direction;
	float speed;
	Tile* currentTile;
	Tile* tileGoal;
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	Rect* textureRect;
	GhostColour type;
public:
	void GhostAI(int elapsedTime, Tile nodes[], PlayerClass* _pacman);
	void GhostAnimation();
	void SetCurrentNode(Tile nodes[]);
	bool solve_Astar(Tile nodes[]);
};

