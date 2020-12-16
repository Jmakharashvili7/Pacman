#include "Pacman.h"

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, TILECOUNTX * TILESIZE, TILECOUNTY * TILESIZE, true, 25, 25, "Pacman", 60);
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
	for (i = 0; i < _munchies.size(); i++)
	{
		delete _munchies[i];
	}
	_munchies.clear();

	// Clean up cherries
	for (i = 0; i < _cherries.size(); i++)
	{
		delete _cherries[i];
	}
	_cherries.clear();

	// Clean up Pacman Lives UI
	delete _lifeUI->texture;
	delete _lifeUI->sourceRect;
	delete _lifeUI;
	for(i = 0; i < PACMANLIVES; i++)
		delete _lifeUI->positions[i];
}

void Pacman::LoadContent()
{
	srand((unsigned)time(0)); // Seed random nunmber generator with time

	int i; // local variable

	// The level map
	_map += L"############################";
	_map += L"#............##............#";
	_map += L"#.####.#####.##.#####.####.#";
	_map += L"#o####.#####.##.#####.####o#";
	_map += L"#.####.#####.##.#####.####.#";
	_map += L"#..........................#";
	_map += L"#.####.##.########.##.####.#";
	_map += L"#.####.##.########.##.####.#";
	_map += L"#......##....##....##......#";
	_map += L"######.##### ## #####.######";
	_map += L"######.##### ## #####.######";
	_map += L"######.##          ##.######";
	_map += L"######.## ######## ##.######";
	_map += L"######.## ######## ##.######";
	_map += L"#......   ########   ......#";
	_map += L"######.## ######## ##.######";
	_map += L"######.## ######## ##.######";
	_map += L"######.##          ##.######";
	_map += L"######.## ######## ##.######";
	_map += L"######.## ######## ##.######";
	_map += L"#............##............#";
	_map += L"#.####.#####.##.#####.####.#";
	_map += L"#.####.#####.##.#####.####.#";
	_map += L"#o..##................##..o#";
	_map += L"###.##.##.########.##.##.###";
	_map += L"###.##.##.########.##.##.###";
	_map += L"#......##....##....##......#";
	_map += L"#.##########.##.##########.#";
	_map += L"#.##########.##.##########.#";
	_map += L"#..........................#";
	_map += L"############################";

	// Get what type the tile is based on the map
	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < TILECOUNTX && y >= 0 && y < TILECOUNTY)
			return _map[y*TILECOUNTX + x];
		else
			return L'x'; // error tile out of bounds
	};

	// Load pointer for munchie texture 
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", false);

	//Load pointer for cherry texture
	Texture2D* cherryTex = new Texture2D();
	cherryTex->Load("Textures/Cherry.png", false);

	// initialize tiles
	// i is used for munchie count and j is used for fruit count
	// x and y are for cordinates
	_tiles = new Tile[TILECOUNTX * TILECOUNTY];
	for (int x = 0, i = 0, j = 0; x < TILECOUNTX; x++) 
		for (int y = 0; y < TILECOUNTY; y++)
		{
			float sizeX = x*TILESIZE, sizeY = y*TILESIZE;
			_tiles[y * TILECOUNTX + x].posInArray = y * TILECOUNTX + x;
			_tiles[y * TILECOUNTX + x].sourceRect = new Rect(sizeX, sizeY, 27.0f, 27.0f);
			_tiles[y * TILECOUNTX + x].position = new Vector2(_tiles[y * TILECOUNTX + x].sourceRect->Center());
			wchar_t tileID = GetTile(x, y);

			switch (tileID)
			{
			case L'.': 
				_munchies.push_back(new Munchie());
				_munchies[i]->texture = munchieTex;
				_munchies[i]->position = new Vector2(_tiles[y * TILECOUNTX + x].position->X-5, _tiles[y * TILECOUNTX + x].position->Y-5);
				i++;
				break;
			case L'o':
				_cherries.push_back(new Fruit());
				_cherries[j]->texture = cherryTex;
				_cherries[j]->position = new Vector2(_tiles[y * TILECOUNTX + x].sourceRect->X, _tiles[y * TILECOUNTX + x].sourceRect->Y);
				j++;
				break;
			case L'#':
				//_tiles[y * TILECOUNTX + x].wall = true;
				break;
			}
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

	// Menu, Game Manager, pacman setup
	_menu = new Menu();
	_gameManager = new GameManager();
	_pacman = new PlayerClass(&_tiles[30]);
	_pop = new SoundEffect();
	_pop->Load("Sounds/pop.wav");


	// Manually position the current and goal nodes so they are not empty
	_redGhost = new EnemyGhost(Red, &_tiles[50]);
	_redGhost->currentTile = &_tiles[1];
	_redGhost->tileGoal = &_tiles[1];
	/*
	// Same as above for pink ghost
	_pinkGhost = new EnemyGhost(Pink);
	_pinkGhost->currentTile = &_tiles[1];
	_pinkGhost->tileGoal = &_tiles[1];
	*/

	// Load Pacman Life UI
	_lifeUI = new LifeUI();
	_lifeUI->amount = PACMANLIVES;
	_lifeUI->texture = new Texture2D();
	_lifeUI->texture->Load("Textures/PacmanLives.tga", false);
	_lifeUI->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	for (i = 0; i < PACMANLIVES; i++)
		_lifeUI->positions[i] = new Vector2(5.0f + (32.0f * i), 60.0f);
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
					SpriteBatch::DrawRectangle(_tiles[y * TILECOUNTX + x].sourceRect, Color::Red);
			}

		// Draw the munchies
		for (i = 0; i < _munchies.size(); i++)
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect); // Draws Munchies
		
		// Draw the cherries
		for (i = 0; i < _cherries.size(); i++)
			SpriteBatch::Draw(_cherries[i]->texture, _cherries[i]->position, _cherries[i]->sourceRect); // Draws Cherries
		
		// draw pacman if he isnt invisible
		if (!_pacman->invisible)
		SpriteBatch::Draw(_pacman->texture, _pacman->sourceRect, _pacman->textRect); 

		// Draw Pacman Health UI
		for (i = 0; i < _pacman->currentLives; i++)
			SpriteBatch::Draw(_lifeUI->texture, _lifeUI->positions[i], _lifeUI->sourceRect);

		// Draws Score String
		SpriteBatch::DrawString(scoreStream.str().c_str(), _menu->scorePosition, Color::Yellow); 

		 // Draw Red Ghost
		SpriteBatch::Draw(_redGhost->texture, _redGhost->sourceRect, _redGhost->textureRect);

		// Draw Pink Ghost
		//SpriteBatch::Draw(_pinkGhost->texture, _pinkGhost->rectPosition, _pinkGhost->sourceRect);
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

			// Pacman functions
			_pacman->UpdatePacman(elapsedTime);
			_pacman->SetCurrentTile(_tiles);

			// Check input if we're at the center of the tile
			if (abs(_pacman->position->X - _pacman->currentTile->position->X) < 1.0f)
				if (abs(_pacman->position->Y - _pacman->currentTile->position->Y) < 1.0f)
					_pacman->CheckInput(keyboardState);

			_pacman->Movement(elapsedTime, _tiles);

			// Update munchies
			for (i = 0; i < _munchies.size(); i++)
			{
				_munchies[i]->UpdatePowerUp(elapsedTime);
				if (CheckCollision(_pacman, _munchies[i]))
				{
					_munchies[i]->position->X = -100;
					_gameManager->score++;
				}	
			}
			
			// Update cherries
			for (i = 0; i < _cherries.size(); i++)
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

			/*
			_pinkGhost->SetCurrentNode(_tiles);
			_pinkGhost->GhostAI(elapsedTime, _tiles, _pacman);
			_pinkGhost->GhostAnimation();
			*/
			// If pacman collided with ghost kill pacman
			if (CheckCollision(_pacman, _redGhost))
				{
					_pacman->dead = true; // kill pacman
					Audio::Play(_pop);
					_pacman->currentLives--;
					_redGhost->sourceRect->X = -100; // reset ghost
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
	if (_pacman->sourceRect->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth())
		_pacman->sourceRect->X = 0;

	// Checks if the Pacman hit left edge
	if (_pacman->sourceRect->X < 0)
		_pacman->sourceRect->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;

	// Checks if the Pacman hit bottom edge
	if (_pacman->sourceRect->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight())
		_pacman->sourceRect->Y = 0;

	// Checks if the Pacman hit top edge
	if (_pacman->sourceRect->Y < 0)
		_pacman->sourceRect->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
}

bool Pacman::CheckCollision(PlayerClass* pacman, PowerUp* powerUp)
{
	// Check x-axis collision
	bool collisionX = (pacman->sourceRect->X + pacman->sourceRect->Width >= powerUp->position->X &&
		powerUp->position->X + powerUp->sourceRect->Width >= pacman->sourceRect->X);

	// Check y-axis collision
	bool collisionY = (pacman->sourceRect->Y + pacman->sourceRect->Height >= powerUp->position->Y &&
		powerUp->position->Y + powerUp->sourceRect->Height >= pacman->sourceRect->Y);

	// If both are true objects collided
	return collisionX && collisionY;
}

bool Pacman::CheckCollision(PlayerClass* pacman, EnemyGhost* ghost)
{
	// Check x-axis collision
	bool collisionX = (pacman->sourceRect->X + pacman->sourceRect->Width >= ghost->sourceRect->X &&
		ghost->sourceRect->X + ghost->textureRect->Width >= pacman->sourceRect->X);

	// Check y-axis collision
	bool collisionY = (pacman->sourceRect->Y + pacman->sourceRect->Height >= ghost->sourceRect->Y &&
		ghost->sourceRect->Y + ghost->textureRect->Height >= pacman->sourceRect->Y);

	// If both are true objects collided
	return collisionX && collisionY;
}

