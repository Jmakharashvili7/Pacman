#pragma once
#define PACMANLIVES 3

// Just need to include main header file
#include "S2D/S2D.h"
#include "TileManager.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

enum PacmanDirection { Right, Down, Left, Up };	

class PlayerClass
{
public:
	PlayerClass();
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
	Vector2* rectPosition;
	Vector2* position;
	Rect* sourceRect;
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
	void SetCurrentTile(Tile nodes[]);
	void Movement(int elapsedTime, Tile nodes[]);
	void CheckInput(Input::KeyboardState* state);
};




