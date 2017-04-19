#pragma once
#include "Model.h"
#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>

class airMolecule
{
public:
	airMolecule();
	~airMolecule();	
	bool isCo2;
	bool inScene;
	void initWorld(glm::mat4 toWorld);
	void updateAndDraw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView, float deltaT);
	glm::vec3 position;
	glm::vec3 velocity;
	glm::mat4 toWorld;
	static int seed;
	Model * model;
	Sphere * sphere;
private:
	glm::vec3 spinAxis;
	float deg;
};

