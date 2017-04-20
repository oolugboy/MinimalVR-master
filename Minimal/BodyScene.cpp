#include "BodyScene.h"
#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
BodyScene::BodyScene()
{
	//TODO generate hand model instead of sphere
	leftHand = new Sphere(0.02f, false);
	rightHand = new Sphere(0.02f, false);

	leftLaser = new Line(glm::vec3(0,0,0));
	rightLaser = new Line(glm::vec3(0,0,0));
	//TODO fill in laser with cylinder model
	initLasersPosition();
}

void BodyScene::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::vec3 handLPos, glm::vec3 handRPos, 
					glm::mat4 rotationL, glm::mat4 rotationR, float deltaT)
{
	//similar to toWorld matrix in airMolecule file
	toLeft = glm::translate(glm::mat4(1.0f), handLPos);
	toRight = glm::translate(glm::mat4(1.0f), handRPos);

	glm::mat4 toLLaser = toLeft * rotationL;
	glm::mat4 toRLaser = toRight * rotationR;

	/* Update the model matrix*/
	leftHand->setToWorld(toLeft);
	rightHand->setToWorld(toRight);

	drawHands(shaderProgram, projection, view);

	//leftLaser->setToWorld(toLLaser);
	//rightLaser->setToWorld(toRLaser);

	leftLaser->setToWorld(toLeft);
	rightLaser->setToWorld(toRight);

	drawLasers(shaderProgram, projection, view);

}

void BodyScene::drawHands(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view) {
	leftHand->draw(shaderProgram, projection, view);
	rightHand->draw(shaderProgram, projection, view);
}

void BodyScene::drawLasers(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view)
{
	leftLaser->draw(shaderProgram, projection, view);
	rightLaser->draw(shaderProgram, projection, view);
}
void BodyScene::initLasersPosition()
{
}
BodyScene::~BodyScene()
{
}
