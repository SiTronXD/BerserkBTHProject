#pragma once

#include <string>

class SettingsHandler
{
private:
	// 1280, 720	16:9
	// 1920, 1080	16:9
	// 1680, 720	21:9
	// 2520, 1080	21:9	(2560, 1080)
	static unsigned int windowWidth;
	static unsigned int windowHeight;
	static float mouseSensitivity;
	static float soundEffectsVolume;
	static float musicVolume;
	static bool fullscreen;

	SettingsHandler(const SettingsHandler& other) = delete;
	SettingsHandler operator=(const SettingsHandler& other) = delete;

public:
	static void loadSettings();

	static unsigned int getWidth();
	static unsigned int getHeight();

	static float getMouseSensitivity();
	static float getSoundEffectsVolume();
	static float getMusicVolume();

	static bool getIsFullscreen();
};