#pragma once
#include <vector>
#include "Sphere.h"
#include <vector>
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
#include "Line.h"
using namespace std;

class BodyScene
{
public:
	BodyScene();
	~BodyScene();
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::vec3 handLPos, glm::vec3 handRPos, 
			  glm::mat4 rotationL, glm::mat4 rotationR, float deltaT);
	void drawHands(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view);
	void drawLasers(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view);
	void initLasersPosition();
	//vector< airMolecule * > airMolecules; //Temp

private:
	glm::mat4 toLeft;
	glm::mat4 toRight;

	Sphere * leftHand;
	Sphere * rightHand;

	//TODO cyclinder * laser
	Line * leftLaser;
	Line * rightLaser;

	const int numMolecules = 1;
	int numInSceneMolecules;
};

