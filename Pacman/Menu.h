#pragma once

#include "S2D/S2D.h"

using namespace S2D;

struct Button
{
	~Button();
	Texture2D* background;
	Rect* rectangle;
	Vector2* position;
};

class Menu
{
public:
	Menu();
	~Menu();
	Texture2D* background;
	Rect* Rectangle;
	Vector2* titlePosition;
	Vector2* scorePosition;
	Button* startButton;
	Button* optionsButton;
	Button* quitButton;
	bool CheckMenuButtonPress(Input::MouseState* mouseState, Rect* button);
};

