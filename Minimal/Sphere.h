#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "Geode.h"

using namespace std;

class Sphere : public Geode
{
public:
	Sphere(float radius, bool wired);
	void draw(GLint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	void genVertices();
	void genIndices();
private:
	int latNum;
	int longNum;
	float m_radius;
	glm::vec3 currPos;
	vector< glm::vec3 > vertices;
	vector< glm::vec3 > normals;
	vector< int > indices;
	const float PI = 3.1415927;
};


