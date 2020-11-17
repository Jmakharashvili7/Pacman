#pragma once
#include "pacman.h"

class Player
{
public:
	int direction;
	int currentFrameTime;
	int frame;
	LifeUI* lifeUI;
	Texture2D* texture;
	Vector2* position;
	Rect* sourceRect;
};
