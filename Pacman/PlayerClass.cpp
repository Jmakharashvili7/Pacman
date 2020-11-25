#include "PlayerClass.h"

#include "iostream"
#include "Pacman.h"

PlayerClass::PlayerClass()
{
	// General Variables
	currentLives = PACMANLIVES;
	direction = 0;
	timePassedHit = 0;
	speedModifier = 0.3f;
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
	rectPosition = new Vector2(350.0f, 350.0f);
	sourceRect = new Rect(rectPosition->X, rectPosition->Y, 32.0f, 32.0f);
	position = new Vector2(sourceRect->Center());
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

void PlayerClass::SetCurrentNode(Node nodes[])
{
	// Check collision against every node
	for (int x = 0; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
		{
			// Check X collision if no collision we can skip this tile
			if (position->X < nodes[y * TILECOUNTX + x].position->X + nodes[y * TILECOUNTX + x].sourceRect->Width
				&& position->X > nodes[y * TILECOUNTX + x].position->X);
			else
				break;

			// check Y collision
			if (position->Y < nodes[y * TILECOUNTX + x].position->Y + nodes[y * TILECOUNTX + x].sourceRect->Height
				&& position->Y > nodes[y * TILECOUNTX + x].position->Y)
				currentNode = &nodes[y * TILECOUNTX + x];
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
		rectPosition->X += pacmanSpeed; // Move Right
		position->X += pacmanSpeed;
		direction = 0; // Face Right
	}
	else if (state->IsKeyDown(Input::Keys::S))
	{
		rectPosition->Y += pacmanSpeed; // Move Down
		position->Y += pacmanSpeed;
		direction = 1; // Face Down
	}
	else if (state->IsKeyDown(Input::Keys::A))
	{
		rectPosition->X -= pacmanSpeed; // Move Left
		position->X -= pacmanSpeed;
		direction = 2; // Face Left

	}
	else if (state->IsKeyDown(Input::Keys::W))
	{
		rectPosition->Y -= pacmanSpeed; // Move Up
		position->Y -= pacmanSpeed;
		direction = 3; // Face Up
	}
}

PlayerClass::~PlayerClass()
{
	// Clean up Pacman
	delete texture;
	delete sourceRect;
	delete rectPosition;
}