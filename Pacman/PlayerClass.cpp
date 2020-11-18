#include "PlayerClass.h"
#include "iostream"

PlayerClass::PlayerClass()
{
	s_frameTime = 250;
	s_pacmanSpeed = 0.1f;
	currentLives = PACMANLIVES;
	direction = 0;
	currentFrameTime = 0;
	frameCount = 0;

	// Load Pacman texture
	texture = new Texture2D();
	texture->Load("Textures/Pacman.tga", false);
	position = new Vector2(350.0f, 350.0f);
	sourceRect = new Rect(0.0f, 0.0f, 32, 32);
}

void PlayerClass::UpdatePacman(int elapsedTime)
{
	currentFrameTime += elapsedTime;

	sourceRect->Y = sourceRect->Height * direction; // Change pacmans direction
	sourceRect->X = sourceRect->Width * frameCount; // Change Pacmans mouth

	// Pacman Animation Cycles
	if (currentFrameTime > s_frameTime)
	{
		frameCount++;

		// If we're at the last frame cycle back to start
		if (frameCount >= 2)
			frameCount = 0;

		currentFrameTime = 0; // Reset Frame Time
	}
}

void PlayerClass::CheckInput(int elapsedTime, Input::KeyboardState* state)
{
	float pacmanSpeed = s_pacmanSpeed * elapsedTime;

	// Pacman Movement 
	if (state->IsKeyDown(Input::Keys::D))
	{
		position->X += pacmanSpeed; // Move Right
		direction = 0; // Face Right
	}
	else if (state->IsKeyDown(Input::Keys::S))
	{
		position->Y += pacmanSpeed; // Move Down
		direction = 1; // Face Down
	}
	else if (state->IsKeyDown(Input::Keys::A))
	{
		position->X -= pacmanSpeed; // Move Left
		direction = 2; // Face Left

	}
	else if (state->IsKeyDown(Input::Keys::W))
	{
		position->Y -= pacmanSpeed; // Move Up
		direction = 3; // Face Up
	}
}

PlayerClass::~PlayerClass()
{
	// Clean up Pacman
	delete texture;
	delete sourceRect;
	delete position;
}