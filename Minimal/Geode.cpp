#include "Geode.h"



Geode::Geode()
{
	/* In case I need to scale the geode */
	scaleVal = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Geode::setToWorld(glm::mat4 toWorld)
{
	this->toWorld = toWorld;
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), scaleVal);	
}

Geode::~Geode()
{
}
