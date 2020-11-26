#include "SettingsHandler.h"

const float SettingsHandler::MOUSE_SENSITIVITY = 2.0f;


unsigned int SettingsHandler::getWidth()
{
	return WINDOW_WIDTH;
}

unsigned int SettingsHandler::getHeight()
{
	return WINDOW_HEIGHT;
}

float SettingsHandler::getMouseSensitivity()
{
	return MOUSE_SENSITIVITY;
}
