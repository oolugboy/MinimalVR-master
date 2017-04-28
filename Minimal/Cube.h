#pragma once 

#define GLFW_INCLUDE_GLEXT

#include <GL/glew.h>

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Geode.h"

class Cube : public Geode
{
public:
	Cube(bool wired);
	~Cube();
	void setToWorld(glm::mat4 toWorld);	
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	void update();
	void spin(float);
	void scaleBoundaries();
	void factorSphereCollision(glm::vec3 pos, float radius, glm::vec3 & velocity);	
	float xMax, xMin, yMax, yMin, zMax, zMin;	
	void loadTextures(const char * fileName);
	
};

// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.
const GLfloat vertices[24][3] = {
	// "Front" face
	{ -1.0, -1.0,  1.0 },{ 1.0, -1.0,  1.0 },{ 1.0,  1.0,  1.0 },{ -1.0,  1.0,  1.0 },
	//  "Right" face
	{ 1.0, -1.0, 1.0 },{ 1.0, -1.0, -1.0 },{ 1.0, 1.0, -1.0 },{ 1.0, 1.0, 1.0 },
	// "Back" vertices
	{-1.0, -1.0, -1.0 },{ 1.0, -1.0, -1.0 },{ 1.0,  1.0, -1.0 },{ -1.0,  1.0, -1.0 },
	// "left" face
	{-1.0, -1.0, 1.0},{-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0},
	// "Bottom" face
	{-1.0, -1.0, 1.0},{1.0, -1.0, 1.0},{1.0, -1.0, -1.0},{-1.0, -1.0, -1.0},
	//" top face
	{-1.0, 1.0, 1.0},{1.0, 1.0, 1.0}, {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0}
};

const GLfloat texCoords[24][2] = {
	// " Front face" texCoords
	{ 0.0, 1.0 },{ 1.0, 1.0 },{ 1.0, 0.0 },{ 0.0, 0.0 },
	// " Right face" texCoords
	{ 0.0, 1.0 },{ 1.0, 1.0 },{ 1.0, 0.0 },{ 0.0, 0.0 },
	// " Back face" texCoords
	{ 1.0, 1.0 },{ 1.0, 0.0 },{ 0.0, 0.0 },{ 0.0, 1.0 },
	// " left face texCoords
	{ 1.0, 1.0 },{ 1.0, 0.0 },{ 0.0, 0.0 },{ 0.0, 1.0 },
	// " The bottom face texCoords
	{ 0.0, 0.0 },{ 0.0, 1.0 },{ 1.0, 1.0 },{ 1.0, 0.0 },
	// " Top face texCoords "
	{ 0.0, 1.0 },{ 1.0, 1.0 },{ 1.0, 0.0 },{ 0.0, 0.0 }
};

// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
const GLuint indices[6][6] = {
	// Front face
	{ 0, 1, 2, 2, 3, 0 },
	// Right face
	{ 4, 5, 6, 6, 7, 4 },
	// Back face
	{ 8, 9, 10, 10, 11, 8 },
	// left face
	{ 12, 13, 14, 14, 15, 12 },
	// Bottom face
	{ 16, 17, 18, 18, 19, 16 },
	// Top face
	{ 20, 21, 22, 22, 23, 20 }
};

