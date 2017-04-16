#pragma once
#include "Cube.h"
#include "GlfwApp.h"
class ColorCubeScene
{
public:	
	Cube * cube;
	GLuint instanceCount;

	// VBOs for the cube's vertices and normals
	const unsigned int GRID_SIZE{ 5 };
	ColorCubeScene();
	void render(const mat4 & projection, const mat4 & modelview, GLint shaderProgram);
	~ColorCubeScene();
};

