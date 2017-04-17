#pragma once
#include "Model.h"
#include "airMolecule.h"
#include <vector>

using namespace std;

class Factory
{
public:
	Factory();
	~Factory();
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	
private:
	Model * factModel;
	glm::mat4 toWorld;
	vector< airMolecule * > airMolecules;
	Model * cO2Model;
	Model * O2Model;
	
};

