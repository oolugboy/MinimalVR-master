#include "ColorCubeScene.h"
#include <vector>

using namespace std;
ColorCubeScene::ColorCubeScene()
{
	cube = new Cube();
}
void ColorCubeScene::render(const mat4 & projection, const mat4 & modelview, GLint shaderProgram)
{
	cube->spin(1.0f);
	cube->vrDraw(shaderProgram, projection, modelview);
}

ColorCubeScene::~ColorCubeScene()
{

}
