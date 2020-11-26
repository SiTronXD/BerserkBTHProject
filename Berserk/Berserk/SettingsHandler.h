#pragma once

class SettingsHandler
{
private:
	// 1280, 720	16:9
	// 1920, 1080	16:9
	// 1680, 720	21:9
	// 2520, 1080	21:9	(2560, 1080)
	static const unsigned int WINDOW_WIDTH = 1280;
	static const unsigned int WINDOW_HEIGHT = 720;

public:
	static unsigned int getWidth();
	static unsigned int getHeight();
};