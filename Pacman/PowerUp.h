#pragma once

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

class PowerUp
{
public:
	PowerUp();
	~PowerUp();
	int currentFrameTime;
	int frameTime;
	int frameCount;
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
};

