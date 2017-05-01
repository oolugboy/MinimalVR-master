#include "ColorCubeScene.h"
#include <vector>

using namespace std;
ColorCubeScene::ColorCubeScene()
{
	cube = new Cube(false);
	cube->scaleVal = glm::vec3(1.0f, 1.0f, 1.0f);
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -30.0f));
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	cube->setToWorld(toWorld);
}
void ColorCubeScene::render(const mat4 & projection, const mat4 & modelview, GLint shaderProgram)
{	
	cube->draw(shaderProgram, projection, modelview);
}
void ColorCubeScene::loadTextures(const char * fileName)
{
	cube->loadTextures(fileName);
}
void ColorCubeScene::update()
{
	//cube->spin(1.0f);
}
ColorCubeScene::~ColorCubeScene()
{

}
