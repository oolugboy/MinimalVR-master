#pragma once
#include "Model.h"
#include "airMolecule.h"
#include "Co2.h"
#include "O2.h"
#include <vector>
#include "Cube.h"

using namespace std;

class FactoryScene
{
public:
	FactoryScene();
	~FactoryScene();
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView, float deltaT);
	void drawMolecules(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView, float deltaT);
	void initMoleculesPosition();
	void initMolecules();
	void spawnNewMolecule();
	vector< airMolecule * > airMolecules; //Temp
	
private:
	Model * factoryModel;
	glm::mat4 toWorld;

	Model * cO2Model;
	Model * o2Model;
	Sphere * genSphere;
	Cube * boundCube;
	const int numMolecules = 1;	
	int numInSceneMolecules;
};
