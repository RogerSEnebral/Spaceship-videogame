#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "TitleScreen.h"
#include "ShaderProgramSetter.h"
#include "SoundManager.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	bool getMousePressed() const;
	bool getKeyPress(int key);
	bool getSpecialKeyPress(int key);

private:
	void inTitle(int deltaTime);
	void playing(int deltaTime);

	bool bPlay, screen;                       // Continue to play game?
	Scene *scene;                      // Scene to render
	TitleScreen *titleScreen;
	bool keys[256], specialKeys[256], keyPresses[256], specialKeyPresses[256];// Store key states so that we can have access at any time

	bool mousePressed, gameFinished, restart; 
	int lives, timeSinceGameEnd;
};


#endif // _GAME_INCLUDE


