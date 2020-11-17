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
#define PACMANLIVES 3
#define ENEMYCOUNT 4
#define BUTTONCOUNT 3

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct LifeUI
{
	int amount;
	Texture2D* texture;
	Vector2* positions[PACMANLIVES];
	Rect* sourceRect;
};

struct PowerUp
{
	int frameCount;
	int frameTime;
	int currentFrameTime;
	Texture2D* texture;
	Vector2* position;
	Rect* sourceRect;
};

struct Button
{
	Texture2D* background;
	Rect* rectangle;
	Vector2* position;
};

struct Menu
{
	Texture2D* background;
	Rect* Rectangle;	
	Vector2* titlePosition;
	Button* startButton;
	Button* optionsButton;
	Button* quitButton;
};

struct GameManager
{
	int score;
	bool started;
	bool paused;
	bool pauseMenu;
	bool pKeyDown;
};

struct Enemy 
{
	int frameCount;
	int frameTime;
	int currentFrameTime;
	Texture2D* texture;
	Vector2* position;
	Rect* sourceRect;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	// Check Methods
	void CheckInput(int elapsedtime, Input::KeyboardState* state, Input::MouseState* mouseState);
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	bool CheckMenuButtonPress(Input::MouseState* mouseState, Rect* button);
	void CheckViewportCollision();

	// Update Methods
	void UpdatePacman(int elapsedTime);
	void UpdatePowerUp(PowerUp* powerUp, int elapsedTime);

	// Pacman Methods
	void PacmanHit(int elapsedTime, Player* pacman);

	// Struct declarations
	Player* _pacman;
	PowerUp* _munchies[MUNCHIECOUNT];
	PowerUp* _cherries[CHERRYCOUNT];
	Menu* _menu;
	GameManager* _game;

	// Data for Constant Game Variables
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	const int _cMunchieFrameTime;

	// Screen Parameters
	float screenHeight;
	float screenWidth;

	// Position for String
	Vector2* _stringPosition;
	Vector2* _scorePosition;

public:
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

	/// <summary> Checks if there is collision between two objects. </summary>
	bool CheckCollision(Player* pacman, PowerUp* powerUp);
};