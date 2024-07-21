#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

enum Anims
{
	IDLE
};

bool Enemy::gotHit(float damage)
{
	hp -= damage;
	return hp <= 0;
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::ivec2 Enemy::getEnemySize()
{
	return enemySize;
}

glm::ivec2 Enemy::getEnemyPos()
{
	return enemyPos;
}

bool Enemy::getShoot()
{
	if (shoot)
	{
		lastShot = 0;
		shoot = false;
		return true;
	}
	return false;
}

int Enemy::getEnemyType()
{
	return enemyType;
}

float Enemy::getShotSpeed()
{
	return shotSpeed;
}

glm::ivec2 Enemy::getShotSize()
{
	return shotSize;
}


void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 enemyPos, int enemyType, bool movement, int shootCooldown, Texture &texture) {
	this->shootCooldown = shootCooldown;
	this->enemyType = enemyType;
	this->movement = movement;
	lastShot = 0;
	shoot = false;
	this->enemyPos = glm::vec2(enemyPos.x + tileMapDispl.x, enemyPos.y + tileMapDispl.y);

	spritesheet = texture;

	if(enemyType == ENEMY_TYPE1) enemy1Setup(shaderProgram);
	else if (enemyType == ENEMY_TYPE2) enemy2Setup(shaderProgram);
	else if (enemyType == ENEMY_TYPE3) enemy3Setup(shaderProgram);
	else enemy4Setup(shaderProgram);

	tileMapDispl = tileMapPos;
	
	sprite->setPosition(this->enemyPos);
}


void Enemy::update(int deltaTime)
{
	if (enemyType != ENEMY_TYPE2)
	{
		lastShot += deltaTime;
		if (lastShot >= shootCooldown)
		{
			shoot = true;
		}
	}

	if (enemyType == ENEMY_TYPE1) enemy1Update(deltaTime);
	else if (enemyType == ENEMY_TYPE2) enemy2Update(deltaTime);
	else if (enemyType == ENEMY_TYPE3) enemy3Update(deltaTime);
	else enemy4Update(deltaTime);

	sprite->update(deltaTime);
	sprite->setPosition(enemyPos);
}


//------------------ ENEMY 1: TRIPLE SHOOTERS --------------------//

void Enemy::enemy1Setup(ShaderProgram &shaderProgram)
{
	//last shot inicializado mayor que cero para cambiar la velocidad del primer ataque cuando el enemigo aparece en pantalla 
	lastShot = shootCooldown - 600;
	hp = 5;
	enemySize = glm::ivec2(30, 30);
	shotSpeed = 3.f;
	shotSize = glm::ivec2(14,14);
	moveSpeed = 0.8f;

	sprite = Sprite::createSprite(enemySize, glm::vec2(0.166667f, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 6);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.1666f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.3333f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.6666f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.8333f, 0.f));

	sprite->changeAnimation(0);
}

void Enemy::enemy1Update(int deltaTime)
{
	if (movement)
	{
		//se mueve en una direccion aleatoria cada vez que dispara
		if (shoot)
		{
			moveDirection = glm::normalize(glm::fvec2(rand() % 51, rand() % 151 - 75));
		}
		//siempre se mueve hacia atras
		glm::fvec2 nextPos = glm::vec2(enemyPos.x + moveDirection.x * moveSpeed, enemyPos.y);
		if (!map->collisionMoveRight(nextPos, enemySize)) enemyPos = nextPos;

		//si no puede moverse en un sentido vertical, prueba con el otro
		nextPos = glm::vec2(enemyPos.x, enemyPos.y + moveDirection.y * moveSpeed);
		if (!map->collisionMoveUp(nextPos, enemySize) && !map->collisionMoveDown(nextPos, enemySize)) enemyPos = nextPos;
		else
		{
			nextPos = glm::vec2(enemyPos.x, enemyPos.y - moveDirection.y * moveSpeed);
			if (!map->collisionMoveUp(nextPos, enemySize) && !map->collisionMoveDown(nextPos, enemySize)) enemyPos = nextPos;
		}
	}
}



//------------------ ENEMY 2: WAVERS --------------------//

void Enemy::enemy2Setup(ShaderProgram &shaderProgram)
{
	lastShot = -shootCooldown/2;
	hp = 2;
	enemySize = glm::ivec2(20, 20);
	shotSize = glm::ivec2(15, 15);
	moveSpeed = 1.5f;
	moveDirection = glm::fvec2(0, -1);

	sprite = Sprite::createSprite(enemySize, glm::vec2(0.166667f, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 6);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.1666f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.3333f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.6666f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.8333f, 0.f));

	sprite->changeAnimation(0);

}

void Enemy::enemy2Update(int deltaTime)
{
	if (movement)
	{
		//movimiento x siempre izquierda
		glm::fvec2 nextPos = glm::vec2(enemyPos.x - moveSpeed, enemyPos.y);
		if (!map->collisionMoveLeft(nextPos, enemySize))  enemyPos = nextPos;

		// movimiento y en olas
		float wave = sin((-lastShot+shootCooldown/2.f) / - shootCooldown / 2.f);
		nextPos = glm::vec2(enemyPos.x, enemyPos.y +  wave * 2.5f*moveSpeed);
		if (!map->collisionMoveUp(nextPos, enemySize) && !map->collisionMoveDown(nextPos, enemySize)) enemyPos = nextPos;

		if (lastShot > shootCooldown)
			moveDirection.y = -1;
		else if (lastShot < 0)
		{
			moveDirection.y = 1;
		}

		if(moveDirection.y > 0) lastShot += deltaTime;
		else lastShot -= deltaTime;
	}
}

//------------------ ENEMY 3: WALL CRAWLERS --------------------//

void Enemy::enemy3Setup(ShaderProgram &shaderProgram)
{
	lastShot = shootCooldown - 600;
	hp = 2.1;
	shotSpeed = 4.f;
	enemySize = glm::ivec2(35, 35);
	shotSize = glm::ivec2(15, 15);
	moveSpeed = 0.5f;
	moveDirection = glm::fvec2(1, 0);

	sprite = Sprite::createSprite(enemySize, glm::vec2(0.33333f, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 3);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.3333f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.6666f, 0.f));

	sprite->changeAnimation(0);
}


void Enemy::enemy3Update(int deltaTime)
{

}

//------------------ ENEMY 4: SHIELDS --------------------//

void Enemy::enemy4Setup(ShaderProgram &shaderProgram)
{
	hp = 11;
	lastShot = shootCooldown - 600;
	enemySize = glm::ivec2(27, 55);
	shotSize = glm::ivec2(15, 0);
	moveSpeed = 1.3f;
	moveDirection = glm::fvec2(0, 1);

	sprite = Sprite::createSprite(enemySize, glm::vec2(0.25f, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 6);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(0);
}



void Enemy::enemy4Update(int deltaTime)
{
	if (movement)
	{
		glm::fvec2 nextPos = glm::vec2(enemyPos.x, enemyPos.y + moveDirection.y * moveSpeed);
		if (map->collisionMoveUp(nextPos, enemySize) || map->collisionMoveDown(nextPos, enemySize))
			moveDirection.y = -moveDirection.y;

		enemyPos = nextPos;
	}
}