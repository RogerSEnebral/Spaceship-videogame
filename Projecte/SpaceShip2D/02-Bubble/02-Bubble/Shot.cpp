#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Player.h"
#include "Shot.h"
#include "Game.h"

#define STRAIGHT 0
#define BOUNCING 1
#define SEEKING 2

void Shot::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, 
			glm::fvec2 direction, glm::ivec2 position, glm::ivec2 size, bool bigShot, Texture &texture, float damage, float shotSpeed, bool force, int type) {
	sizeShot = size;
	this->bigShot = bigShot;
	this->force = force;
	shotType = type;
	
	//bigShot hace menos daño ya que atraviesa y golpea en cada punto en el que esta en contacto. Cuanto mas largo sea el enemigo mas daño hara bigShot
	this->damage = damage;

	this-> shotSpeed = shotSpeed;
	actualShotSpeed = shotSpeed;
	if (type == SEEKING) actualShotSpeed = 2.5 * shotSpeed;

	this->movDirection = direction;
	tileMapDispl = tileMapPos;

	this->texture = texture;
	texProgram = shaderProgram;
	textureSetUp(bigShot, texture);

	shotPos = glm::vec2(position.x + tileMapDispl.x, position.y + tileMapDispl.y - sizeShot.y / 2);
}


void Shot::update(int deltaTime, glm::ivec2 playerPos, float cameraX)
{
	if (shotType == SEEKING)
	{
		movDirection = glm::normalize(glm::fvec2(playerPos.x + 5 - shotPos.x, playerPos.y+5 - shotPos.y));
		if (actualShotSpeed >= shotSpeed) actualShotSpeed -= 0.05;
	}
	else if(shotType == BOUNCING)
	{
		if (shotPos.x <= cameraX + 7)
		{
			movDirection.x = -movDirection.x;
		}
		if (shotPos.y <= 5 || shotPos.y >= SCREEN_HEIGHT - 7) movDirection.y = -movDirection.y;
	}

	shotPos.x += movDirection.x * shotSpeed;
	shotPos.y += movDirection.y * shotSpeed;
}


void Shot::render()
{
	glm::mat4 modelview;
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(shotPos.x, shotPos.y, 0.f));
	modelview = glm::rotate(modelview, movDirection.y, glm::vec3(0.0f, 0.0f, 1.0f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad->render(texture);
}


glm::ivec2 Shot::getSize()
{
	return sizeShot;
}

glm::ivec2 Shot::getPos()
{
	return shotPos;
}

float Shot::getDamage()
{
	return damage;
}

bool Shot::getBigShot()
{
	return bigShot;
}

int Shot::getShotSpeed()
{
	return shotSpeed;
}

int Shot::getShotType()
{
	return shotType;
}


void Shot::setShotSpeed(int speed)
{
	actualShotSpeed = speed;
}

void Shot::textureSetUp(bool bigShot, Texture &text)
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), sizeShot };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
}