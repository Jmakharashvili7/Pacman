#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#define MUNCHIECOUNT 50
#define CHERRYCOUNT 4
#define GHOSTCOUNT 4
#define TILECOUNTX 27
#define TILECOUNTY 20

// Include all the headers
#include <sstream>
#include <ctime>
#include <iostream>
#include <vector>

// Include all the classes
#include "S2D/S2D.h"
#include "EnemyGhost.h"
#include "PlayerClass.h"
#include "PowerUp.h"
#include "Menu.h"
#include "GameManager.h"
#include "TileManager.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct LifeUI
{
	int amount;
	Texture2D* texture;
	Vector2* positions[PACMANLIVES];
	Rect* sourceRect;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	// Check Methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();

	// class declarations
	PlayerClass* _pacman;
	EnemyGhost* _enemyGhost;
	PowerUp* _munchies[MUNCHIECOUNT];
	PowerUp* _cherries[CHERRYCOUNT];
	LifeUI* _lifeUI;
	Menu* _menu;
	GameManager* _gameManager;
	
	// Screen Parameters
	float screenHeight;
	float screenWidth;

	// Position for String
	Vector2* _stringPosition;
	Vector2* _scorePosition;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	// setup nodes
	Node* nodes = nullptr;
	Node* nodeStart = nullptr;
	Node* nodeEnd = nullptr;

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	/// <summary> Checks if there is collision between two objects. </summary>
	bool CheckCollision(PlayerClass* pacman, PowerUp* powerUp);
	bool CheckCollision(PlayerClass* pacman, EnemyGhost* ghost);
};