#include "Texture.h"
#include "TexturedQuad.h"
#include <glm/glm.hpp>
#include "Sprite.h"

class Force
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, glm::ivec2 playerPos, glm::ivec2 playerSize);
	void render(glm::ivec2 playerPos, glm::ivec2 playerSize);
	void upgradeForce();

	glm::ivec2 getPos();
	glm::ivec2 getSize();
	int getForceLevel();

private:
	void setSprite();

	glm::ivec2 position, size;
	Texture spritesheet;
	Sprite *sprite;
	TexturedQuad *texQuad;
	int forceLevel;
};

