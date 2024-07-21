#ifndef _SHADER_PROGRAM_SETTER_INCLUDE
#define _SHADER_PROGRAM_SETTER_INCLUDE

#include <iostream>
#include "ShaderProgramSetter.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>


void ShaderProgramSetter::init(int screenWidth, int screenHeight)
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	projection = glm::ortho(0.f, float(screenWidth), float(screenHeight), 0.f);
}


ShaderProgram& ShaderProgramSetter::getShaderProgram() {
	return texProgram;
}

void ShaderProgramSetter::setModelView(glm::mat4 modelview)
{
	texProgram.setUniformMatrix4f("modelview", modelview);
}

void ShaderProgramSetter::setProjection(float xDisplacement, int screenWidth, int screenHeight, float yDisplacement)
{
	projection = glm::ortho(xDisplacement, float(screenWidth), float(screenHeight), yDisplacement);
	texProgram.setUniformMatrix4f("projection", projection);
}

void ShaderProgramSetter::use()
{
	glm::mat4 modelview = glm::mat4(1.0f);;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
}



#endif //_SHADER_PROGRAM_SETTER_INCLUDE