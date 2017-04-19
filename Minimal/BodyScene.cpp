#include "BodyScene.h"
#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
BodyScene::BodyScene()
{
	//TODO generate hand model instead of sphere
	leftHand = new Sphere(0.02f, false);
	rightHand = new Sphere(0.02f, false);

	//TODO fill in laser with cylinder model
	initLasersPosition();
}

void BodyScene::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::vec3 handLPos, glm::vec3 handRPos, float deltaT)
{
	//similar to toWorld matrix in airMolecule file
	toLeft = glm::translate(glm::mat4(1.0f), handLPos);
	toRight = glm::translate(glm::mat4(1.0f), handRPos);

	/* Update the model matrix*/
	leftHand->setToWorld(toLeft);
	rightHand->setToWorld(toRight);

	drawHands(shaderProgram, projection, view);
	drawLasers(shaderProgram, projection, view, deltaT);
}

void BodyScene::drawHands(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view) {
	leftHand->draw(shaderProgram, projection, view);
	rightHand->draw(shaderProgram, projection, view);
}

void BodyScene::drawLasers(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, float deltaT)
{
}
void BodyScene::initLasersPosition()
{
}
BodyScene::~BodyScene()
{
}
