#include "TitleScreen.h"
#include "Game.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glut.h>

#define PLAY 0
#define MENU 1
#define INSTRUCTIONS 2
#define CREDITS 3



void TitleScreen::init()
{
	titleScreenTex.loadFromFile("images/titleScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructionsScreen.loadFromFile("images/instructionsScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsScreen.loadFromFile("images/creditsScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	start.loadFromFile("images/start.png", TEXTURE_PIXEL_FORMAT_RGBA);
	credits.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);

	//full screen
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, ShaderProgramSetter::getInstance().getShaderProgram());

	textureBlink = false;
	selectionPlay = PLAY;
	selectTextures(PLAY);
	change = MENU;
	// eliminamos el buffer de teclas, ya que si no se hace esto se almacena la ultima vez que pulsaste
	// enter y le da a play instantaniamente al volver a la pantalla de titulo
	Game::instance().getKeyPress(13);
	Game::instance().getKeyPress('w');
	Game::instance().getKeyPress('s');
	Game::instance().getSpecialKeyPress(GLUT_KEY_UP);
	Game::instance().getSpecialKeyPress(GLUT_KEY_DOWN);
}

void TitleScreen::update(int deltaTime)
{
	if (change == MENU) menuBehaviour();
	
	else if (change == CREDITS | change == INSTRUCTIONS)
	{
		submenuBehaviour();
	}
	
	textBlinkSystem(deltaTime);
}

void TitleScreen::menuBehaviour()
{
	if (change == MENU)
		if (Game::instance().getSpecialKeyPress(GLUT_KEY_UP) || Game::instance().getKeyPress('w'))
		{
			if (selectionPlay == CREDITS)
			{
				textureBlink = false;
				timeBlinking = 0;
				lastBlink = 0;
				selectionPlay = INSTRUCTIONS;
				selectTextures(INSTRUCTIONS);

				irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/switchOption.mp3", false, false, true);
				sound->setVolume(2.f);
			}

			else if (selectionPlay == INSTRUCTIONS)
			{
				textureBlink = false;
				timeBlinking = 0;
				lastBlink = 0;
				selectionPlay = PLAY;
				selectTextures(PLAY);

				irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/switchOption.mp3", false, false, true);
				sound->setVolume(2.f);
			}
		}

	if (Game::instance().getSpecialKeyPress(GLUT_KEY_DOWN) || Game::instance().getKeyPress('s'))
	{
		if (selectionPlay == PLAY)
		{
			textureBlink = false;
			timeBlinking = 0;
			lastBlink = 0;
			selectionPlay = INSTRUCTIONS;
			selectTextures(INSTRUCTIONS);

			irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/switchOption.mp3", false, false, true);
			sound->setVolume(2.f);
		}

		else if (selectionPlay == INSTRUCTIONS)
		{
			textureBlink = false;
			timeBlinking = 0;
			lastBlink = 0;
			selectionPlay = CREDITS;
			selectTextures(CREDITS);

			irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/switchOption.mp3", false, false, true);
			sound->setVolume(2.f);
		}
	}
	//enter
	if (Game::instance().getKeyPress(13))
	{
		if (selectionPlay == PLAY) change = 0;
		else if (selectionPlay == CREDITS) change = CREDITS;
		else change = INSTRUCTIONS;
		irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/selectOption.mp3", false, false, true);
		sound->setVolume(0.8f);
	}
}

void TitleScreen::submenuBehaviour()
{
	if (Game::instance().getKeyPress(13))
	{
		change = MENU;
		irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/selectOption.mp3", false, false, true);
		sound->setVolume(0.8f);
	}
}

void TitleScreen::textBlinkSystem(int deltaTime)
{
	if (!textureBlink) lastBlink += deltaTime;
	if (lastBlink >= 1000)
	{
		textureBlink = true;
		lastBlink = 0;
	}
	if (textureBlink) timeBlinking += deltaTime;
	if (timeBlinking >= 500)
	{
		textureBlink = false;
		timeBlinking = 0;
	}
}




void TitleScreen::render()
{
	if (change == MENU)
	{
		ShaderProgramSetter::getInstance().use();
		texQuad[0]->render(titleScreenTex);
		glm::mat4 modelview;

		//posicion de las texturas en pantalla
		glm::vec3 posTextPlay = glm::vec3(430, 70, 0.f);
		glm::vec3 posTextInstructions = glm::vec3(450, 110, 0.f);
		glm::vec3 posTextCredits = glm::vec3(470, 150, 0.f);

		//depende de la textura que este seleccionada blinkea o no
		if (selectionPlay == PLAY)
		{

			if (!textureBlink)
			{
				modelview = glm::translate(glm::mat4(1.0f), posTextPlay);
				ShaderProgramSetter::getInstance().setModelView(modelview);
				texQuad[1]->render(start);
			}

			modelview = glm::translate(glm::mat4(1.0f), posTextInstructions);
			ShaderProgramSetter::getInstance().setModelView(modelview);
			texQuad[2]->render(instructions);

			modelview = glm::translate(glm::mat4(1.0f), posTextCredits);
			ShaderProgramSetter::getInstance().setModelView(modelview);
			texQuad[3]->render(credits);
		}

		else if (selectionPlay == CREDITS)
		{
			modelview = glm::translate(glm::mat4(1.0f), posTextPlay);
			ShaderProgramSetter::getInstance().setModelView(modelview);
			texQuad[1]->render(start);

			modelview = glm::translate(glm::mat4(1.0f), posTextInstructions);
			ShaderProgramSetter::getInstance().setModelView(modelview);
			texQuad[2]->render(instructions);

			if (!textureBlink)
			{
				modelview = glm::translate(glm::mat4(1.0f), posTextCredits);
				ShaderProgramSetter::getInstance().setModelView(modelview);
				texQuad[3]->render(credits);
			}
		}

		else
		{
			modelview = glm::translate(glm::mat4(1.0f), posTextPlay);
			ShaderProgramSetter::getInstance().setModelView(modelview);
			texQuad[1]->render(start);

			if (!textureBlink)
			{
				modelview = glm::translate(glm::mat4(1.0f), posTextInstructions);
				ShaderProgramSetter::getInstance().setModelView(modelview);
				texQuad[2]->render(instructions);
			}

			modelview = glm::translate(glm::mat4(1.0f), posTextCredits);
			ShaderProgramSetter::getInstance().setModelView(modelview);
			texQuad[3]->render(credits);
		}
	}

	else if (change == INSTRUCTIONS)
	{
		ShaderProgramSetter::getInstance().use();
		texQuad[0]->render(instructionsScreen);
	}

	else if (change == CREDITS)
	{
		ShaderProgramSetter::getInstance().use();
		texQuad[0]->render(creditsScreen);
	}
}


void TitleScreen::selectTextures(int selectedOption)
{
	//start
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f),  geom[1] = glm::ivec2(190, 30) };
	glm::vec2 texCoords[2];
	if (selectedOption == PLAY)
	{
		texCoords[0] = glm::vec2(0.f, 0.f);
		texCoords[1] = glm::vec2(1.f, 0.5f);
	}
	else {
		texCoords[0] = glm::vec2(0.f, 0.5f);
		texCoords[1] = glm::vec2(1.f, 1.f);
	}

	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, ShaderProgramSetter::getInstance().getShaderProgram());

	//instructions
	geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::ivec2(160, 30);
	if (selectedOption == INSTRUCTIONS)
	{
		texCoords[0] = glm::vec2(0.f, 0.f);
		texCoords[1] = glm::vec2(1.f, 0.5f);

	}
	else {
		texCoords[0] = glm::vec2(0.f, 0.5f);
		texCoords[1] = glm::vec2(1.f, 1.f);
	}

	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, ShaderProgramSetter::getInstance().getShaderProgram());

	//credits
	geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::ivec2(130, 30);
	if (selectedOption == CREDITS)
	{
		texCoords[0] = glm::vec2(0.f, 0.f);
		texCoords[1] = glm::vec2(1.f, 0.5f);
		
	}
	else {
		texCoords[0] = glm::vec2(0.f, 0.5f);
		texCoords[1] = glm::vec2(1.f, 1.f);
	}

	texQuad[3] = TexturedQuad::createTexturedQuad(geom, texCoords, ShaderProgramSetter::getInstance().getShaderProgram());
}

bool TitleScreen::getPlay()
{
	return change == 0;
}