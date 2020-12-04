#include "SettingsHandler.h"
#include <cmath>

const float SettingsHandler::MOUSE_SENSITIVITY = 2.0f;
const float SettingsHandler::KEYBOARD_LOOK_SENSITIVITY = 7.0f;


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

float SettingsHandler::getKeyboardLookSensitivity()
{
	return KEYBOARD_LOOK_SENSITIVITY;
}
