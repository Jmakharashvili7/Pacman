#include "Pacman.h"

#include <sstream>
#include <ctime>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	int i; //Local variable

	// Initialise Pacman
	_pacman = new Player();
	_pacman->lifeUI = new LifeUI();
	_pacman->lifeUI->amount = PACMANLIVES;
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;

	// Initialise Munchies
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new PowerUp();
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameTime = rand() % 100 + 400;
		_munchies[i]->frameCount = rand() % 2;
	}

	// Initialise Cherries
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i] = new PowerUp();
		_cherries[i]->currentFrameTime = 0;
		_cherries[i]->frameTime = rand() % 100 + 400;
		_cherries[i]->frameCount = rand() % 2;
	}

	// Initialise Menu
	_menu = new Menu();
	_menu->startButton = new Button();
	_menu->optionsButton = new Button();
	_menu->quitButton = new Button();

	// Initialise GameManager
	_game = new GameManager();
	_game->score = 0;
	_game->paused = false;
	_game->started = false;
	_game->pKeyDown = false;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1080, 720, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	int i; // Local Variable

#pragma region Menu

	// Clean up String positions
	delete _stringPosition;
	delete _scorePosition;
	delete _menu->titlePosition;
	delete _menu->optionsButton->position;
	delete _menu->quitButton->position;

	//Clean up Menu and Buttons
	delete _menu->background;
	delete _menu->Rectangle;

	delete _menu->startButton->background;
	delete _menu->startButton->position;
	delete _menu->startButton->rectangle;
	delete _menu->startButton;

	delete _menu->optionsButton->background;
	delete _menu->optionsButton->position;
	delete _menu->optionsButton->rectangle;
	delete _menu->optionsButton;

	delete _menu->quitButton->background;
	delete _menu->quitButton->position;
	delete _menu->quitButton->rectangle;
	delete _menu->quitButton;

#pragma endregion Clear up menu and string positions

#pragma region Pacman
	// Clean up Pacman
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;

	// Clean up Pacman Lives UI
	delete _pacman->lifeUI->texture;
	delete _pacman->lifeUI->sourceRect;
	delete _pacman->lifeUI;
	for(i = 0; i < PACMANLIVES; i++)
		delete _pacman->lifeUI->positions[i];
#pragma endregion Pacman cleanup

#pragma region powerUps
	// Clean up Munchie
	delete _munchies[0]->texture;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i]->position;
		delete _munchies[i]->sourceRect;
		delete _munchies[i];
	}

	delete[] _munchies;

	//Clean up cherries
	delete _cherries[0]->texture;

	for (i = 0; i < CHERRYCOUNT; i++)
	{
		delete _cherries[i]->position;
		delete _cherries[i]->sourceRect;
		delete _cherries[i];
	}
	
	delete[] _cherries;
#pragma endregion Powerups Cleanup
}

void Pacman::LoadContent()
{
	srand((unsigned) time(0)); // Seed random nunmber generator with time

	int i; // local variable

#pragma region LoadPacman

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Pacman Lives
	_pacman->lifeUI->texture = new Texture2D();
	_pacman->lifeUI->texture->Load("Textures/PacmanLives.tga", false);
	_pacman->lifeUI->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	for (i = 0; i < PACMANLIVES; i++)
		_pacman->lifeUI->positions[i] = new Vector2(5.0f + (32.0f * i), 60.0f);

#pragma endregion Loading pacman and Life UI

#pragma region LoadMunchies
	// Load pointer for munchie texture
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", false);

	// Load Munchies
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_munchies[i]->sourceRect = new Rect(0.0f, 0.0f, 12, 12);
	}
#pragma endregion Loads the munchies

#pragma region LoadCherry
	//Load pointer for cherry texture
	Texture2D* cherryTex = new Texture2D();
	cherryTex->Load("Textures/Cherry.png", false);

	// Load Cherries
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i]->texture = cherryTex;
		_cherries[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_cherries[i]->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}
#pragma endregion Load the cherries

#pragma region MenuLoad

	float screenMidWidth = Graphics::GetViewportWidth() / 2.0f;
	float screenMidHeight = Graphics::GetViewportHeight() / 2.0f;

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	_scorePosition = new Vector2(10.0f, 50.0f);

	// Set Menu Parameters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->titlePosition = new Vector2(screenMidWidth - 20.0f, screenMidHeight);
	_menu->Rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	// Setup start button
	_menu->startButton->background = new Texture2D();
	_menu->startButton->background->Load("Textures/Transparency.png", false);
	_menu->startButton->position = new Vector2(screenMidWidth - 20.0f, screenMidHeight + 25.0f);
	_menu->startButton->rectangle = new Rect(_menu->startButton->position->X, _menu->startButton->position->Y, 50.0f, -15.0f);

	// Setup options button
	_menu->optionsButton->background = new Texture2D();
	_menu->optionsButton->background->Load("Textures/Transparency.png", false);
	_menu->optionsButton->position = new Vector2(screenMidWidth - 20.0f, screenMidHeight + 50.0f);
	_menu->optionsButton->rectangle = new Rect(_menu->optionsButton->position->X, _menu->optionsButton->position->Y, 65.0, -15.0f);

	// Setup quit button
	_menu->quitButton->background = new Texture2D();
	_menu->quitButton->background->Load("Textures/Transparency.png", false);
	_menu->quitButton->position = new Vector2(screenMidWidth - 20.0f, screenMidHeight + 75.0f);
	_menu->quitButton->rectangle = new Rect(_menu->quitButton->position->X, _menu->quitButton->position->Y, 50.0, -15.0f);

#pragma endregion Load the menu
}

void Pacman::Draw(int elapsedTime)
{
#pragma region strings
	std::stringstream gameNameHeader; // Main Menu
	gameNameHeader << "Pacman";
	std::stringstream startButton; // Start button
	startButton << "Start";
	std::stringstream optionsButton; // Options Button
	optionsButton << "Options";
	std::stringstream quitButton; // Quit Button
	quitButton << "Quit";
	std::stringstream positionStream; // Display Position
	positionStream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;
	std::stringstream scoreStream; // Display Score
	scoreStream << "Score : " << _game->score;
#pragma endregion Set up the strings

	SpriteBatch::BeginDraw(); // Starts Drawing

	// If the game hasn't started draw the main menu
	if (!_game->started)
	{
		SpriteBatch::DrawString(gameNameHeader.str().c_str(), _menu->titlePosition, Color::Yellow); // Draws Title
		SpriteBatch::DrawString(startButton.str().c_str(), _menu->startButton->position, Color::Red); // Draws start Button
		SpriteBatch::DrawString(optionsButton.str().c_str(), _menu->optionsButton->position, Color::Red); // Draws options button
		SpriteBatch::DrawString(quitButton.str().c_str(), _menu->quitButton->position, Color::Red); // Draws quit Button
	}

	//if the game has started draw everything else
	else
	{
		int i; // Local Variable

		for (i = 0; i < MUNCHIECOUNT; i++)
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect); // Draws Munchies

		for (i = 0; i < CHERRYCOUNT; i++)
			SpriteBatch::Draw(_cherries[i]->texture, _cherries[i]->position, _cherries[i]->sourceRect); // Draws Cherries

		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
		// Draw pacmans health as many as pacman health in for loop
		// Draw Pacman Health UI
		for (i = 0; i < _pacman->lifeUI->amount; i++)
			SpriteBatch::Draw(_pacman->lifeUI->texture, _pacman->lifeUI->positions[i], _pacman->lifeUI->sourceRect);
		SpriteBatch::DrawString(positionStream.str().c_str(), _stringPosition, Color::Green); // Draws Position String
		SpriteBatch::DrawString(scoreStream.str().c_str(), _scorePosition, Color::Yellow); // Draws Score String
	}

	// Draw pause menu
	if (_game->pauseMenu)
	{
		std::stringstream pauseStream;
		pauseStream << "PAUSED!";

		SpriteBatch::Draw(_menu->background, _menu->Rectangle, nullptr); // Menu Background
		SpriteBatch::DrawString(pauseStream.str().c_str(), _menu->titlePosition, Color::Red); // Menu String
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::Update(int elapsedTime)
{
	int i; // local variable
	
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState(); // Get Keyboard State
	Input::MouseState* mouseState = Input::Mouse::GetState(); // Get Mouse State

	if (!_game->started)
	{
		if (CheckMenuButtonPress(mouseState, _menu->startButton->rectangle) && mouseState->LeftButton == Input::ButtonState::PRESSED)
			_game->started = true;
		/*if (CheckMenuButtonPress(mouseState, _menu->quitButton->rectangle) && mouseState->LeftButton == Input::ButtonState::PRESSED)
			Quit the application */
	}
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);

		if (!_game->paused)
		{
			CheckInput(elapsedTime, keyboardState, mouseState);
			CheckViewportCollision();
			UpdatePacman(elapsedTime);
			for (i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdatePowerUp(_munchies[i], elapsedTime);
				if (CheckCollision(_pacman, _munchies[i]))
				{
					_munchies[i]->position->X = -100;
					_game->score++;
				}	
			}
			for (i = 0; i < CHERRYCOUNT; i++)
			{
				UpdatePowerUp(_cherries[i], elapsedTime);
				if (CheckCollision(_pacman, _cherries[i]))
				{
					_cherries[i]->position->X = -100;
					_game->score++;
				}
			}
			/*for (i = 0; i < ENEMYCOUNT; i++)
			{
				if (CollisionCheck(_pacman, _ghosts[i]))
				{
					_pacman->lifeUI->amount--;
					_game->paused = true;
				}
			} */
		}
	}
}	
	
#pragma region CheckFunctions

void Pacman::CheckInput(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime;

	// Pacman Movement 
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->position->X += pacmanSpeed; // Move Right
		_pacman->direction = 0; // Face Right
	}
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += pacmanSpeed; // Move Down
		_pacman->direction = 1; // Face Down
	}
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->position->X -= pacmanSpeed; // Move Left
		_pacman->direction = 2; // Face Left

	}
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->position->Y -= pacmanSpeed; // Move Up
		_pacman->direction = 3; // Face Up
	}
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	if (state->IsKeyDown(Input::Keys::P) && !_game->pKeyDown)
	{
		_game->pKeyDown = true;
		_game->paused = !_game->paused;
		_game->pauseMenu = !_game->pauseMenu;
	}

	if (state->IsKeyUp(Input::Keys::P))
		_game->pKeyDown = false;
}

void Pacman::CheckViewportCollision() 
{

	// Checks if the Pacman hit right edge
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth())
		_pacman->position->X = 0;

	// Checks if the Pacman hit left edge
	if (_pacman->position->X < 0)
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;

	// Checks if the Pacman hit bottom edge
	if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight())
		_pacman->position->Y = 0;

	// Checks if the Pacman hit top edge
	if (_pacman->position->Y < 0)
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
}

bool Pacman::CheckCollision(Player* pacman, PowerUp* powerUp)
{
	// Check x-axis collision
	bool collisionX = (pacman->position->X + pacman->sourceRect->Width >= powerUp->position->X &&
		powerUp->position->X + powerUp->sourceRect->Width >= pacman->position->X);

	// Check y-axis collision
	bool collisionY = (pacman->position->Y + pacman->sourceRect->Height >= powerUp->position->Y &&
		powerUp->position->Y + powerUp->sourceRect->Height >= pacman->position->Y);

	// If both are true objects collided
	return collisionX && collisionY;
}

bool Pacman::CheckMenuButtonPress(Input::MouseState* mouseState, Rect* button)
{
	// Check x-axis collision
	bool collisionX = (mouseState->X > button->X && mouseState->X < button->Width + button->X);
	bool collisionY = (mouseState->Y < button->Y && mouseState->Y > button->Height + button->Y);

	return collisionX && collisionY;
}

#pragma endregion Functions for checking

#pragma region PacmanFunctions

void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->currentFrameTime += elapsedTime;

	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction; // Change pacmans direction
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame; // Change Pacmans mouth

	// Pacman Animation Cycles
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;

		// If we're at the last frame cycle back to start
		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0; // Reset Frame Time
	}
}

void Pacman::PacmanHit(int elapsedTime, Player* pacman)
{
	/*
	int currentFrameTime = 0;
	currentFrameTime += elapsedTime;

	_pacman->sourceRect->Y = _pacman->sourceRect->Height * 0; // Change pacmans direction
	_pacman->sourceRect->X = _pacman->sourceRect->Width * 0; // Change Pacmans mouth

	// Pacman Animation Cycles
	if (currentFrameTime > pacman->2)
	{
		pacman->position->X = -100;
	}
	else
	{
		pacman->position = 0;
	}
	_pacman->lives--;
	*/
}

#pragma endregion The pacman Functions

void Pacman::UpdatePowerUp(PowerUp* powerUp, int elapsedTime)
{
	powerUp->currentFrameTime += elapsedTime;

	powerUp->sourceRect->X = powerUp->sourceRect->Width * powerUp->frameCount; // Change Munchie Frame

	// Munchie Animation Cycle
	if (powerUp->currentFrameTime > powerUp->frameTime)
	{
		powerUp->frameCount++;

		// If we're at the last frame cycle back to start
		if (powerUp->frameCount >= 2)
			powerUp->frameCount = 0;

		powerUp->currentFrameTime = 0; // Reset Frame Time
	}
}
