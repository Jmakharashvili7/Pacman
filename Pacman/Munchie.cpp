#include "Munchie.h"

Munchie::Munchie()
{
	currentFrameTime = 0;
	frameTime = rand() % 100 + 400;
	frameCount = rand() % 2;
	sourceRect = new Rect(0, 0, 12.0f, 12.0f);
}

Munchie::~Munchie()
{
	delete sourceRect;
	delete position;
	delete texture;
}
