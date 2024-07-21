#include "Force.h"
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgramSetter.h"

#define FORCE_SIZE 8

enum PlayerAnims
{
	LVL1, LVL2, LVL3
};

void Force::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	forceLevel = 0;
	spritesheet.loadFromFile("images/force.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->position = position;
	size = glm::ivec2(0, 0);
	setSprite();

	sprite->changeAnimation(LVL1);
}


void Force::update(int deltaTime, glm::ivec2 playerPos, glm::ivec2 playerSize)
{
	position = glm::ivec2(playerPos.x + playerSize.x - size.x, playerPos.y - size.y - 5);
	sprite->update(deltaTime);
}



void Force::render(glm::ivec2 playerPos, glm::ivec2 playerSize)
{
	sprite->setPosition(position);
	sprite->render();
}



glm::ivec2 Force::getSize()
{
	return size;
}

glm::ivec2 Force::getPos()
{
	return position;
}

int Force::getForceLevel()
{
	return forceLevel;
}

void Force::upgradeForce()
{
	forceLevel++;
	if (forceLevel <= 7) size = glm::ivec2(8 + forceLevel * 2, 8 + forceLevel * 2);

	if (forceLevel == 1)
	{
		setSprite();
		sprite->changeAnimation(LVL1);
	}
	else if (forceLevel == 2)
	{
		setSprite();
		sprite->changeAnimation(LVL2);
	}
	else
	{
		setSprite();
		sprite->changeAnimation(LVL3);
	}
}

void Force::setSprite()
{
	sprite = Sprite::createSprite(size, glm::vec2(0.25f, 0.33333f), &spritesheet, &ShaderProgramSetter::getInstance().getShaderProgram());
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(LVL1, 4);
	sprite->addKeyframe(LVL1, glm::vec2(0.0f, 0.f));
	sprite->addKeyframe(LVL1, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(LVL1, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(LVL1, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(LVL2, 4);
	sprite->addKeyframe(LVL2, glm::vec2(0.0f, 0.33333f));
	sprite->addKeyframe(LVL2, glm::vec2(0.25f, 0.33333f));
	sprite->addKeyframe(LVL2, glm::vec2(0.5f, 0.33333f));
	sprite->addKeyframe(LVL2, glm::vec2(0.75f, 0.33333f));


	sprite->setAnimationSpeed(LVL3, 4);
	sprite->addKeyframe(LVL3, glm::vec2(0.0f, 0.66666f));
	sprite->addKeyframe(LVL3, glm::vec2(0.25f, 0.66666f));
	sprite->addKeyframe(LVL3, glm::vec2(0.5f, 0.66666f));
	sprite->addKeyframe(LVL3, glm::vec2(0.75f, 0.66666f));
}