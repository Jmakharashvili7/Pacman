#include "Pacman.h"

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 729, 810, true, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	int i; // Local Variable

	// Clean up the classes
	delete _gameManager;
	delete _redGhost;
	delete _pacman;
	delete _menu;
	delete _pop;

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
	delete _lifeUI->texture;
	delete _lifeUI->sourceRect;
	delete _lifeUI;
	for(i = 0; i < PACMANLIVES; i++)
		delete _lifeUI->positions[i];
}

void Pacman::LoadContent()
{
	srand((unsigned) time(0)); // Seed random nunmber generator with time

	int i; // local variable

	// initialize nodes
	_tiles = new Tile[TILECOUNTX * TILECOUNTY];
	for (int x = 0; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
		{
			float sizeX  = x*27, sizeY = y*27;
			_tiles[y * TILECOUNTX + x].positionInArray = y * TILECOUNTX + x;
			_tiles[y * TILECOUNTX + x].rectPosition = new Vector2(sizeX, sizeY);
			_tiles[y * TILECOUNTX + x].sourceRect = new Rect(0.0f, 0.0f, 27.0f, 27.0f);
			_tiles[y * TILECOUNTX + x].position = new Vector2(sizeX + (_tiles[y * TILECOUNTX + x].sourceRect->Width / 2),
															sizeY + (_tiles[y * TILECOUNTX + x].sourceRect->Height / 2));
			_tiles[y * TILECOUNTX + x].wall = false;
			_tiles[y * TILECOUNTX + x].parent = nullptr;
			_tiles[y * TILECOUNTX + x].visited = false;
		}

	// Setup neighbour nodes
	for (int x = 0; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
		{
			if (y > 0)
				_tiles[y * TILECOUNTX + x].nodeNeighbours.push_back(&_tiles[(y - 1) * TILECOUNTX + (x + 0)]);
			if (y < TILECOUNTY - 1)
				_tiles[y * TILECOUNTX + x].nodeNeighbours.push_back(&_tiles[(y + 1) * TILECOUNTX + (x + 0)]);
			if (x > 0)
				_tiles[y * TILECOUNTX + x].nodeNeighbours.push_back(&_tiles[(y + 0) * TILECOUNTX + (x - 1)]);
			if (x < TILECOUNTX - 1)
				_tiles[y * TILECOUNTX + x].nodeNeighbours.push_back(&_tiles[(y + 0) * TILECOUNTX + (x + 1)]);
		}

	#pragma region setting up walls
	// Draw top wall excluding the first and last block
	for (int x = 1; x < TILECOUNTX - 1; x++)
		for (int y = 0; y < 1; y++)
			_tiles[y * TILECOUNTX + x].wall = true;
	
	// Draw top wall excluding the first and last block
	for (int x = 1; x < TILECOUNTX - 1; x++)
		for (int y = TILECOUNTY - 1; y < TILECOUNTY; y++)
			_tiles[y * TILECOUNTX + x].wall = true;

	// Draw left wall
	for (int x = 0; x < 1; x++)
		for (int y = 0; y < TILECOUNTY; y++)
			_tiles[y * TILECOUNTX + x].wall = true;

	// Draw right wall
	for (int x = TILECOUNTX - 1; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
			_tiles[y * TILECOUNTX + x].wall = true;
    #pragma endregion 

	// Menu, Game Manager, pacman setup
	_menu = new Menu();
	_gameManager = new GameManager();
	_pacman = new PlayerClass();
	_pop = new SoundEffect();
	_pop->Load("Sounds/pop.wav");

	// Manually position the current and goal nodes so they are not empty
	_redGhost = new EnemyGhost(Red);
	_redGhost->currentTile = &_tiles[1];
	_redGhost->tileGoal = &_tiles[1];

	// Same as above for pink ghost
	_pinkGhost = new EnemyGhost(Pink);
	_pinkGhost->currentTile = &_tiles[1];
	_pinkGhost->tileGoal = &_tiles[1];

	// Load pointer for munchie texture 
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", false);

	// Initialise Munchies
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new PowerUp();
		_munchies[i]->texture = munchieTex;
		_munchies[i]->sourceRect = new Rect(0, 0, 12.0f, 12.0f);
	}

	//Load pointer for cherry texture
	Texture2D* cherryTex = new Texture2D();
	cherryTex->Load("Textures/Cherry.png", false);

	// Initialise Cherries
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i] = new PowerUp();
		_cherries[i]->texture = cherryTex;
		_cherries[i]->sourceRect = new Rect(0.0f, 0.0f, 32.0f, 32.0f);
	}

	// Load Pacman Life UI
	_lifeUI = new LifeUI();
	_lifeUI->amount = PACMANLIVES;
	_lifeUI->texture = new Texture2D();
	_lifeUI->texture->Load("Textures/PacmanLives.tga", false);
	_lifeUI->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	for (i = 0; i < PACMANLIVES; i++)
		_lifeUI->positions[i] = new Vector2(5.0f + (32.0f * i), 60.0f);

	// Load Cherries
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i]->texture = cherryTex;
		_cherries[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}
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
	positionStream << "Pacman X: " << _pacman->rectPosition->X << " Y: " << _pacman->rectPosition->Y;
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
		int i , j; // Local Variable

		// Draw all the tiles
		for (int x = 0; x < TILECOUNTX; x++)
			for (int y = 0; y < TILECOUNTY; y++)
			{
				if (_tiles[y * TILECOUNTX + x].wall)
					SpriteBatch::DrawRectangle(_tiles[y * TILECOUNTX + x].rectPosition, _tiles[y * TILECOUNTX + x].sourceRect->Height, 
						_tiles[y * TILECOUNTX + x].sourceRect->Width, Color::Red);
			}

		// Draw the munchies
		for (i = 0; i < MUNCHIECOUNT; i++)
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect); // Draws Munchies

		// Draw the cherries
		for (i = 0; i < CHERRYCOUNT; i++)
			SpriteBatch::Draw(_cherries[i]->texture, _cherries[i]->position, _cherries[i]->sourceRect); // Draws Cherries

		// draw pacman if he isnt invisible
		if (!_pacman->invisible)
		SpriteBatch::Draw(_pacman->texture, _pacman->rectPosition, _pacman->sourceRect); 

		// Draw Pacman Health UI
		for (i = 0; i < _pacman->currentLives; i++)
			SpriteBatch::Draw(_lifeUI->texture, _lifeUI->positions[i], _lifeUI->sourceRect);

		// Draws Score String
		SpriteBatch::DrawString(scoreStream.str().c_str(), _menu->scorePosition, Color::Yellow); 

		 // Draw Red Ghost
		SpriteBatch::Draw(_redGhost->texture, _redGhost->rectPosition, _redGhost->sourceRect);

		// Draw Pink Ghost
		SpriteBatch::Draw(_pinkGhost->texture, _pinkGhost->rectPosition, _pinkGhost->sourceRect);
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
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);

		//Check if pacman is alive
		if (_pacman->dead)
		{
			_pacman->PacmanHit(elapsedTime);
		}

		if (!_gameManager->paused && !_pacman->dead)
		{
			CheckViewportCollision();

			MarkupWalls(mouseState, _tiles);

			for (int x = 0; x < TILECOUNTX; x++)
				for (int y = 0; y < TILECOUNTY; y++)
				{
					
				}

			// Pacman functions
			_pacman->CheckInput(keyboardState);
			_pacman->UpdatePacman(elapsedTime);
			_pacman->SetCurrentTile(_tiles);
			_pacman->Movement(elapsedTime, _tiles);

			// Update munchies
			for (i = 0; i < MUNCHIECOUNT; i++)
			{
				_munchies[i]->UpdatePowerUp(elapsedTime);
				if (CheckCollision(_pacman, _munchies[i]))
				{
					_munchies[i]->position->X = -100;
					_gameManager->score++;
				}	
			}

			// Update cherries
			for (i = 0; i < CHERRYCOUNT; i++)
			{
				_cherries[i]->UpdatePowerUp(elapsedTime);
				if (CheckCollision(_pacman, _cherries[i]))
				{
					_cherries[i]->position->X = -100;
					_gameManager->score++;
				}
			}	

			// Red Ghost functions
			_redGhost->SetCurrentNode(_tiles);
			_redGhost->GhostAI(elapsedTime, _tiles, _pacman);
			_redGhost->GhostAnimation();

			_pinkGhost->SetCurrentNode(_tiles);
			_pinkGhost->GhostAI(elapsedTime, _tiles, _pacman);
			_pinkGhost->GhostAnimation();

			// If pacman collided with ghost kill pacman
			if (CheckCollision(_pacman, _redGhost))
				{
					_pacman->dead = true; // kill pacman
					Audio::Play(_pop);
					_pacman->currentLives--;
					_redGhost->rectPosition->X = -100; // reset ghost
				}
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
	if (_pacman->rectPosition->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth())
		_pacman->rectPosition->X = 0;

	// Checks if the Pacman hit left edge
	if (_pacman->rectPosition->X < 0)
		_pacman->rectPosition->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;

	// Checks if the Pacman hit bottom edge
	if (_pacman->rectPosition->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight())
		_pacman->rectPosition->Y = 0;

	// Checks if the Pacman hit top edge
	if (_pacman->rectPosition->Y < 0)
		_pacman->rectPosition->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
}

bool Pacman::CheckCollision(PlayerClass* pacman, PowerUp* powerUp)
{
	// Check x-axis collision
	bool collisionX = (pacman->rectPosition->X + pacman->sourceRect->Width >= powerUp->position->X &&
		powerUp->position->X + powerUp->sourceRect->Width >= pacman->rectPosition->X);

	// Check y-axis collision
	bool collisionY = (pacman->rectPosition->Y + pacman->sourceRect->Height >= powerUp->position->Y &&
		powerUp->position->Y + powerUp->sourceRect->Height >= pacman->rectPosition->Y);

	// If both are true objects collided
	return collisionX && collisionY;
}

bool Pacman::CheckCollision(PlayerClass* pacman, EnemyGhost* ghost)
{
	// Check x-axis collision
	bool collisionX = (pacman->rectPosition->X + pacman->sourceRect->Width >= ghost->rectPosition->X &&
		ghost->rectPosition->X + ghost->sourceRect->Width >= pacman->rectPosition->X);

	// Check y-axis collision
	bool collisionY = (pacman->rectPosition->Y + pacman->sourceRect->Height >= ghost->rectPosition->Y &&
		ghost->rectPosition->Y + ghost->sourceRect->Height >= pacman->rectPosition->Y);

	// If both are true objects collided
	return collisionX && collisionY;
}

void Pacman::MarkupWalls(Input::MouseState* mouseState, Tile* tiles)
{
	// Check x-axis collision
	for (int x = 0; x < TILECOUNTX; x++)
		for (int y = 0; y < TILECOUNTY; y++)
		{
			bool collisionX = (mouseState->X > tiles[y * TILECOUNTX + x].rectPosition->X &&
				mouseState->X < tiles[y * TILECOUNTX + x].sourceRect->Width + tiles[y * TILECOUNTX + x].rectPosition->X);

			bool collisionY = (mouseState->Y > tiles[y * TILECOUNTX + x].rectPosition->Y && 
				mouseState->Y < tiles[y * TILECOUNTX + x].sourceRect->Height + tiles[y * TILECOUNTX + x].rectPosition->Y);

			if (collisionY && collisionX && mouseState->LeftButton == Input::ButtonState::PRESSED)
			{
				printf("Tile: %d\n", tiles[y * TILECOUNTX + x].positionInArray);
				tiles[y * TILECOUNTX + x].wall = true;
			}
		}
}
