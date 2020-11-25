#pragma once
#define PACMANLIVES 3

// Just need to include main header file
#include "S2D/S2D.h"
#include "TileManager.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

class PlayerClass
{
public:
	PlayerClass();
	~PlayerClass();

	// General pacman Variables
	int direction;
	int currentLives;
	int timePassedHit;
	float speedModifier;
	bool dead;
	bool invisible;
	Node* currentNode;

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
	void SetCurrentNode(Node nodes[]);
	void CheckInput(int elapsedTime, Input::KeyboardState* state);
};




