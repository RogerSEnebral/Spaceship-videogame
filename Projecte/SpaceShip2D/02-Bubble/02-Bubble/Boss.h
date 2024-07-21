#include "Sprite.h"
#include "TileMap.h"

class Boss
{
public:

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;


	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 pos, glm::ivec2 size);
	void update(int deltaTime);
	void render();
	bool gotHit(float damage);
	void setTileMap(TileMap *tileMap);
	glm::ivec2 getSize();
	glm::ivec2 getPos();
	bool getShoot();
	float getBouncingShotSpeed();
	float getSeekingShotSpeed();
	glm::ivec2 getBouncingShotSize();
	glm::ivec2 getSeekingShotSize();
	bool getDropBall();


private:
	glm::ivec2 tileMapDispl, size, bouncingShotSize, seekingShotSize;
	glm::fvec2 pos;
	float hp, bouncingShotSpeed, seekingShotSpeed;
	int shootCooldown, lastShot;
	bool shoot, dropBall;
};

