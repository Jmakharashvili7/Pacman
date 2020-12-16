#pragma once
#define PACMANLIVES 3

// Just need to include main header file
#include "S2D/S2D.h"
#include "Tile.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

enum PacmanDirection { Right, Down, Left, Up };	

class PlayerClass
{
public:
	PlayerClass(Tile* tile);
	~PlayerClass();

	// General pacman Variables
	PacmanDirection direction;
	int currentLives;
	int timePassedHit;
	float speedModifier;
	bool dead;
	bool invisible;
	Tile* currentTile;

	// Variables for drawing pacman
	Texture2D* texture;
	Vector2* position;
	Rect* sourceRect;
	Rect* textRect;
private:
	// Frame Variables
	int s_frameTime;
	int currentFrame;
	int currentFrameTime;

	// Invis Variables
	int currentInvisCount;
	int invisCount;
public:
	// Pacman Methods
	void PacmanHit(int elapsedTime);
	void UpdatePacman(int elapsedTime);
	void SetCurrentTile(Tile tiles[]);
	void Movement(int elapsedTime, Tile tiles[]);
	void CheckInput(Input::KeyboardState* state);
};




