#include "PowerUp.h"

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