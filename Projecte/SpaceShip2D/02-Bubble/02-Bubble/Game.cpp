#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define TITLE true
#define PLAYING false

#define TIME_END_GAME 8000

void Game::init()
{

	ShaderProgramSetter::getInstance().init(SCREEN_WIDTH, SCREEN_HEIGHT);
	SoundManager::instance().init();

	screen = TITLE;
	bPlay = true;
	restart = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	titleScreen = new TitleScreen();
	titleScreen->init();

	timeSinceGameEnd = 0;

	irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/titleTheme.mp3", true, false, true);
	sound->setVolume(0.3f);
}

bool Game::update(int deltaTime)
{
	if (screen == TITLE)
	{
		inTitle(deltaTime);
	}
	else 
	{
		playing(deltaTime);
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (screen == TITLE) titleScreen->render();
	else scene->render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		if(screen == TITLE) bPlay = false;
		else
		{
			timeSinceGameEnd = TIME_END_GAME;
			gameFinished = true;
			titleScreen->init();
		}
	keys[key] = true;
	keyPresses[key] = true;
	if (key == 'r' && screen == PLAYING && scene->getGameOver())
	{
		restart = true;
	}
}

void Game::keyReleased(int key)
{
	if (key == 'g')
	{
		if (screen == PLAYING) scene->invulnerability();

	}
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
	specialKeyPresses[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
	mousePressed = true;
}

void Game::mouseRelease(int button)
{
	mousePressed = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

bool Game::getMousePressed() const
{
	return mousePressed;
}

bool Game::getKeyPress(int key)
{
	if (keyPresses[key])
	{
		keyPresses[key] = false;
		return true;
	}
	return false;
}

bool Game::getSpecialKeyPress(int key)
{
	if (specialKeyPresses[key])
	{
		specialKeyPresses[key] = false;
		return true;
	}
	return false;
}


void Game::inTitle(int deltaTime)
{
	titleScreen->update(deltaTime);
	if (titleScreen->getPlay())
	{
		lives = 3;
		gameFinished = false;

		scene = new Scene();
		scene->init(lives);

		screen = PLAYING;

	}
}

void Game::playing(int deltaTime)
{
	if (!gameFinished)
	{
		if (scene->getVictory())
		{
			gameFinished = true;

			SoundManager::instance().getSoundEngine()->removeAllSoundSources();
			irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/victorySong.mp3", false, false, true);
			sound->setVolume(0.3f);
		}

		else if (!scene->getGameOver())
		{
			scene->update(deltaTime);
		}

		else
		{
			if (lives > 1)
			{
				if (restart)
				{
					lives--;
					scene = new Scene();
					scene->init(lives);
					restart = false;
				}
			}
			else
			{
				gameFinished = true;

				SoundManager::instance().getSoundEngine()->removeAllSoundSources();
				irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/loseSong.mp3", false, false, true);
				sound->setVolume(0.5f);
			}

		}
	}
	else
	{
		timeSinceGameEnd += deltaTime;
		if (timeSinceGameEnd > TIME_END_GAME)
		{
			ShaderProgramSetter::getInstance().setProjection(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
			titleScreen->init();
			screen = TITLE;
			timeSinceGameEnd = 0;

			SoundManager::instance().getSoundEngine()->removeAllSoundSources();
			irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/titleTheme.mp3", true, false, true);
			sound->setVolume(0.3f);
		}
		else if (scene->getVictory()) scene->update(deltaTime);
	}
}