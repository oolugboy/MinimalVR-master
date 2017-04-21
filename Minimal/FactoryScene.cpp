#include "FactoryScene.h"
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

FactoryScene::FactoryScene()
{
	factoryModel = new Model("../ModelAssets/factory1/factory1.obj");
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -35.0f));
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	testLine = new Line(glm::vec3(0.0f, 0.0f, 0.0f));
	testLine->setToWorld(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -35.0f)));

	factoryModel->setToWorld(toWorld);
	

	o2Model = new Model("../ModelAssets/o2/o2.obj");
	cO2Model = new Model("../ModelAssets/co2/co2.obj");

	genSphere = new Sphere(1.0f, true);

	boundCube = new Cube(true);
	boundCube->scaleVal = glm::vec3(20.0f, 20.0f, 20.0f);
	boundCube->isCube = true;
	boundCube->setToWorld(toWorld);
	
	initMolecules();
	initMoleculesPosition();

	gameRunning = true;
	gameLost = false;
	renderedEndScene = false;
}
bool FactoryScene::checkSlice(glm::vec3 lBegin, glm::vec3 lEnd, glm::vec3 rBegin, glm::vec3 rEnd)
{
	if(!gameRunning)
	{
		return false;
	}
	bool sliced = false;
	for (int i = 0; i < numMolecules; i++)
	{
		if (airMolecules[i]->inScene && airMolecules[i]->isCo2)
		{
			float radius = airMolecules[i]->sphere->getRadius();
			glm::vec3 currPos = airMolecules[i]->position;
			glm::vec3 num = glm::cross(currPos - lBegin, currPos - lEnd);
			glm::vec3 denum = lEnd - lBegin;

			float lDist = glm::distance(glm::vec3(0), num)/glm::distance(glm::vec3(0), denum);

			num = glm::cross(currPos - rBegin, currPos - rEnd);
			denum = rEnd - rBegin;

			float rDist = glm::distance(glm::vec3(0), num) / glm::distance(glm::vec3(0), denum);

			if (lDist < radius && rDist < radius)
			{
				airMolecules[i]->model = o2Model;
				airMolecules[i]->isCo2 = false;
				numCo2Molecules--;
				sliced = true;
			}
		}
	}	
	return sliced;
}
void FactoryScene::update(float deltaT)
{
	if (!renderedEndScene)
	{
		updateMolecules(deltaT);
		if (numCo2Molecules >= 10)
		{
			gameRunning = false;
			gameLost = true;
		}
		if (numCo2Molecules == 0)
		{
			gameRunning = false;
			gameLost = false;
		}
	}
}
void FactoryScene::spawnNewMolecule()
{
	if (gameRunning)
	{
		if (numInSceneMolecules < numMolecules)
		{
			airMolecules[numInSceneMolecules]->inScene = true;
			numInSceneMolecules++;
			numCo2Molecules++;
		}
	}
}
void FactoryScene::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	factoryModel->draw(shaderProgram, projection, modelView);
	drawMolecules(shaderProgram, projection, modelView);
//	boundCube->draw(shaderProgram, projection, modelView);
	//testLine->draw(shaderProgram, projection, modelView);
}

void FactoryScene::initMolecules()
{
	if (airMolecules.size() > 0)
	{
		for (int i = 0; i < numMolecules; i++)
		{
			delete(airMolecules[i]);
		}
	}

	airMolecules.clear();

	for (int i = 0; i < numMolecules; i++)
	{
		airMolecule * co2Mol = new Co2();

		co2Mol->isCo2 = true;
		co2Mol->inScene = false;
		co2Mol->model = cO2Model;
		co2Mol->sphere = genSphere;
		airMolecules.push_back(co2Mol);		
	}

	/* For the first molecule in the scene */
	if (numMolecules > 0)
	{
		airMolecules[0]->inScene = true;
		numInSceneMolecules = 1;
		numCo2Molecules = numInSceneMolecules;
	}
}
void FactoryScene::restartScene()
{
	if (gameRunning == false)
	{
		glClearColor(0.0f, 0.0f, 0.545f, 1.0f);
		initMolecules();
		initMoleculesPosition();
		gameRunning = true;
		gameLost = false;
		renderedEndScene = false;
	}
}
void FactoryScene::updateMolecules(float deltaT)
{
	for (int i = 0; i < numMolecules; i++)
	{
		if (gameRunning)
		{
			if (airMolecules[i]->inScene)
			{
				glm::vec3 currPos = airMolecules[i]->position;
				/* Doesnt matter which radius because all the radi are the same */
				float radius = airMolecules[i]->sphere->getRadius();

				boundCube->factorSphereCollision(currPos, radius, airMolecules[i]->velocity);
				airMolecules[i]->update(deltaT);
			}
		}
		else
		{
			if (gameLost)
			{
				airMolecules[i]->inScene = true;
				airMolecules[i]->randPositon();
			}
			else
			{
				airMolecules[i]->inScene = false;
				if (renderedEndScene == false)
				{
					glClearColor(0.678f, 0.847f, 0.902f, 1.0f);
				}
			}
			/* Premature boolean set does not hurt */
			renderedEndScene = true;
		}
	}
}
void FactoryScene::drawMolecules(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	for (int i = 0; i < numMolecules; i++)
	{
		if(airMolecules[i]->inScene)
		{
			airMolecules[i]->draw(shaderProgram, projection, modelView);
		}
	}
}

void FactoryScene::initMoleculesPosition()
{
	glm::mat4 offset = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	for (int i = 0; i < numMolecules; i++)
	{
		airMolecules[i]->initWorld(toWorld * offset);
	}
}
FactoryScene::~FactoryScene()
{
}
