#pragma once
#include "Texture.h"
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include <glm/glm.hpp>



class TitleScreen
{
public:

	void init();
	void update(int deltaTime);
	void render();
	bool getPlay();

private:
	void selectTextures(int selectPlay);
	void menuBehaviour();
	void submenuBehaviour();
	void textBlinkSystem(int deltaTime);

	Texture titleScreenTex, creditsScreen, instructionsScreen, start, credits, instructions;
	TexturedQuad *texQuad[4];
	bool textureBlink;
	int lastBlink, timeBlinking;
	int change, selectionPlay;
};

