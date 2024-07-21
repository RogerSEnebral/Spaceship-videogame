#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define ANIMATION_TIME 150

enum PlayerAnims
{
	IDLE, START_MOVE_UP, MOVE_UP, START_MOVE_DOWN, MOVE_DOWN
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	sizePlayer = glm::ivec2(35, 20);
	moveSpeed = 3.f;
	crushed = false;
	startedMovingDown = false;
	startedMovingUp = false;
	spritesheet.loadFromFile("images/navesita.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.2, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(IDLE, 0);
	sprite->addKeyframe(IDLE, glm::vec2(0.4f, 0.f));

	sprite->setAnimationSpeed(START_MOVE_UP, 0);
	sprite->addKeyframe(START_MOVE_UP, glm::vec2(0.6f, 0.f));
	
	sprite->setAnimationSpeed(MOVE_UP, 0);
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.8f, 0.f));

	sprite->setAnimationSpeed(START_MOVE_DOWN, 0);
	sprite->addKeyframe(START_MOVE_DOWN, glm::vec2(0.2f, 0.f));

	sprite->setAnimationSpeed(MOVE_DOWN, 0);
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(IDLE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime, float camSpeed, int camPosX)
{
	bool moving = false;
	bool movingVertical = false;
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT) | Game::instance().getKey('a'))
	{
		if (posPlayer.x + tileMapDispl.x > camPosX)
		{
			//sumamos un poco mas que la velocidad de movimiento debido al avance de la camara (no del todo)
			posPlayer.x -= moveSpeed + 0.2f;
			if (map->collisionMoveLeft(posPlayer, sizePlayer))
			{
				posPlayer.x += moveSpeed + 0.5f;
			}
			sprite->changeAnimation(IDLE);
			moving = true;
		}
	}

	if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT) | Game::instance().getKey('d'))
	{
		if (posPlayer.x + tileMapDispl.x + sizePlayer.x < camPosX + SCREEN_WIDTH)
		{
			posPlayer.x += moveSpeed;
			if (map->collisionMoveRight(posPlayer, sizePlayer))
			{
				posPlayer.x -= moveSpeed;
			}
			sprite->changeAnimation(IDLE);
			moving = true;
		}
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_UP) | Game::instance().getKey('w'))
	{
		startedMovingDown = 0;
		startedMovingUp += deltaTime;
		if (startedMovingUp < ANIMATION_TIME)
		{
			sprite->changeAnimation(START_MOVE_UP);
		}
		else sprite->changeAnimation(MOVE_UP);
		posPlayer.y -= moveSpeed;
		if (map->collisionMoveUp(posPlayer, sizePlayer))
		{
			posPlayer.y += moveSpeed;
		}
		moving = true;
		movingVertical = true;
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) | Game::instance().getKey('s'))
	{
		startedMovingUp = 0;
		startedMovingDown += deltaTime;
		if (startedMovingDown < ANIMATION_TIME)
		{
			sprite->changeAnimation(START_MOVE_DOWN);
		}
		else sprite->changeAnimation(MOVE_DOWN);
		posPlayer.y += moveSpeed;
		if (map->collisionMoveDown(posPlayer, sizePlayer))
		{
			posPlayer.y -= moveSpeed;
		}
		moving = true;
		movingVertical = true;
	}
	if (!map->collisionMoveRight(glm::ivec2(posPlayer.x + camSpeed, posPlayer.y), sizePlayer)) posPlayer.x += camSpeed;
	else
	{
		if (posPlayer.x + 2 * sizePlayer.x / 3 < camPosX)
		{
			crushed = true;
		}
	}
	if (!moving)
	{
		sprite->changeAnimation(IDLE);
		if (!movingVertical)
		{
			startedMovingDown = 0;
			startedMovingUp = 0;
		}
	}
		
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

glm::ivec2 Player::getSize()
{
	return sizePlayer;
}

bool Player::getCrushed()
{
	return crushed;
}