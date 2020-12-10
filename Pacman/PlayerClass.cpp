#include "PlayerClass.h"

#include "iostream"
#include "Pacman.h"

PlayerClass::PlayerClass()
{
	// General Variables
	currentLives = PACMANLIVES;
	direction = Right;
	timePassedHit = 0;
	speedModifier = 0.1f;
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

void PlayerClass::SetCurrentTile(Tile tiles[])
{
	// Check collision against every node
	for (int x = 0; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
		{
			// Check X collision if no collision we can skip this tile
			if (position->X < tiles[y * TILECOUNTX + x].rectPosition->X + tiles[y * TILECOUNTX + x].sourceRect->Width
				&& position->X > tiles[y * TILECOUNTX + x].rectPosition->X);
			else
				break;

			// check Y collision
			if (position->Y < tiles[y * TILECOUNTX + x].rectPosition->Y + tiles[y * TILECOUNTX + x].sourceRect->Height
				&& position->Y > tiles[y * TILECOUNTX + x].rectPosition->Y)
			{
				currentTile = &tiles[y * TILECOUNTX + x];
			}
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

void PlayerClass::CheckInput(Input::KeyboardState* state)
{
	// Pacman Movement 
	if (state->IsKeyDown(Input::Keys::D))
		direction = Right; // Face Right

	else if (state->IsKeyDown(Input::Keys::S))
		direction = Down; // Face Down

	else if (state->IsKeyDown(Input::Keys::A))
		direction = Left; // Face Left

	else if (state->IsKeyDown(Input::Keys::W))
		direction = Up; // Face Up
}

void PlayerClass::Movement(int elapsedTime, Tile tiles[])
{
	float pacmanSpeed = speedModifier * elapsedTime;

	switch (direction)
	{
	case Right:
		if (// Center of right wall is colliding with the left wall of the tile on the right stop movement)
		rectPosition->X += pacmanSpeed; // Move Right
		position->X += pacmanSpeed;
		break;
	case Down:
		rectPosition->Y += pacmanSpeed; // Move Down
		position->Y += pacmanSpeed;
		break;
	case Left:
		rectPosition->X -= pacmanSpeed; // Move Left
		position->X -= pacmanSpeed;
		break;
	case Up:
		rectPosition->Y -= pacmanSpeed; // Move Up
		position->Y -= pacmanSpeed;
		break;
	default:
		break;
	}
}

PlayerClass::~PlayerClass()
{
	// Clean up Pacman
	delete texture;
	delete sourceRect;
	delete rectPosition;
}