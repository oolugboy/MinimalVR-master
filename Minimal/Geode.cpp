#include "Geode.h"
#include <iostream>

using namespace std;

Geode::Geode()
{
	/* In case I need to scale the geode */
	toWorld = glm::mat4(1.0f);
	scaleVal = glm::vec3(1.0f, 1.0f, 1.0f);
	isCube = false;
}
void Geode::printVector(glm::vec3 val)
{
	cout << val.x << " " << val.y << " " << val.z << endl;
}

Geode::~Geode()
{
}
