#include "SettingsHandler.h"
#include <fstream>
#include <SFML/Window.hpp>

unsigned int SettingsHandler::windowWidth;
unsigned int SettingsHandler::windowHeight;
float SettingsHandler::mouseSensitivity;
float SettingsHandler::soundEffectsVolume;
float SettingsHandler::musicVolume;
bool SettingsHandler::fullscreen;

void SettingsHandler::loadSettings()
{
	// Load file
	std::ifstream in;
	in.open("Resources/Settings.ini");

	// Open file
	if (in.is_open())
	{
		std::string currentLine;

		// Go through each line
		while (std::getline(in, currentLine))
		{
			// Mouse sensitivity
			if (currentLine == "<Mouse Sensitivity>")
			{
				std::getline(in, currentLine);

				mouseSensitivity = std::stof(currentLine);

			}
			// Fullscreen
			else if (currentLine == "<Fullscreen>")
			{
				std::getline(in, currentLine);

				fullscreen = (currentLine == "yes");

				if (fullscreen)
				{
					windowWidth = sf::VideoMode::getDesktopMode().width;
					windowHeight = sf::VideoMode::getDesktopMode().height;
				}
			}
			// Window width
			else if (currentLine == "<Window Width>")
			{
				std::getline(in, currentLine);

				if(!fullscreen)
					windowWidth = std::stoi(currentLine);
			}
			// Window height
			else if (currentLine == "<Window Height>")
			{
				std::getline(in, currentLine);

				if (!fullscreen)
					windowHeight = std::stoi(currentLine);
			}
			// Sound effects volume
			else if (currentLine == "<Sound Effects Volume>")
			{
				std::getline(in, currentLine);

				soundEffectsVolume = std::stof(currentLine);
			}
			// Music volume
			else if (currentLine == "<Music Volume>")
			{
				std::getline(in, currentLine);

				musicVolume = std::stof(currentLine);
			}
		}
	}
	else
		std::cout << "Could not open settings file." << std::endl;

	// Close, even if the file was never opened
	in.close();
}

unsigned int SettingsHandler::getWidth()
{
	return windowWidth;
}

unsigned int SettingsHandler::getHeight()
{
	return windowHeight;
}

float SettingsHandler::getMouseSensitivity()
{
	return mouseSensitivity;
}

bool SettingsHandler::getIsFullscreen()
{
	return fullscreen;
}

float SettingsHandler::getSoundEffectsVolume()
{
	return soundEffectsVolume;
}

float SettingsHandler::getMusicVolume()
{
	return musicVolume;
}
