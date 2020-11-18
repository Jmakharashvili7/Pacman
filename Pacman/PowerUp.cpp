#include "PowerUp.h"

PowerUp::PowerUp()
{
	currentFrameTime = 0;
	frameTime = rand() % 100 + 400;
	frameCount = rand() % 2;
	position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
}

PowerUp::~PowerUp()
{
	delete sourceRect;
	delete position;
	delete texture;
}