#include "PlayerClass.h"

#include "iostream"
#include "Pacman.h"

PlayerClass::PlayerClass(Tile* spawn)
{
	// General Variables
	currentLives = PACMANLIVES;
	currentTile = nullptr;
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
	textRect = new Rect(0.0f, 0.0f, 32.0f, 32.0f);
	sourceRect = new Rect(spawn->sourceRect->X, spawn->sourceRect->Y, 27.0f, 27.0f);
	position = new Vector2(sourceRect->Center());
}

void PlayerClass::PacmanHit(int elapsedTime)
{
	timePassedHit += elapsedTime;

	textRect->X = textRect->Width * 0; // Reset animation

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
			if (position->X < tiles[y * TILECOUNTX + x].sourceRect->X + tiles[y * TILECOUNTX + x].sourceRect->Width
				&& position->X > tiles[y * TILECOUNTX + x].sourceRect->X);
			else
				break;

			// check Y collision
			if (position->Y < tiles[y * TILECOUNTX + x].sourceRect->Y + tiles[y * TILECOUNTX + x].sourceRect->Height
				&& position->Y > tiles[y * TILECOUNTX + x].sourceRect->Y)
			{
				currentTile = &tiles[y * TILECOUNTX + x];
			}
		}
}

void PlayerClass::UpdatePacman(int elapsedTime)
{
	currentFrameTime += elapsedTime;

	textRect->Y = textRect->Height * direction; // Change pacmans direction
	textRect->X = textRect->Width * currentFrame; // Change Pacmans mouth

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
	{
		direction = Right; // Face Right
	}
	else if (state->IsKeyDown(Input::Keys::S))
		direction = Down; // Face Down

	else if (state->IsKeyDown(Input::Keys::A))
		direction = Left; // Face Left

	else if (state->IsKeyDown(Input::Keys::W))
		direction = Up; // Face Up
}

void PlayerClass::Movement(int elapsedTime, Tile tiles[])
{
	float pacmanSpeed = 1.5f;

	switch (direction)
	{
	case Right:
		if (tiles[currentTile->posInArray + 1].wall)
		{
			if (sourceRect->X + sourceRect->Width < tiles[currentTile->posInArray + 1].sourceRect->X)
			{
			sourceRect->X += pacmanSpeed;
			position->X += pacmanSpeed;
			}
		}
		else
		{
			sourceRect->X += pacmanSpeed;
			position->X += pacmanSpeed;
		}
		break;
	case Down:
		if (tiles[currentTile->posInArray + TILECOUNTX].wall)
		{
			if (sourceRect->Y + sourceRect->Height < tiles[currentTile->posInArray + TILECOUNTX].sourceRect->Y)
			{
				sourceRect->Y += pacmanSpeed; // Move Down
				position->Y += pacmanSpeed;
			}
		}
		else 
		{
			sourceRect->Y += pacmanSpeed; // Move Down
			position->Y += pacmanSpeed;
		}
		break;
	case Left:
		// Check if the tile on the left is a wall
		if (tiles[currentTile->posInArray - 1].wall)
		{
			//  Stop moving if we collide with the wall
			if (sourceRect->X > tiles[currentTile->posInArray - 1].sourceRect->X + tiles[currentTile->posInArray - 1].sourceRect->Width)
			{
				sourceRect->X -= pacmanSpeed;
				position->X -= pacmanSpeed;
			}
		}
		// Keep moving if there is no wall
		else
		{
			sourceRect->X -= pacmanSpeed;
			position->X -= pacmanSpeed;
		}
		break;
	case Up:
		if (tiles[currentTile->posInArray - TILECOUNTX].wall)
		{
			if (sourceRect->Y > tiles[currentTile->posInArray - TILECOUNTX].sourceRect->Y +
				tiles[currentTile->posInArray - TILECOUNTX].sourceRect->Height)
			{
				sourceRect->Y -= pacmanSpeed; // Move Up
				position->Y -= pacmanSpeed;
			}
		}
		else
		{
			sourceRect->Y -= pacmanSpeed; // Move Up
			position->Y -= pacmanSpeed;
		}
		break;
	}
}

PlayerClass::~PlayerClass()
{
	// Clean up Pacman
	delete texture;
	delete sourceRect;
	delete sourceRect;
}