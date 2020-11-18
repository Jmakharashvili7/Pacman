#pragma once

#include "S2D/S2D.h"
#include "Pacman.h" // for button

using namespace S2D;

class Menu
{
	Texture2D* background;
	Rect* Rectangle;
	Vector2* titlePosition;
	Button* startButton;
	Button* optionsButton;
	Button* quitButton;
};

