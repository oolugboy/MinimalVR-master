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

	this->position = glm::vec3(toWorld[3]);

	/* Update the models toWorld */
	this->model->setToWorld(toWorld);
	this->sphere->setToWorld(toWorld);

	this->model->spin(this->deg++, this->spinAxis);
	this->model->draw(shaderProgram, projection, modelView);
	this->sphere->draw(shaderProgram, projection, modelView);
}
void airMolecule::initWorld(glm::mat4 toWorld)
{
	/* Initialize the position */
	this->toWorld =  toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	
	/* Initialize the model's toWorld */
	this->model->setToWorld(this->toWorld);
	this->sphere->setToWorld(this->toWorld);

	/* Initialize the velocity */
	this->velocity.y = 1.0f;
	this->velocity.x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)/4.0f)) - 2.0f;
	this->velocity.z = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)/4.0f)) - 2.0f;

	int spinVal = (static_cast <int> (rand()) / (static_cast <int> (RAND_MAX) / 3.0f));

	if (spinVal >= 0 && spinVal < 1)
		spinAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	else if (spinVal >= 1 && spinVal < 2)
		spinAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	else
		spinAxis = glm::vec3(0.0f, 0.0f, 1.0f);
}

airMolecule::~airMolecule()
{

}
