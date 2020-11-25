#pragma once

class SettingsHandler
{
private:
	static const unsigned int WINDOW_WIDTH = 1280;	// 1680
	static const unsigned int WINDOW_HEIGHT = 720;

public:
	static unsigned int getWidth();
	static unsigned int getHeight();
	static float getWindowSizeFactor(float height);
};