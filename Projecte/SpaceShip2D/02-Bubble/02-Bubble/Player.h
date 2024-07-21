#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, float camSpeed, int camPos);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	glm::ivec2 getSize();
	bool getCrushed();
	
private:
	float moveSpeed;
	glm::ivec2 tileMapDispl, sizePlayer;
	glm::fvec2 posPlayer;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool crushed;
	int startedMovingDown, startedMovingUp;

};


#endif // _PLAYER_INCLUDE


