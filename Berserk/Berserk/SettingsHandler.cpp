#include "SettingsHandler.h"

unsigned int SettingsHandler::getWidth()
{
	return WINDOW_WIDTH;
}

unsigned int SettingsHandler::getHeight()
{
	return WINDOW_HEIGHT;
}

float SettingsHandler::getWindowSizeFactor(float height)
{
	return WINDOW_HEIGHT / height;
}
