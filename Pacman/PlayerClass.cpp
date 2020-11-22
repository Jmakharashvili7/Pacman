#include "PlayerClass.h"
#include "iostream"

PlayerClass::PlayerClass()
{
	// General Variables
	currentLives = PACMANLIVES;
	direction = 0;
	timePassedHit = 0;
	speedModifier = 0.5f;
	dead = false;

	// Initialise Frame Variables
	currentFrameTime = 0;
	s_frameTime = 250;
	currentFrame = 0;

	// Invisibility Variables
	invisible = false;
	currentInvisCount = 0;
	invisCount = 6; // Time invincible (1 = 0.5 seconds)

	// Load Pacman texture and location
	texture = new Texture2D();
	texture->Load("Textures/Pacman.tga", false);
	position = new Vector2(350.0f, 350.0f);
	sourceRect = new Rect(0.0f, 0.0f, 32, 32);
}

void PlayerClass::PacmanHit(int elapsedTime)
{
	timePassedHit += elapsedTime;

	sourceRect->X = sourceRect->Width * 0; // Reset animation

	// Make pacman Invisible every 0.5 seconds to create flashing effect
	if (timePassedHit >= 500)
	{
		if (!invisible)
		{
			currentInvisCount++;
			invisible = !invisible;
			timePassedHit = 0;
		}
		else
		{
			currentInvisCount++;
			invisible = !invisible;
			timePassedHit = 0;
		}
	}
	if (currentInvisCount >= invisCount)
	{
		currentInvisCount = 0;
		timePassedHit = 0;
		invisible = false;
		dead = false;
	}
}

void PlayerClass::UpdatePacman(int elapsedTime)
{
	currentFrameTime += elapsedTime;

	sourceRect->Y = sourceRect->Height * direction; // Change pacmans direction
	sourceRect->X = sourceRect->Width * currentFrame; // Change Pacmans mouth

	// Pacman Animation Cycles
	if (currentFrameTime > s_frameTime)
	{
		currentFrame++;

		// If we're at the last frame cycle back to start
		if (currentFrame >= 2)
			currentFrame = 0;

		currentFrameTime = 0; // Reset Frame Time
	}
}

void PlayerClass::CheckInput(int elapsedTime, Input::KeyboardState* state)
{
	float pacmanSpeed = speedModifier * elapsedTime;

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