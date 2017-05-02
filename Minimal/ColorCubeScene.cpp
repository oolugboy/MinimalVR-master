#include "ColorCubeScene.h"
#include <iostream>
#include <vector>

using namespace std;
ColorCubeScene::ColorCubeScene()
{
	cube = new Cube(false);
	cube->scaleVal = glm::vec3(1.0f, 1.0f, 1.0f);
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	cubeScaleVal = 10.0f;
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
void ColorCubeScene::expandCube()
{
	if(cubeScaleVal < 100.0f)
	{
		cubeScaleVal *= 1.002f;
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(1.002f));
	}
	cout << " The cube scale val " << cubeScaleVal << endl;
}
void ColorCubeScene::contractCube()
{
	if(cubeScaleVal > 1.0f)
	{
		cubeScaleVal *= 0.998f;
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.998f));
	}
	cout << " The cube scale val " << cubeScaleVal << endl;
}
void ColorCubeScene::resetCubeSize()
{
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f/cubeScaleVal));
	cubeScaleVal = 10.0f;
	cout << " The cube scale val " << cubeScaleVal << endl;
}
void ColorCubeScene::update()
{
	cube->setToWorld(toWorld);
	//cube->spin(1.0f);
}
ColorCubeScene::~ColorCubeScene()
{

}
