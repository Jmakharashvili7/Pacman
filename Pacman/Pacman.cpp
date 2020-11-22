#include "Pacman.h"

#include <sstream>
#include <ctime>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	_gameManager = new GameManager();

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1080, 720, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	int i; // Local Variable

	// Clean up the classes
	delete _gameManager;
	delete _enemyGhost;
	delete _pacman;
	delete _menu;

	// Clean up munchies
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i];
	}
	delete[] _munchies;

	// Clean up cherries
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		delete _cherries[i];
	}
	delete[] _cherries;

	// Clean up Pacman Lives UI
	delete lifeUI->texture;
	delete lifeUI->sourceRect;
	delete lifeUI;
	for(i = 0; i < PACMANLIVES; i++)
		delete lifeUI->positions[i];
}

void Pacman::LoadContent()
{
	srand((unsigned) time(0)); // Seed random nunmber generator with time

	int i; // local variable

	_enemyGhost = new EnemyGhost();
	_pacman = new PlayerClass();
	lifeUI = new LifeUI();
	lifeUI->amount = PACMANLIVES;

	// Load pointer for munchie texture 
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", false);

	//Load pointer for cherry texture
	Texture2D* cherryTex = new Texture2D();
	cherryTex->Load("Textures/Cherry.png", false);

	// Initialise Munchies
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new PowerUp();
		_munchies[i]->texture = munchieTex;
		_munchies[i]->sourceRect = new Rect(0, 0, 12.0f, 12.0f);
	}

	// Initialise Cherries
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i] = new PowerUp();
		_cherries[i]->texture = cherryTex;
		_cherries[i]->sourceRect = new Rect(0.0f, 0.0f, 32.0f, 32.0f);
	}

	// Load Pacman Lives
	lifeUI->texture = new Texture2D();
	lifeUI->texture->Load("Textures/PacmanLives.tga", false);
	lifeUI->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	for (i = 0; i < PACMANLIVES; i++)
		lifeUI->positions[i] = new Vector2(5.0f + (32.0f * i), 60.0f);

	// Load Cherries
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i]->texture = cherryTex;
		_cherries[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}

	_menu = new Menu();
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
	scoreStream << "Score : " << _gameManager->score;
#pragma endregion Set up the strings

	SpriteBatch::BeginDraw(); // Starts Drawing

	// If the game hasn't started draw the main menu
	if (!_gameManager->started)
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

		// draw pacman if he isnt invisible
		if (!_pacman->invisible)
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); 

		// Draw Pacman Health UI
		for (i = 0; i < _pacman->currentLives; i++)
			SpriteBatch::Draw(lifeUI->texture, lifeUI->positions[i], lifeUI->sourceRect);

		SpriteBatch::DrawString(scoreStream.str().c_str(), _menu->scorePosition, Color::Yellow); // Draws Score String

		SpriteBatch::Draw(_enemyGhost->texture, _enemyGhost->position, _enemyGhost->sourceRect); // Draw ghost
	}

	// Draw pause menu
	if (_gameManager->pauseMenu)
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

	if (!_gameManager->started)
	{
		if (_menu->CheckMenuButtonPress(mouseState, _menu->startButton->rectangle) && mouseState->LeftButton == Input::ButtonState::PRESSED)
			_gameManager->started = true;
		if (_menu->CheckMenuButtonPress(mouseState, _menu->quitButton->rectangle) && mouseState->LeftButton == Input::ButtonState::PRESSED)
			exit(0);
	}
	if (_pacman->dead)
	{
		_pacman->PacmanHit(elapsedTime);
	}
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);

		if (!_gameManager->paused && !_pacman->dead)
		{
			CheckViewportCollision();

			_pacman->CheckInput(elapsedTime, keyboardState);
			_pacman->UpdatePacman(elapsedTime);

			for (i = 0; i < MUNCHIECOUNT; i++)
			{
				_munchies[i]->UpdatePowerUp(elapsedTime);
				if (CheckCollision(_pacman, _munchies[i]))
				{
					_munchies[i]->position->X = -100;
					_gameManager->score++;
				}	
			}

			for (i = 0; i < CHERRYCOUNT; i++)
			{
				_cherries[i]->UpdatePowerUp(elapsedTime);
				if (CheckCollision(_pacman, _cherries[i]))
				{
					_cherries[i]->position->X = -100;
					_gameManager->score++;
				}
			}

			//for (i = 0; i < GHOSTCOUNT; i++)
			//{
				_enemyGhost->GhostMovement(elapsedTime);
				_enemyGhost->GhostAnimation();
				if (CheckCollision(_pacman, _enemyGhost))
				{
					_pacman->currentLives--;
					_pacman->dead = true;
					_enemyGhost->position->X = -100;
				}
			//} 
		}
	}
}	

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	if (state->IsKeyDown(Input::Keys::P) && !_gameManager->pKeyDown)
	{
		_gameManager->pKeyDown = true;
		_gameManager->paused = !_gameManager->paused;
		_gameManager->pauseMenu = !_gameManager->pauseMenu;
	}

	if (state->IsKeyUp(Input::Keys::P))
		_gameManager->pKeyDown = false;
}

void Pacman::CheckViewportCollision()
{

	// Checks if the Pacman hit right edge
	if (_pacman->position->X + _pacman->sourceRect->Width >= Graphics::GetViewportWidth())
		_pacman->position->X = 0;

	// Checks if the Pacman hit left edge
	if (_pacman->position->X <= 0)
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;

	// Checks if the Pacman hit bottom edge
	if (_pacman->position->Y + _pacman->sourceRect->Height >= Graphics::GetViewportHeight())
		_pacman->position->Y = 0;

	// Checks if the Pacman hit top edge
	if (_pacman->position->Y <= 0)
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
}

bool Pacman::CheckCollision(PlayerClass* pacman, PowerUp* powerUp)
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

bool Pacman::CheckCollision(PlayerClass* pacman, EnemyGhost* ghost)
{
	// Check x-axis collision
	bool collisionX = (pacman->position->X + pacman->sourceRect->Width >= ghost->position->X &&
		ghost->position->X + ghost->sourceRect->Width >= pacman->position->X);

	// Check y-axis collision
	bool collisionY = (pacman->position->Y + pacman->sourceRect->Height >= ghost->position->Y &&
		ghost->position->Y + ghost->sourceRect->Height >= pacman->position->Y);

	// If both are true objects collided
	return collisionX && collisionY;
}

