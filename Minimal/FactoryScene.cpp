#include "FactoryScene.h"
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

FactoryScene::FactoryScene()
{
	factoryModel = new Model("../ModelAssets/factory1/factory1.obj");
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -35.0f));
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	factoryModel->setToWorld(toWorld);

	o2Model = new Model("../ModelAssets/o2/o2.obj");
	cO2Model = new Model("../ModelAssets/co2/co2.obj");

	genSphere = new Sphere(3.0f, true);

	boundCube = new Cube(true);
	boundCube->scaleVal = glm::vec3(20.0f, 20.0f, 20.0f);
	boundCube->isCube = true;
	boundCube->setToWorld(toWorld);
	
	initMolecules();
	initMoleculesPosition();

	numInSceneMolecules = 1;
}
void FactoryScene::spawnNewMolecule()
{
	if (numInSceneMolecules < numMolecules)
	{
		airMolecules[numInSceneMolecules]->inScene = true;
		numInSceneMolecules++;
	}
}
void FactoryScene::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView, float deltaT)
{
	factoryModel->draw(shaderProgram, projection, modelView);
	drawMolecules(shaderProgram, projection, modelView, deltaT);
	boundCube->draw(shaderProgram, projection, modelView);
}

void FactoryScene::initMolecules()
{
	for (int i = 0; i < numMolecules; i++)
	{
		airMolecule * co2Mol = new Co2();

		co2Mol->model = cO2Model;
		co2Mol->sphere = genSphere;
		airMolecules.push_back(co2Mol);		
	}
}
void FactoryScene::drawMolecules(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView, float deltaT)
{
	for (int i = 0; i < numMolecules; i++)
	{
		if(airMolecules[i]->inScene)
		{
			glm::vec3 currPos = airMolecules[i]->position;
			/* Doesnt matter which radius because all the radi are the same */
			float radius = airMolecules[i]->sphere->getRadius();
	
			boundCube->factorSphereCollision(currPos, radius, airMolecules[i]->velocity);
			airMolecules[i]->updateAndDraw(shaderProgram, projection, modelView, deltaT);
		}
		//TODO : consider the breaking optimization
	}
}
void FactoryScene::initMoleculesPosition()
{
	glm::mat4 offset = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	for (int i = 0; i < numMolecules; i++)
	{
		airMolecules[i]->initWorld(toWorld * offset);
	}
	/* For the first molecule in the scene */
	if (numMolecules > 0)
	{
		airMolecules[0]->inScene = true;
	}
}
FactoryScene::~FactoryScene()
{
}
