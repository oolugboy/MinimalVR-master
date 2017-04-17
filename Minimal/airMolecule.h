#pragma once
#include "Model.h"
class airMolecule
{
public:
	airMolecule();
	~airMolecule();	
	bool isCo2();
	bool inScene();
	void update();
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
private:
	glm::vec3 position;
	glm::vec3 velocity;	
	bool co2Bool, sceneBool;
	glm::mat4 toWorld;
	Model * model;
};

