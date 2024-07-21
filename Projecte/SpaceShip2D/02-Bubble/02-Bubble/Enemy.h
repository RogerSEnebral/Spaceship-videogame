#pragma once
#include "Sprite.h"
#include "TileMap.h"

#define ENEMY_TYPE1 1
#define ENEMY_TYPE2 2
#define ENEMY_TYPE3 3
#define ENEMY_TYPE4 4

class Enemy
{
public:

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;


	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 enemyPos, int enemyType, bool movement, int shootCooldown, Texture &texture);
	void update(int deltaTime);
	void render();
	bool gotHit(float damage);	
	void setTileMap(TileMap *tileMap);
	glm::ivec2 getEnemySize();
	glm::ivec2 getEnemyPos();
	bool getShoot();
	int getEnemyType();
	float getShotSpeed();
	glm::ivec2 getShotSize();


private:
	glm::ivec2 tileMapDispl, enemySize, shotSize;
	glm::fvec2 moveDirection, enemyPos;
	float hp, shotSpeed, moveSpeed;
	int shootCooldown, lastShot, enemyType;
	bool shoot, movement;
	

	void enemy1Setup(ShaderProgram &shaderProgram);
	void enemy2Setup(ShaderProgram &shaderProgram);
	void enemy3Setup(ShaderProgram &shaderProgram);
	void enemy4Setup(ShaderProgram &shaderProgram);

	void enemy1Update(int deltaTime);
	void enemy2Update(int deltaTime);
	void enemy3Update(int deltaTime);
	void enemy4Update(int deltaTime);

};

