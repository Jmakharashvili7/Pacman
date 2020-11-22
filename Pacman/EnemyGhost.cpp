#include "EnemyGhost.h"

#include <iostream>

EnemyGhost::EnemyGhost()
{ 
	direction = 1;
	speed = 0.2f;
	texture = new Texture2D();
	texture->Load("Textures/Ghost_Pink_Sprite_Sheet.png", false);
	position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	sourceRect = new Rect(0.0f, 0.0f, 32.0f, 32.0f);
}

void EnemyGhost::GhostAnimation()
{
	sourceRect->X = sourceRect->Width * direction;
}

void EnemyGhost::GhostMovement(int elapsedTime)
{
	// Ghost movement based on direction
	if (direction == 0) // Right
		position->X += speed * elapsedTime;
	else if (direction == 1) // Up 
		position->Y -= speed * elapsedTime;
	else if (direction == 2) // Down
		position->Y += speed * elapsedTime;
	else if (direction == 3) // Left
		position->X -= speed * elapsedTime;

	// Change ghost direction if hit the edge
	if (position->X + sourceRect->Width >= Graphics::GetViewportWidth())
		direction = 3;
	else if (position->X <= 0)
		direction = 0;
	else if (position->Y + sourceRect->Height >= Graphics::GetViewportHeight())
		direction = 1;
	else if (position->Y <= 0)
		direction = 2;
}

EnemyGhost::~EnemyGhost()
{
	delete texture;
	delete position;
	delete sourceRect;
}