#include "Factory.h"
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>



Factory::Factory()
{
	factModel = new Model("../ModelAssets/factory1/factory1.obj");
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -35.0f));
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	factModel->setToWorld(toWorld);
}

void Factory::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	factModel->draw(shaderProgram, projection, modelView);
}

Factory::~Factory()
{
}
