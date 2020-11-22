#include "GameManager.h"
#include "Pacman.h"

GameManager::GameManager()
{
	score = 0;
	paused = false;
	started = false;
	pKeyDown = false;
	pauseMenu = false;
}