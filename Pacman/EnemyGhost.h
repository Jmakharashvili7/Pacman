#pragma once

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

class EnemyGhost
{
public:
	EnemyGhost();
	~EnemyGhost();
	int direction;
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
};

