#include "Fruit.h"

Fruit::Fruit()
{
	currentFrameTime = 0;
	frameTime = rand() % 100 + 400;
	frameCount = rand() % 2;
	sourceRect = new Rect(0, 0, 27.0f, 27.0f);
}

Fruit::~Fruit()
{
	delete sourceRect;
	delete position;
	delete texture;
}