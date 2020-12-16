#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#define GHOSTCOUNT 4
#define TILECOUNTX 28
#define TILECOUNTY 31
#define TILESIZE 27

// Include all the headers
#include <sstream>
#include <ctime>
#include <iostream>
#include <vector>
#include <list>

// Include all the classes
#include "S2D/S2D.h"
#include "EnemyGhost.h"
#include "PlayerClass.h"
#include "Menu.h"
#include "GameManager.h"
#include "Tile.h"
#include "PowerUp.h"
#include "Munchie.h"
#include "Fruit.h"

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

	// Map
	wstring _map;

	// class declarations
	PlayerClass* _pacman;
	EnemyGhost* _redGhost;
	EnemyGhost* _pinkGhost;
	vector<PowerUp*> _munchies;
	vector<Fruit*> _cherries;
	LifeUI* _lifeUI;
	Menu* _menu;
	GameManager* _gameManager;
	SoundEffect* _pop;

	// Screen Parameters
	float screenHeight;
	float screenWidth;

	// Position for String
	Vector2* _scorePosition;
public:
	// Declare and initialize nodes
	Tile* _tiles = nullptr;

	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

private:
	/// <summary> Checks if there is collision between two objects. </summary>
	bool CheckCollision(PlayerClass* pacman, PowerUp* powerUp);
	bool CheckCollision(PlayerClass* pacman, EnemyGhost* ghost);
};