#include "Boss.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

enum Anims
{
	IDLE, DESTROYED
};

void Boss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 pos, glm::ivec2 size) {
	
	hp = 230;
	this->shootCooldown = 1200;
	lastShot = 500;
	shoot = false;
	this->size = glm::ivec2(size.x + 20, size.y + 20);
	this->pos = glm::vec2(pos.x + tileMapDispl.x - 20, pos.y + tileMapDispl.y-10);
	bouncingShotSize = glm::ivec2(25,25);
	seekingShotSize = glm::ivec2(30, 30);
	bouncingShotSpeed = 4.5f;
	seekingShotSpeed = 1.4f;

	tileMapDispl = tileMapPos;

	spritesheet.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(this->size, glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(DESTROYED, 2);
	sprite->addKeyframe(DESTROYED, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(DESTROYED, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(DESTROYED, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DESTROYED, glm::vec2(0.75f, 0.5f));


	sprite->changeAnimation(IDLE);

	sprite->setPosition(this->pos);

}


void Boss::update(int deltaTime)
{

	lastShot += deltaTime;
	if (lastShot >= shootCooldown)
	{
		shoot = true;
	}

	sprite->update(deltaTime);
	sprite->setPosition(pos);
}


void Boss::render()
{
	sprite->render();
}


glm::ivec2 Boss::getBouncingShotSize()
{
	return bouncingShotSize;
}

glm::ivec2 Boss::getSeekingShotSize()
{
	return seekingShotSize;
}

float Boss::getBouncingShotSpeed()
{
	return bouncingShotSpeed;
}

float Boss::getSeekingShotSpeed()
{
	return seekingShotSpeed;
}


bool Boss::getShoot()
{
	if (shoot)
	{
		lastShot = 0;
		shoot = false;
		return true;
	}
	return false;
}

bool Boss::getDropBall()
{
	if (dropBall)
	{
		dropBall = false;
		return true;
	}
	return false;
}

bool Boss::gotHit(float damage)
{
	hp -= damage;
	if (hp >= 160 && hp <= 170)
	{
		dropBall = true;
		hp -= 11;
	}
	else if (hp <= 0)
	{
		sprite->changeAnimation(DESTROYED);
		return true;
	}
	return false;
}

void Boss::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::ivec2 Boss::getSize()
{
	return size;
}

glm::ivec2 Boss::getPos()
{
	return pos;
}