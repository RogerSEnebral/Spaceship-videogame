#include "Texture.h"
#include "TexturedQuad.h"
#include <glm/glm.hpp>

class PickUpItem
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 position, glm::ivec2 size, Texture &texture);
	void render();

	glm::ivec2 getPos();
	glm::ivec2 getSize();

private:
	glm::ivec2 position, size;
	Texture texture;
	TexturedQuad *texQuad;
};

