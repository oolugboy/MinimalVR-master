#include "airMolecule.h"
#include <stdlib.h>
#include <ctime>
#include <iostream>

int airMolecule::seed = 0;
airMolecule::airMolecule()
{
	srand(0);	
}
void airMolecule::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	/* "Bind" the models to this molecule */
	this->model->setToWorld(toWorld);
	this->sphere->setToWorld(toWorld);
	/* Spin the model */
	this->model->spin(this->deg++, this->spinAxis);

	/* Then draw them */
	this->model->draw(shaderProgram, projection, modelView);
	this->sphere->draw(shaderProgram, projection, modelView);
}
void airMolecule::printVector(glm::vec3 val)
{
	cout << val.x << " " << val.y << " " << val.z << endl;
}
void airMolecule::randPositon()
{
	glm::vec3 newPosition;
	/* Get the random position */
	newPosition.x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / 18.0f)) - 9.0f;
	newPosition.y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / 18.0f)) - 9.0f;
	newPosition.z = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / 18.0f)) - 9.0f;

	glm::vec3 posDiff = newPosition - position;
	position = newPosition;
	this->toWorld = glm::translate(glm::mat4(1.0f), posDiff) * toWorld;
}
void airMolecule::update(float deltaT)
{
	/*cout << " The deltaT is " << deltaT << endl;
	cout << " The model velocity is ";
	printVector(velocity);*/
	glm::vec3 posDiff = deltaT * velocity;
	/*cout << " The posDiff is ";
	printVector(posDiff);*/
	toWorld = glm::translate(glm::mat4(1.0f), posDiff) * toWorld;
	this->position = glm::vec3(toWorld[3]);
	/*cout << " The model position is ";
	printVector(this->position);*/
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
