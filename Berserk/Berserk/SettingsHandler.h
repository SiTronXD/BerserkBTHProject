#pragma once

class SettingsHandler
{
private:
	// 1280, 720	16:9
	// 1920, 1080	16:9
	// 1680, 720	21:9
	// 2520, 1080	21:9	(2560, 1080)
	static const unsigned int WINDOW_WIDTH = 1680;
	static const unsigned int WINDOW_HEIGHT = 720;
	static const float MOUSE_SENSITIVITY;
	static const float KEYBOARD_LOOK_SENSITIVITY;

public:
	static unsigned int getWidth();
	static unsigned int getHeight();

	static float getMouseSensitivity();
	static float getKeyboardLookSensitivity();
};