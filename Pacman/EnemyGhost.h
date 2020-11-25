#pragma once

// Just need to include main header file
#include "S2D/S2D.h"
#include "TileManager.h"
#include "PlayerClass.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

class EnemyGhost
{
public:
	EnemyGhost();
	~EnemyGhost();
	int direction;
	float speed;
	Node* nodeStart;
	Node* nodeGoal;
	Vector2* rectPosition;
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
public:
	void GhostMovement(int elapsedTime);
	void GhostAnimation();
	void SetGoalNode(PlayerClass* _pacman);
	void SetCurrentNode(Node nodes[]);
	bool solve_Astar(Node nodes[]);
};

