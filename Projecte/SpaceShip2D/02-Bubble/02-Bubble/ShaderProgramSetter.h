#include "ShaderProgram.h"
#include <glm/glm.hpp>


class ShaderProgramSetter
{
public:

	// singleton
	static ShaderProgramSetter& getInstance() {
		static ShaderProgramSetter shaderProgramSetter;
		return shaderProgramSetter;
	}

	void init(int screenWidth, int screenHeight);
	void use();

	ShaderProgram& getShaderProgram();

	void setModelView(glm::mat4 modelview);
	void setProjection(float xDisplacement, int screenWidth, int screenHeight, float yDisplacement);
	

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
};

