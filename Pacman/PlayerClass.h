#pragma once

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

class PlayerClass
{
public:
	PlayerClass();
	~PlayerClass();
	int direction;
	int frameCount;
	float s_pacmanSpeed;
	int s_frameTime;
	int currentFrameTime;
	Texture2D* texture;
	Vector2* position;
	Rect* sourceRect;
public:
	// Pacman Methods
	void PacmanHit(int elapsedTime, PlayerClass* _pacman);
	void UpdatePacman(int elapsedTime);
	void CheckInput(int elapsedTime, Input::KeyboardState* state);
};



