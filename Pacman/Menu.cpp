#include "Menu.h"

Menu::Menu()
{
	float screenMidWidth = Graphics::GetViewportWidth() / 2;
	float screenMidHeight = Graphics::GetViewportHeight() / 2;

	// Set Menu Parameters
	background = new Texture2D();
	background->Load("Textures/Transparency.png", false);
	titlePosition = new Vector2(screenMidWidth - 20.0f, screenMidHeight);
	Rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	scorePosition = new Vector2(10.0f, 50.0f);

	// Setup start button
	startButton = new Button();
	startButton->background = new Texture2D();
	startButton->background->Load("Textures/Transparency.png", false);
	startButton->position = new Vector2(screenMidWidth - 20.0f, screenMidHeight + 25.0f);
	startButton->rectangle = new Rect(startButton->position->X, startButton->position->Y, 50.0f, -15.0f);

	// Setup options button
	optionsButton = new Button();
	optionsButton->background = new Texture2D();
	optionsButton->background->Load("Textures/Transparency.png", false);
	optionsButton->position = new Vector2(screenMidWidth - 20.0f, screenMidHeight + 50.0f);
	optionsButton->rectangle = new Rect(optionsButton->position->X, optionsButton->position->Y, 65.0, -15.0f);

	// Setup quit button
	quitButton = new Button();
	quitButton->background = new Texture2D();
	quitButton->background->Load("Textures/Transparency.png", false);
	quitButton->position = new Vector2(screenMidWidth - 20.0f, screenMidHeight + 75.0f);
	quitButton->rectangle = new Rect(quitButton->position->X, quitButton->position->Y, 50.0, -15.0f);
}

Button::~Button()
{
	delete background;
	delete rectangle;
	delete position;
}

Menu::~Menu()
{
	// Clean up String positions
	delete scorePosition;
	delete titlePosition;

	//Clean up Menu and Buttons
	delete background;
	delete Rectangle;
	delete startButton;
	delete optionsButton;
	delete quitButton;
}

bool Menu::CheckMenuButtonPress(Input::MouseState* mouseState, Rect* button)
{
	// Check x-axis collision
	bool collisionX = (mouseState->X > button->X && mouseState->X < button->Width + button->X);
	bool collisionY = (mouseState->Y < button->Y&& mouseState->Y > button->Height + button->Y);

	return collisionX && collisionY;
}