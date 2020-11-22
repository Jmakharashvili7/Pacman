#include "PowerUp.h"

PowerUp::PowerUp()
{
	currentFrameTime = 0;
	frameTime = rand() % 100 + 400;
	frameCount = rand() % 2;
	position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
}

void PowerUp::UpdatePowerUp(int elapsedTime)
{
	currentFrameTime += elapsedTime;

	sourceRect->X = sourceRect->Width * frameCount; // Change Munchie Frame

	// Munchie Animation Cycle
	if (currentFrameTime > frameTime)
	{
		frameCount++;

		// If we're at the last frame cycle back to start
		if (frameCount >= 2)
			frameCount = 0;

		currentFrameTime = 0; // Reset Frame Time
	}
}

PowerUp::~PowerUp()
{
	delete sourceRect;
	delete position;
	delete texture;
}