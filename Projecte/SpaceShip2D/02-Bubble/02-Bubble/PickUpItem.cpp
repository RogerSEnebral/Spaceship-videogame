#include "PickUpItem.h"
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgramSetter.h"


void PickUpItem::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 position, glm::ivec2 size, Texture &texture)
{
	this->position = position;
	this->texture = texture;
	this->size = size;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), size };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, ShaderProgramSetter::getInstance().getShaderProgram());
}

void PickUpItem::render()
{
	glm::mat4 modelview;

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	ShaderProgramSetter::getInstance().setModelView(modelview);
	texQuad->render(texture);
}



glm::ivec2 PickUpItem::getSize()
{
	return size;
}

glm::ivec2 PickUpItem::getPos()
{
	return position;
}