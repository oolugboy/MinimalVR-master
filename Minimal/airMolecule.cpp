#include "airMolecule.h"
#include <stdlib.h>
#include <ctime>
#include <iostream>

int airMolecule::seed = 0;
airMolecule::airMolecule()
{
	srand(0);	
}
void airMolecule::updateAndDraw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView, float deltaT)
{
	glm::vec3 posDiff = deltaT * velocity;
	toWorld = glm::translate(glm::mat4(1.0f), posDiff) * toWorld;

	/* Update the models toWorld */
	this->model->setToWorld(toWorld);
	this->sphere->setToWorld(toWorld);

	this->model->draw(shaderProgram, projection, modelView);
	this->sphere->draw(shaderProgram, projection, modelView);
}
void airMolecule::initWorld(glm::mat4 toWorld)
{
	/* Initialize the position */
	this->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)) * toWorld;

	/* Initialize the velocity */
	//srand(seed+=5);
	this->velocity.y = 1.0f;
	this->velocity.x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)/4.0f)) - 2.0f;
	this->velocity.z = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)/4.0f)) - 2.0f;
}

airMolecule::~airMolecule()
{

}
