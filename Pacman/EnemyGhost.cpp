#include "EnemyGhost.h"

#include <iostream>

EnemyGhost::EnemyGhost()
{ 
	direction = 0;
	texture = new Texture2D();
	texture->Load("Textures/Ghost_Sprite_Sheet.png", false);
	position = new Vector2(350.0f, 350.0f);
	sourceRect = new Rect(0.0f, 0.0f, 20.0f, 20.0f);
}

EnemyGhost::~EnemyGhost()
{
	delete texture;
	delete position;
	delete sourceRect;
}