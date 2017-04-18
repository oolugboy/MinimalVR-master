#include "ColorCubeScene.h"
#include <vector>

using namespace std;
ColorCubeScene::ColorCubeScene()
{
	cube = new Cube(false);
}
void ColorCubeScene::render(const mat4 & projection, const mat4 & modelview, GLint shaderProgram)
{
	cube->spin(1.0f);
	cube->draw(shaderProgram, projection, modelview);
}

ColorCubeScene::~ColorCubeScene()
{

}
