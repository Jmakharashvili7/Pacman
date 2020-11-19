#pragma once

#include "S2D/S2D.h"


using namespace S2D;

class GameManager
{
public:
	GameManager();
	int score;
	bool started;
	bool paused;
	bool pauseMenu;
	bool pKeyDown;
};

