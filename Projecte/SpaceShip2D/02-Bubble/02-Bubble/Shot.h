#ifndef _SHOT_INCLUDE
#define _SHOT_INCLUDE

#include "Player.h"
#include "TexturedQuad.h"

class Shot
{
public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::fvec2 direction, glm::ivec2 playerPos, glm::ivec2 size,
			  bool bigShot, Texture &texture, float damage, float shotSpeed, bool force, int type);
	void update(int deltaTime, glm::ivec2 playerPos, float cameraX);
	void render();
	glm::ivec2 getSize();
	glm::ivec2 getPos();
	bool getBigShot();
	float getDamage();
	int getShotSpeed();
	int getShotType();

	void setShotSpeed(int speed);


private:

	void textureSetUp(bool bigShot, Texture &texture);
	Texture texture;
	TexturedQuad *texQuad;
	ShaderProgram texProgram;

	glm::ivec2 tileMapDispl;
	TileMap *map;

	glm::ivec2 sizeShot;
	glm::fvec2 shotPos, movDirection;
	bool bigShot, force;
	int shotType;
	float damage, shotSpeed, actualShotSpeed;
};

#endif // _TEXTURE_INCLUDE

